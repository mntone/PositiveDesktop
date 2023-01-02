#pragma once
#include "Common/VirtualDesktop.h"
#include "../VirtualDesktopCache.h"
#include "../VirtualDesktopInterface.h"

namespace app::desktop {

	struct VirtualDesktopNotificationListener10240
		: public IVirtualDesktopNotificationListener
		, winrt::implements<VirtualDesktopNotificationListener10240, IVirtualDesktopNotification, IVirtualDesktopNotification2> {
		VirtualDesktopNotificationListener10240(std::shared_ptr<VirtualDesktopCache> cache, IVirtualDesktopNotificationCallback* callback) noexcept: cache_(cache), callback_(callback) { };

	private:
		// - IVirtualDesktopNotification
		IFACEMETHOD(VirtualDesktopCreated)(IVirtualDesktop* pDesktop);
		IFACEMETHOD(VirtualDesktopDestroyBegin)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyFailed)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyed)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(ViewVirtualDesktopChanged)(IUnknown* pView);
		IFACEMETHOD(CurrentVirtualDesktopChanged)(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew);

		// - IVirtualDesktopNotification2
		IFACEMETHOD(VirtualDesktopNameChanged)(IVirtualDesktop2* pDesktop, HSTRING name);

	private:
		std::shared_ptr<VirtualDesktopCache> cache_;
		IVirtualDesktopNotificationCallback* callback_;
	};

}
