#pragma once
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

		IVirtualDesktopDelegate* DeleteDelegate(IVirtualDesktop* iface);
		IVirtualDesktopDelegate* DeleteDelegate(IVirtualDesktop2* iface);
		IVirtualDesktopDelegate* DeleteDelegate(IVirtualDesktop20231* iface);

	private:
		std::unordered_map<winrt::guid, IVirtualDesktopDelegate*> cache_;
	};

}
