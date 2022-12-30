#include "pch.h"
#include "DesktopServiceImplWin10.h"

#include <atomic>
#include <vector>
#include <map>

using namespace winrt;

using namespace app::win10;

#pragma region Support functions

inline guid GetDesktopId(IVirtualDesktop* pDesktop) {
	guid desktopId;
	check_hresult(pDesktop->GetID(&desktopId));
	return desktopId;
}

inline app::VirtualDesktopBag CreateBag(IVirtualDesktop2* pDesktop, UINT nIndex) {
	hstring desktopName;
	{
		HSTRING abiDesktopName;
		HRESULT hr = pDesktop->GetDesktopName(&abiDesktopName);
		if (SUCCEEDED(hr)) {
			attach_abi(desktopName, abiDesktopName);
		}
	}

	app::VirtualDesktopBag bag {
		static_cast<int>(nIndex),
		desktopName,
	};
	return bag;
}

#pragma endregion

#pragma region Service implementation

app::IDesktopServiceImpl* app::win10::CreateDesktopServiceImpl(reps::observer_t& observer) {
	impl::com_ref<DesktopServiceImplWin10> impl = make_self<DesktopServiceImplWin10>(observer);
	return impl.detach();
}

void app::win10::ReleaseDesktopServiceImpl(IDesktopServiceImpl* impl) {
	reinterpret_cast<IUnknown*>(impl)->Release();
}

DesktopServiceImplWin10::DesktopServiceImplWin10(reps::observer_t& observer)
	: serviceProvider_(nullptr)
	, virtualDesktopNotificationService_(nullptr)
	, cookie_(0) {
	check_hresult(CoCreateInstance(
		clsidImmersiveShell,
		nullptr,
		CLSCTX_LOCAL_SERVER,
		guid_of<IServiceProvider>(),
		serviceProvider_.put_void()));

	check_hresult(serviceProvider_->QueryService(
		__uuidof(IApplicationViewCollection),
		__uuidof(IApplicationViewCollection),
		applicationViewCollection_.put_void()));

	check_hresult(serviceProvider_->QueryService(
		clsidVirtualDesktopManagerInternal,
		__uuidof(IVirtualDesktopManagerInternal2),
		virtualDesktopManager_.put_void()));

	check_hresult(serviceProvider_->QueryService(
		clsidVirtualNotificationService,
		__uuidof(IVirtualDesktopNotificationService),
		virtualDesktopNotificationService_.put_void()));

	subject_.addObserver(observer);

	com_ptr<app::win10::IVirtualDesktopNotification2> sink;
	check_hresult(QueryInterface(__uuidof(app::win10::IVirtualDesktopNotification2), sink.put_void()));
	check_hresult(virtualDesktopNotificationService_->Register(sink.get(), &cookie_));

	loadDesktops();
}

void DesktopServiceImplWin10::loadDesktops() {
	com_ptr<IObjectArray> desktops;
	check_hresult(virtualDesktopManager_->GetDesktops(desktops.put()));

	UINT count { 0 };
	check_hresult(desktops->GetCount(&count));
	for (UINT i = 0; i < count; ++i) {
		com_ptr<IVirtualDesktop2> desktop;
		check_hresult(desktops->GetAt(i, __uuidof(IVirtualDesktop2), desktop.put_void()));

		guid desktopId { GetDesktopId(desktop.get()) };
		VirtualDesktopBag bag { CreateBag(desktop.get(), i) };
		bag_.emplace(desktopId, std::move(bag));
	}
}

void DesktopServiceImplWin10::close() {
	WINRT_ASSERT(cookie_);

	subject_.clearObserver();
	check_hresult(virtualDesktopNotificationService_->Unregister(cookie_));
	cookie_ = 0;
}

#pragma endregion

#pragma region Operation implementation

void DesktopServiceImplWin10::moveForegroundWindowToLeftOfCurrent() const {
	com_ptr<IVirtualDesktop> current;
	check_hresult(virtualDesktopManager_->GetCurrentDesktop(current.put()));

	com_ptr<IVirtualDesktop> left;
	check_hresult(virtualDesktopManager_->GetAdjacentDesktop(current.get(), AD_LEFT, left.put()));

	com_ptr<IUnknown> view;
	check_hresult(applicationViewCollection_->GetViewForHwnd(GetForegroundWindow(), view.put()));
	check_hresult(virtualDesktopManager_->MoveViewToDesktop(view.get(), left.get()));
}

void DesktopServiceImplWin10::moveForegroundWindowToRightOfCurrent() const {
	com_ptr<IVirtualDesktop> current;
	check_hresult(virtualDesktopManager_->GetCurrentDesktop(current.put()));

	com_ptr<IVirtualDesktop> right;
	check_hresult(virtualDesktopManager_->GetAdjacentDesktop(current.get(), AD_RIGHT, right.put()));

	com_ptr<IUnknown> view;
	check_hresult(applicationViewCollection_->GetViewForHwnd(GetForegroundWindow(), view.put()));
	check_hresult(virtualDesktopManager_->MoveViewToDesktop(view.get(), right.get()));
}

#pragma endregion

#pragma region Sink implementation

#include "vdevent_t.h"

HRESULT DesktopServiceImplWin10::VirtualDesktopCreated(IVirtualDesktop* pDesktop) {
	guid desktopId { GetDesktopId(pDesktop) };
	VirtualDesktopBag bag {
		static_cast<int>(bag_.size()),
		L"",
	};
	bag_.emplace(desktopId, std::move(bag));

	vdevent_t data {
		vde_created,
		0,
		bag.index,
		L"",
	};
	reps::next(subject_, data);
	return S_OK;
}

HRESULT DesktopServiceImplWin10::VirtualDesktopDestroyBegin(IVirtualDesktop* /*pDesktopDestroyed*/, IVirtualDesktop* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT DesktopServiceImplWin10::VirtualDesktopDestroyFailed(IVirtualDesktop* /*pDesktopDestroyed*/, IVirtualDesktop* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT DesktopServiceImplWin10::VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* /*pDesktopFallback*/) {
	guid desktopId { GetDesktopId(pDesktopDestroyed) };
	auto itr = bag_.find(desktopId);
	if (itr == bag_.end()) {
		// TODO: error log
		return S_OK;
	}

	VirtualDesktopBag bag { itr->second };
	for (auto& bag2 : bag_) {
		if (bag2.second.index > bag.index) {
			--bag2.second.index;
		}
	}
	bag_.erase(itr);

	vdevent_t data {
		vde_removed,
		0,
		bag.index,
		bag.name,
	};
	reps::next(subject_, data);
	return S_OK;
}

HRESULT DesktopServiceImplWin10::ViewVirtualDesktopChanged(IUnknown* /*pView*/) {
	return S_OK;
}

HRESULT DesktopServiceImplWin10::CurrentVirtualDesktopChanged(IVirtualDesktop* /*pDesktopOld*/, IVirtualDesktop* pDesktopNew) {
	guid desktopId { GetDesktopId(pDesktopNew) };
	auto itr = bag_.find(desktopId);
	if (itr == bag_.cend()) {
		// TODO: error log
		return S_OK;
	}

	VirtualDesktopBag const& bag = itr->second;
	vdevent_t data {
		vde_changed,
		0,
		bag.index,
		bag.name,
	};
	reps::next(subject_, data);
	return S_OK;
}

HRESULT DesktopServiceImplWin10::VirtualDesktopRenamed(IVirtualDesktop2* pDesktop, HSTRING abiName) {
	guid desktopId { GetDesktopId(pDesktop) };
	auto itr = bag_.find(desktopId);
	if (itr == bag_.end()) {
		// TODO: error log
		return S_OK;
	}

	winrt::hstring desktopName;
	winrt::copy_from_abi(desktopName, abiName);

	VirtualDesktopBag& bag = itr->second;
	bag.name = desktopName;

	vdevent_t data {
		vde_renamed,
		0,
		bag.index,
		desktopName,
	};
	reps::next(subject_, data);
	return S_OK;
}

#pragma endregion
