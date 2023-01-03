#pragma once
#include "Common/lock_t.h"
#include "Common/VirtualDesktop.h"
#include "VirtualDesktopInterface.h"

#include <unordered_map>

namespace app::desktop {

	class VirtualDesktopCache final {
	public:
		~VirtualDesktopCache();

		HRESULT CreateDelegate(IVirtualDesktop* iface, int index, IVirtualDesktopDelegate** ppDelegate) noexcept;
		HRESULT CreateDelegate(IVirtualDesktop2* iface, int index, IVirtualDesktopDelegate** ppDelegate) noexcept;
		HRESULT CreateDelegate(IVirtualDesktop20231* iface, int index, IVirtualDesktopDelegate** ppDelegate) noexcept;

		HRESULT CreateDelegateIfNeeded(IVirtualDesktop* iface, IVirtualDesktopDelegate** ppDelegate) noexcept;
		HRESULT CreateDelegateIfNeeded(IVirtualDesktop2* iface, IVirtualDesktopDelegate** ppDelegate) noexcept;
		HRESULT CreateDelegateIfNeeded(IVirtualDesktop20231* iface, IVirtualDesktopDelegate** ppDelegate) noexcept;

		HRESULT FromInterface(IVirtualDesktop* iface, IVirtualDesktopDelegate** ppDelegate) noexcept;
		HRESULT FromInterface(IVirtualDesktop2* iface, IVirtualDesktopDelegate** ppDelegate) noexcept;
		HRESULT FromInterface(IVirtualDesktop20231* iface, IVirtualDesktopDelegate** ppDelegate) noexcept;

		HRESULT MoveDelegate(IVirtualDesktop20231* iface, int nFromIndex, int nToIndex, IVirtualDesktopDelegate** ppDelegate) noexcept;

		HRESULT DetachDelegate(IVirtualDesktop* iface, IVirtualDesktopDelegate** ppDesktop) noexcept;
		HRESULT DetachDelegate(IVirtualDesktop2* iface, IVirtualDesktopDelegate** ppDesktop) noexcept;
		HRESULT DetachDelegate(IVirtualDesktop20231* iface, IVirtualDesktopDelegate** ppDesktop) noexcept;

		HRESULT First(IVirtualDesktopDelegate** ppDesktop) noexcept;
		HRESULT GetAt(int index, IVirtualDesktopDelegate** ppDesktop) noexcept;
		HRESULT Last(IVirtualDesktopDelegate** ppDesktop) noexcept;

	private:
		using container_type = std::unordered_map<winrt::guid, IVirtualDesktopDelegate*>;

		app::lock_t locker_;
		container_type cache_;
	};

}
