#include "pch.h"
#include "DesktopServiceImplWin11.h"

#include <atomic>
#include <vector>
#include <map>

using namespace winrt;

using namespace app::win11;

#pragma region Delegate class

struct VirtualDesktopManagerInternalDelegate21313 final: public IVirtualDesktopManagerInternalDelegate {
	VirtualDesktopManagerInternalDelegate21313(IServiceProvider* serviceProvider) {
		check_hresult(serviceProvider->QueryService(
			app::clsidVirtualDesktopManagerInternal,
			__uuidof(IVirtualDesktopManagerInternal21313),
			virtualDesktopManager_.put_void()));
	}

	inline HRESULT MoveViewToDesktop(IUnknown* pView, IVirtualDesktop* pDesktop) noexcept override {
		return virtualDesktopManager_->MoveViewToDesktop(pView, pDesktop);
	}

	inline HRESULT GetCurrentDesktop(IVirtualDesktop** ppDesktop) noexcept override {
		return virtualDesktopManager_->GetCurrentDesktop(nullptr, ppDesktop);
	}

	inline HRESULT GetDesktops(IObjectArray** ppArray) noexcept override {
		return virtualDesktopManager_->GetDesktops(nullptr, ppArray);
	}

	inline HRESULT GetAdjacentDesktop(IVirtualDesktop* pDesktopOrigin, app::AdjacentDesktopDirection nDirection, IVirtualDesktop** ppDesktop) noexcept override {
		return virtualDesktopManager_->GetAdjacentDesktop(pDesktopOrigin, nDirection, ppDesktop);
	}

private:
	winrt::com_ptr<IVirtualDesktopManagerInternal21313> virtualDesktopManager_;
};

struct VirtualDesktopManagerInternalDelegate21359 final: public IVirtualDesktopManagerInternalDelegate {
	VirtualDesktopManagerInternalDelegate21359(IServiceProvider* serviceProvider) {
		check_hresult(serviceProvider->QueryService(
			app::clsidVirtualDesktopManagerInternal,
			__uuidof(IVirtualDesktopManagerInternal21359),
			virtualDesktopManager_.put_void()));
	}

	inline HRESULT MoveViewToDesktop(IUnknown* pView, IVirtualDesktop* pDesktop) noexcept override {
		return virtualDesktopManager_->MoveViewToDesktop(pView, pDesktop);
	}

	inline HRESULT GetCurrentDesktop(IVirtualDesktop** ppDesktop) noexcept override {
		return virtualDesktopManager_->GetCurrentDesktop(nullptr, ppDesktop);
	}

	inline HRESULT GetDesktops(IObjectArray** ppArray) noexcept override {
		return virtualDesktopManager_->GetDesktops(nullptr, ppArray);
	}

	inline HRESULT GetAdjacentDesktop(IVirtualDesktop* pDesktopOrigin, app::AdjacentDesktopDirection nDirection, IVirtualDesktop** ppDesktop) noexcept override {
		return virtualDesktopManager_->GetAdjacentDesktop(pDesktopOrigin, nDirection, ppDesktop);
	}

private:
	winrt::com_ptr<IVirtualDesktopManagerInternal21359> virtualDesktopManager_;
};

struct VirtualDesktopManagerInternalDelegate22449 final: public IVirtualDesktopManagerInternalDelegate {
	VirtualDesktopManagerInternalDelegate22449(IServiceProvider* serviceProvider) {
		check_hresult(serviceProvider->QueryService(
			app::clsidVirtualDesktopManagerInternal,
			__uuidof(IVirtualDesktopManagerInternal22449),
			virtualDesktopManager_.put_void()));
	}

	inline HRESULT MoveViewToDesktop(IUnknown* pView, IVirtualDesktop* pDesktop) noexcept override {
		return virtualDesktopManager_->MoveViewToDesktop(pView, pDesktop);
	}

	inline HRESULT GetCurrentDesktop(IVirtualDesktop** ppDesktop) noexcept override {
		return virtualDesktopManager_->GetCurrentDesktop(nullptr, ppDesktop);
	}

	inline HRESULT GetDesktops(IObjectArray** ppArray) noexcept override {
		return virtualDesktopManager_->GetDesktops(nullptr, ppArray);
	}

	inline HRESULT GetAdjacentDesktop(IVirtualDesktop* pDesktopOrigin, app::AdjacentDesktopDirection nDirection, IVirtualDesktop** ppDesktop) noexcept override {
		return virtualDesktopManager_->GetAdjacentDesktop(pDesktopOrigin, nDirection, ppDesktop);
	}

private:
	winrt::com_ptr<IVirtualDesktopManagerInternal22449> virtualDesktopManager_;
};

struct VirtualDesktopManagerInternalDelegate25158 final: public IVirtualDesktopManagerInternalDelegate {
	VirtualDesktopManagerInternalDelegate25158(IServiceProvider* serviceProvider) {
		check_hresult(serviceProvider->QueryService(
			app::clsidVirtualDesktopManagerInternal,
			__uuidof(IVirtualDesktopManagerInternal25158),
			virtualDesktopManager_.put_void()));
	}

	inline HRESULT MoveViewToDesktop(IUnknown* pView, IVirtualDesktop* pDesktop) noexcept override {
		return virtualDesktopManager_->MoveViewToDesktop(pView, pDesktop);
	}

	inline HRESULT GetCurrentDesktop(IVirtualDesktop** ppDesktop) noexcept override {
		return virtualDesktopManager_->GetCurrentDesktop(nullptr, ppDesktop);
	}

	inline HRESULT GetDesktops(IObjectArray** ppArray) noexcept override {
		return virtualDesktopManager_->GetDesktops(nullptr, ppArray);
	}

	inline HRESULT GetAdjacentDesktop(IVirtualDesktop* pDesktopOrigin, app::AdjacentDesktopDirection nDirection, IVirtualDesktop** ppDesktop) noexcept override {
		return virtualDesktopManager_->GetAdjacentDesktop(pDesktopOrigin, nDirection, ppDesktop);
	}

private:
	winrt::com_ptr<IVirtualDesktopManagerInternal25158> virtualDesktopManager_;
};

#pragma endregion

#pragma region Support functions

inline guid GetDesktopId(IVirtualDesktop* pDesktop) {
	guid desktopId;
	check_hresult(pDesktop->GetID(&desktopId));
	return desktopId;
}

inline app::VirtualDesktopBag CreateBag(IVirtualDesktop* pDesktop, UINT nIndex) {
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

app::IDesktopServiceImpl* app::win11::CreateDesktopServiceImpl(DWORD build, reps::observer_t& observer) {
	winrt::impl::com_ref<DesktopServiceImplWin11> impl = winrt::make_self<DesktopServiceImplWin11>(build, observer);
	return impl.detach();
}

void app::win11::ReleaseDesktopServiceImpl(IDesktopServiceImpl* impl) {
	reinterpret_cast<IUnknown*>(impl)->Release();
}

DesktopServiceImplWin11::DesktopServiceImplWin11(DWORD build, reps::observer_t& observer)
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

	if (build >= 25158) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate25158>(serviceProvider_.get());
	} else if (build >= 22449) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate22449>(serviceProvider_.get());
	} else if (build >= 21359) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate21359>(serviceProvider_.get());
	} else if (build >= 21313) {
		virtualDesktopManagerDelegate_ = std::make_unique<VirtualDesktopManagerInternalDelegate21313>(serviceProvider_.get());
	} else {
		winrt::throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	}

	check_hresult(serviceProvider_->QueryService(
		clsidVirtualNotificationService,
		__uuidof(IVirtualDesktopNotificationService),
		virtualDesktopNotificationService_.put_void()));

	subject_.addObserver(observer);

	com_ptr<app::win11::IVirtualDesktopNotification> sink;
	check_hresult(QueryInterface(__uuidof(app::win11::IVirtualDesktopNotification), sink.put_void()));
	check_hresult(virtualDesktopNotificationService_->Register(sink.get(), &cookie_));

	loadDesktops();
}

void DesktopServiceImplWin11::loadDesktops() {
	com_ptr<IObjectArray> desktops;
	check_hresult(virtualDesktopManagerDelegate_->GetDesktops(desktops.put()));

	UINT count { 0 };
	check_hresult(desktops->GetCount(&count));
	for (UINT i = 0; i < count; ++i) {
		com_ptr<IVirtualDesktop> desktop;
		check_hresult(desktops->GetAt(i, __uuidof(IVirtualDesktop), desktop.put_void()));

		guid desktopId { GetDesktopId(desktop.get()) };
		VirtualDesktopBag bag { CreateBag(desktop.get(), i) };
		bag_.emplace(desktopId, std::move(bag));
	}
}

void DesktopServiceImplWin11::close() {
	WINRT_ASSERT(cookie_);

	subject_.clearObserver();
	check_hresult(virtualDesktopNotificationService_->Unregister(cookie_));
	cookie_ = 0;
}

#pragma endregion

#pragma region Operation implementation

void DesktopServiceImplWin11::moveForegroundWindowToLeftOfCurrent() const {
	com_ptr<IVirtualDesktop> current;
	check_hresult(virtualDesktopManagerDelegate_->GetCurrentDesktop(current.put()));

	com_ptr<IVirtualDesktop> left;
	check_hresult(virtualDesktopManagerDelegate_->GetAdjacentDesktop(current.get(), AD_LEFT, left.put()));

	com_ptr<IUnknown> view;
	check_hresult(applicationViewCollection_->GetViewInFocus(view.put()));
	check_hresult(virtualDesktopManagerDelegate_->MoveViewToDesktop(view.get(), left.get()));
}

void DesktopServiceImplWin11::moveForegroundWindowToRightOfCurrent() const {
	com_ptr<IVirtualDesktop> current;
	check_hresult(virtualDesktopManagerDelegate_->GetCurrentDesktop(current.put()));

	com_ptr<IVirtualDesktop> right;
	check_hresult(virtualDesktopManagerDelegate_->GetAdjacentDesktop(current.get(), AD_RIGHT, right.put()));

	com_ptr<IUnknown> view;
	check_hresult(applicationViewCollection_->GetViewInFocus(view.put()));
	check_hresult(virtualDesktopManagerDelegate_->MoveViewToDesktop(view.get(), right.get()));
}

#pragma endregion

#pragma region Sink implementation

#include "vdevent_t.h"

HRESULT DesktopServiceImplWin11::VirtualDesktopCreated(IVirtualDesktop* pDesktop) {
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

HRESULT DesktopServiceImplWin11::VirtualDesktopDestroyBegin(IVirtualDesktop* /*pDesktopDestroyed*/, IVirtualDesktop* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT DesktopServiceImplWin11::VirtualDesktopDestroyFailed(IVirtualDesktop* /*pDesktopDestroyed*/, IVirtualDesktop* /*pDesktopFallback*/) {
	return S_OK;
}

HRESULT DesktopServiceImplWin11::VirtualDesktopDestroyed(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* /*pDesktopFallback*/) {
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

HRESULT DesktopServiceImplWin11::Unknown1(int /*nUnknown*/) {
	return S_OK;
}

HRESULT DesktopServiceImplWin11::VirtualDesktopMoved(IVirtualDesktop* pDesktop, int nFromIndex, int nToIndex) {
	guid desktopId { GetDesktopId(pDesktop) };
	auto itr = bag_.find(desktopId);
	if (itr == bag_.end()) {
		// TODO: error log
		return S_OK;
	}

	// e.g. [A, B, C, D]
	if (nFromIndex > nToIndex) { // 3 to 1, expected: [A, D, B, C]
		for (auto& bag2 : bag_) {
			int index { bag2.second.index };
			if (nFromIndex <= index && index < nToIndex) {
				++bag2.second.index;
			}
		}
	} else if (nFromIndex < nToIndex) { // 1 to 3, expected: [A, C, D, B]
		for (auto& bag2 : bag_) {
			int index { bag2.second.index };
			if (nFromIndex < index && index <= nToIndex) {
				--bag2.second.index;
			}
		}
	} else {
		WINRT_ASSERT(false);
		exit(1);
	}

	VirtualDesktopBag& bag = itr->second;
	bag.index = nToIndex;

	if (nFromIndex > 0x1FFFFFFF) {
		// TODO: error log
		return S_OK;
	}
	vdevent_t data {
		vde_moved,
		nFromIndex,
		bag.index,
		bag.name,
	};
	reps::next(subject_, data);
	return S_OK;
}

HRESULT DesktopServiceImplWin11::VirtualDesktopRenamed(IVirtualDesktop* pDesktop, HSTRING abiName) {
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

HRESULT DesktopServiceImplWin11::ViewVirtualDesktopChanged(IUnknown* /*pView*/) {
	return S_OK;
}

HRESULT DesktopServiceImplWin11::CurrentVirtualDesktopChanged(IVirtualDesktop* /*pDesktopOld*/, IVirtualDesktop* pDesktopNew) {
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

HRESULT DesktopServiceImplWin11::VirtualDesktopWallpaperChanged(IVirtualDesktop* /*pDesktop*/, HSTRING /*hPath*/) {
	return S_OK;
}

#pragma endregion
