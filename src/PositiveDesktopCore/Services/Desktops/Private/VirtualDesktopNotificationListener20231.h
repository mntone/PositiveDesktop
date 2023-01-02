#pragma once
#include "Common/VirtualDesktop.h"
#include "../VirtualDesktopCache.h"
#include "../VirtualDesktopInterface.h"

namespace app::desktop {

	struct VirtualDesktopNotificationListener20231
		: public IVirtualDesktopNotificationListener
		, winrt::implements<VirtualDesktopNotificationListener20231, IVirtualDesktopNotification20241, IVirtualDesktopNotification20231> {
		VirtualDesktopNotificationListener20231(std::shared_ptr<VirtualDesktopCache> cache, IVirtualDesktopNotificationCallback* callback) noexcept: cache_(cache), callback_(callback) { };

	private:
		// - IVirtualDesktopNotification20231
		IFACEMETHOD(VirtualDesktopCreated)(IVirtualDesktop20231* pDesktop);
		IFACEMETHOD(VirtualDesktopDestroyBegin)(IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyFailed)(IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyed)(IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopIsPerMonitorChanged)(BOOL bPerMonitor);
		IFACEMETHOD(VirtualDesktopNameChanged)(IVirtualDesktop20231* pDesktop, HSTRING name);
		IFACEMETHOD(ViewVirtualDesktopChanged)(IUnknown* pView);
		IFACEMETHOD(CurrentVirtualDesktopChanged)(IVirtualDesktop20231* pDesktopOld, IVirtualDesktop20231* pDesktopNew);

	private:
		std::shared_ptr<VirtualDesktopCache> cache_;
		IVirtualDesktopNotificationCallback* callback_;
	};

}
