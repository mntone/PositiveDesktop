#pragma once
#include "Common/RepsBase.h"

namespace app {

	struct VirtualDesktopBag {
		int index;
		winrt::hstring name;
	};

	struct IVirtualDesktopNotificationServiceImpl {
		virtual ~IVirtualDesktopNotificationServiceImpl() { }

		virtual void moveForegroundWindowToLeftOfCurrent() const = 0;
		virtual void moveForegroundWindowToRightOfCurrent() const = 0;
		virtual void close() = 0;
	};

	static constexpr winrt::guid clsidImmersiveShell { 0xC2F03A33, 0x21F5, 0x47FA, { 0xB4,0xBB,0x15,0x63,0x62,0xA2,0xF2,0x39 } };
	static constexpr winrt::guid clsidVirtualDesktopManagerInternal { 0xC5E0CDCA, 0x7B6E, 0x41B2, { 0x9F, 0xC4, 0xD9, 0x39, 0x75, 0xCC, 0x46, 0x7B } };
	static constexpr winrt::guid clsidVirtualNotificationService { 0xA501FDEC, 0x4A09, 0x464C, { 0xAE,0x4E,0x1B,0x9C,0x21,0xB8,0x49,0x18 } };

	namespace win10 {

		IVirtualDesktopNotificationServiceImpl* CreateVirtualDesktopNotificationServiceImpl(reps::observer_t& observer);
		void ReleaseVirtualDesktopNotificationServiceImpl(IVirtualDesktopNotificationServiceImpl* impl);

	}

	namespace win11 {

		IVirtualDesktopNotificationServiceImpl* CreateVirtualDesktopNotificationServiceImpl(DWORD build, reps::observer_t& observer);
		void ReleaseVirtualDesktopNotificationServiceImpl(IVirtualDesktopNotificationServiceImpl* impl);

	}

}
