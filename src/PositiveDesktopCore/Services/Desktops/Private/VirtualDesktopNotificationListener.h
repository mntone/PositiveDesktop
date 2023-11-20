#pragma once
#include "Common/VirtualDesktop.h"
#include "../VirtualDesktopCache.h"
#include "../VirtualDesktopInterface.h"

namespace app::desktop {

	struct VirtualDesktopNotificationListener10240
		: public IVirtualDesktopNotificationListener
		, winrt::implements<VirtualDesktopNotificationListener10240, IVirtualDesktopNotification> {
		VirtualDesktopNotificationListener10240(std::shared_ptr<VirtualDesktopCache> cache, IVirtualDesktopNotificationCallback* callback) noexcept: cache_(cache), callback_(callback) { };

	private:
		// - IVirtualDesktopNotification
		IFACEMETHOD(VirtualDesktopCreated)(IVirtualDesktop* pDesktop);
		IFACEMETHOD(VirtualDesktopDestroyBegin)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyFailed)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyed)(IVirtualDesktop* pDesktopDestroyed, IVirtualDesktop* pDesktopFallback);
		IFACEMETHOD(ViewVirtualDesktopChanged)(IUnknown* pView);
		IFACEMETHOD(CurrentVirtualDesktopChanged)(IVirtualDesktop* pDesktopOld, IVirtualDesktop* pDesktopNew);

	private:
		std::shared_ptr<VirtualDesktopCache> cache_;
		IVirtualDesktopNotificationCallback* callback_;
	};

	struct VirtualDesktopNotificationListener18963
		: public IVirtualDesktopNotificationListener
		, winrt::implements<VirtualDesktopNotificationListener18963, IVirtualDesktopNotification2, IVirtualDesktopNotification> {
		VirtualDesktopNotificationListener18963(std::shared_ptr<VirtualDesktopCache> cache, IVirtualDesktopNotificationCallback* callback) noexcept: cache_(cache), callback_(callback) { };

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

	struct VirtualDesktopNotificationListener21359
		: public IVirtualDesktopNotificationListener
		, winrt::implements<VirtualDesktopNotificationListener21359, IVirtualDesktopNotification21359> {
		VirtualDesktopNotificationListener21359(std::shared_ptr<VirtualDesktopCache> cache, IVirtualDesktopNotificationCallback* callback) noexcept: cache_(cache), callback_(callback) { };

	private:
		// - IVirtualDesktopNotification21359
		IFACEMETHOD(VirtualDesktopCreated)(IObjectArray* pArray, IVirtualDesktop20231* pDesktop);
		IFACEMETHOD(VirtualDesktopDestroyBegin)(IObjectArray* pArray, IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyFailed)(IObjectArray* pArray, IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyed)(IObjectArray* pArray, IVirtualDesktop20231* pDesktopDestroyed, IVirtualDesktop20231* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopIsPerMonitorChanged)(BOOL bPerMonitor);
		IFACEMETHOD(VirtualDesktopMoved)(IObjectArray* pArray, IVirtualDesktop20231* pDesktop, int nFromIndex, int nToIndex);
		IFACEMETHOD(VirtualDesktopNameChanged)(IVirtualDesktop20231* pDesktop, HSTRING name);
		IFACEMETHOD(ViewVirtualDesktopChanged)(IUnknown* pView);
		IFACEMETHOD(CurrentVirtualDesktopChanged)(IObjectArray* pArray, IVirtualDesktop20231* pDesktopOld, IVirtualDesktop20231* pDesktopNew);
		IFACEMETHOD(VirtualDesktopWallpaperChanged)(IVirtualDesktop20231* pDesktop, HSTRING path);

	private:
		std::shared_ptr<VirtualDesktopCache> cache_;
		IVirtualDesktopNotificationCallback* callback_;
	};

	struct VirtualDesktopNotificationListener22621_2215
		: public IVirtualDesktopNotificationListener
		, winrt::implements<VirtualDesktopNotificationListener22621_2215, IVirtualDesktopNotification22621_2215> {
		VirtualDesktopNotificationListener22621_2215(std::shared_ptr<VirtualDesktopCache> cache, IVirtualDesktopNotificationCallback* callback) noexcept: cache_(cache), callback_(callback) { };

	private:
		// - IVirtualDesktopNotification22621_2215
		IFACEMETHOD(VirtualDesktopCreated)(IObjectArray* pArray, IVirtualDesktop22621_2215* pDesktop);
		IFACEMETHOD(VirtualDesktopDestroyBegin)(IObjectArray* pArray, IVirtualDesktop22621_2215* pDesktopDestroyed, IVirtualDesktop22621_2215* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyFailed)(IObjectArray* pArray, IVirtualDesktop22621_2215* pDesktopDestroyed, IVirtualDesktop22621_2215* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopDestroyed)(IObjectArray* pArray, IVirtualDesktop22621_2215* pDesktopDestroyed, IVirtualDesktop22621_2215* pDesktopFallback);
		IFACEMETHOD(VirtualDesktopMoved)(IObjectArray* pArray, IVirtualDesktop22621_2215* pDesktop, int nFromIndex, int nToIndex);
		IFACEMETHOD(VirtualDesktopNameChanged)(IVirtualDesktop22621_2215* pDesktop, HSTRING name);
		IFACEMETHOD(ViewVirtualDesktopChanged)(IUnknown* pView);
		IFACEMETHOD(CurrentVirtualDesktopChanged)(IObjectArray* pArray, IVirtualDesktop22621_2215* pDesktopOld, IVirtualDesktop22621_2215* pDesktopNew);
		IFACEMETHOD(VirtualDesktopWallpaperChanged)(IVirtualDesktop22621_2215* pDesktop, HSTRING path);
		IFACEMETHOD(VirtualDesktopSwitched)(IVirtualDesktop22621_2215** ppDesktop);
		IFACEMETHOD(RemoteVirtualDesktopConnected)(IVirtualDesktop22621_2215** ppDesktop);

	private:
		std::shared_ptr<VirtualDesktopCache> cache_;
		IVirtualDesktopNotificationCallback* callback_;
	};

}
