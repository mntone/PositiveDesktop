#pragma once
#include "VirtualDesktopNotificationServiceImpl.h"

#include "Common/VirtualDesktop.h"
#include "Common/Reps.h"

namespace app::win10 {

	struct VirtualDesktopNotificationSink
		: winrt::implements<VirtualDesktopNotificationSink, IVirtualDesktopNotification, IVirtualDesktopNotification2> {
		IFACEMETHOD(VirtualDesktopCreated)(IVirtualDesktop* pDesktop);
		IFACEMETHOD(VirtualDesktopDestroyBegin)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyFailed)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyed)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(ViewVirtualDesktopChanged)(void* pView);
		IFACEMETHOD(CurrentVirtualDesktopChanged)(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew);
		IFACEMETHOD(VirtualDesktopRenamed)(IVirtualDesktop* pDesktop, HSTRING name);

		void FASTCALL addObserver(reps::observer_t& observer) noexcept;
		void clearObserver() noexcept;

	private:
		reps::single_subject_t subject_;
	};

	struct VirtualDesktopNotificationServiceImpl final: public app::IVirtualDesktopNotificationServiceImpl {
		VirtualDesktopNotificationServiceImpl(reps::observer_t& observer);

		void close() override;

	private:
		winrt::com_ptr<IServiceProvider> serviceProvider_;
		winrt::com_ptr<IVirtualDesktopNotificationService> virtualDesktopNotificationService_;
		winrt::com_ptr<VirtualDesktopNotificationSink> sink_;
		DWORD cookie_;
	};

}
