#pragma once
#include "VirtualDesktopNotificationServiceImpl.h"

#include "Common/VirtualDesktop.h"
#include "Common/Reps.h"

namespace app::win10 {

	struct VirtualDesktopNotificationServiceWin10
		: public app::IVirtualDesktopNotificationServiceImpl
		, winrt::implements<VirtualDesktopNotificationServiceWin10, IVirtualDesktopNotification, IVirtualDesktopNotification2> {
		VirtualDesktopNotificationServiceWin10(reps::observer_t& observer);

		void close() override;

		// - IVirtualDesktopNotification
		IFACEMETHOD(VirtualDesktopCreated)(IVirtualDesktop* pDesktop);
		IFACEMETHOD(VirtualDesktopDestroyBegin)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyFailed)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyed)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(ViewVirtualDesktopChanged)(void* pView);
		IFACEMETHOD(CurrentVirtualDesktopChanged)(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew);

		// - IVirtualDesktopNotification2
		IFACEMETHOD(VirtualDesktopRenamed)(IVirtualDesktop* pDesktop, HSTRING name);

	private:
		void loadDesktop() noexcept;

	private:
		winrt::com_ptr<IServiceProvider> serviceProvider_;
		winrt::com_ptr<IVirtualDesktopNotificationService> virtualDesktopNotificationService_;
		DWORD cookie_;
		reps::single_subject_t subject_;
	};

}
