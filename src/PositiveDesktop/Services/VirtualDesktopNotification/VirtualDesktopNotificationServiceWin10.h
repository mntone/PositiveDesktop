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

		// - Operations
		void moveForegroundWindowToLeftOfCurrent() const override;
		void moveForegroundWindowToRightOfCurrent() const override;

		// - IVirtualDesktopNotification
		IFACEMETHOD(VirtualDesktopCreated)(IVirtualDesktop* pDesktop);
		IFACEMETHOD(VirtualDesktopDestroyBegin)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyFailed)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyed)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(ViewVirtualDesktopChanged)(IUnknown* pView);
		IFACEMETHOD(CurrentVirtualDesktopChanged)(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew);

		// - IVirtualDesktopNotification2
		IFACEMETHOD(VirtualDesktopRenamed)(IVirtualDesktop2* pDesktop, HSTRING name);

	private:
		void loadDesktops();

	private:
		winrt::com_ptr<IServiceProvider> serviceProvider_;
		winrt::com_ptr<IApplicationViewCollection> applicationViewCollection_;
		winrt::com_ptr<IVirtualDesktopManagerInternal2> virtualDesktopManager_;
		winrt::com_ptr<IVirtualDesktopNotificationService> virtualDesktopNotificationService_;
		DWORD cookie_;
		reps::single_subject_t subject_;

		std::unordered_map<winrt::guid, VirtualDesktopBag> bag_;
	};

}
