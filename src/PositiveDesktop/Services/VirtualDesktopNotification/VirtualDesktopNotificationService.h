#pragma once

namespace app {

	struct IVirtualDesktopNotificationServiceImpl {
		virtual ~IVirtualDesktopNotificationServiceImpl() { }

		virtual void close() = 0;
	};

	static constexpr winrt::guid clsidImmersiveShell { 0xC2F03A33, 0x21F5, 0x47FA, { 0xB4,0xBB,0x15,0x63,0x62,0xA2,0xF2,0x39 } };
	static constexpr winrt::guid clsidVirtualNotificationService { 0xA501FDEC, 0x4A09, 0x464C, { 0xAE,0x4E,0x1B,0x9C,0x21,0xB8,0x49,0x18 } };

}
