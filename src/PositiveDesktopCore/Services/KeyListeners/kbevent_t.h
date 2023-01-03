#pragma once

namespace app::keylistener {

#define ALL_ACTION_ENUM(__type) \
	kbe_switch_##__type = kbe_##__type | kbe_switch, \
	kbe_move_window_##__type = kbe_##__type | kbe_move_window, \
	kbe_move_window_exclude_##__type = kbe_##__type | kbe_move_window_exclude, \
	kbe_move_window_exclude_app_##__type = kbe_##__type | kbe_move_window_exclude_app, \
	kbe_move_window_and_switch_##__type = kbe_##__type | kbe_move_window_and_switch, \
	kbe_move_window_exclude_and_switch_##__type = kbe_##__type | kbe_move_window_exclude_and_switch, \
	kbe_move_window_exclude_app_and_switch_##__type = kbe_##__type | kbe_move_window_exclude_app_and_switch, \
	kbe_move_desktop_##__type = kbe_##__type | kbe_move_desktop, \
	kbe_move_desktop_exclude_##__type = kbe_##__type | kbe_move_desktop_exclude, \
	kbe_move_desktop_exclude_app_##__type = kbe_##__type | kbe_move_desktop_exclude_app, \
	kbe_close_##__type = kbe_##__type | kbe_close

	enum kbevent_t: unsigned char {
		kbe_extend,

		kbe_exit = kbe_extend,
		kbe_rename,
		kbe_settings,
		kbe_taskview,
		kbe_notification_toggle,
		kbe_topmost_toggle,
		kbe_pin_toggle,
		kbe_pin_app_toggle,

		kbe_topmost,
		kbe_topmost_remove,
		kbe_pin,
		kbe_unpin,
		kbe_pin_app,
		kbe_unpin_app,
		kbe_notification_enable,
		kbe_notification_disable,

		kbe_switch = 0x10,
		kbe_move_window = 0x20,
		kbe_move_window_exclude = 0x30,
		kbe_move_window_exclude_app = 0x40,
		kbe_move_window_and_switch = 0x50,
		kbe_move_window_exclude_and_switch = 0x60,
		kbe_move_window_exclude_app_and_switch = 0x70,
		kbe_move_desktop = 0x80,
		kbe_move_desktop_exclude = 0x90,
		kbe_move_desktop_exclude_app = 0xA0,
		kbe_close = 0xB0,
		kbe_op_reserved1 = 0xC0,
		kbe_op_reserved2 = 0xD0,
		kbe_op_reserved3 = 0xE0,

		kbe_op_extend = 0xF0,
		kbe_close_desktop_exclude = kbe_op_extend | 0x00,

		kbe_target_mask = 0x0F,
		kbe_desktop1 = 0x00, ALL_ACTION_ENUM(desktop1),
		kbe_desktop2 = 0x01, ALL_ACTION_ENUM(desktop2),
		kbe_desktop3 = 0x02, ALL_ACTION_ENUM(desktop3),
		kbe_desktop4 = 0x03, ALL_ACTION_ENUM(desktop4),
		kbe_desktop5 = 0x04, ALL_ACTION_ENUM(desktop5),
		kbe_desktop6 = 0x05, ALL_ACTION_ENUM(desktop6),
		kbe_desktop7 = 0x06, ALL_ACTION_ENUM(desktop7),
		kbe_desktop8 = 0x07, ALL_ACTION_ENUM(desktop8),
		kbe_desktop9 = 0x08, ALL_ACTION_ENUM(desktop9),
		kbe_desktop10 = 0x09, ALL_ACTION_ENUM(desktop10),
		kbe_first = 0x0A, ALL_ACTION_ENUM(first),
		kbe_last = 0x0B, ALL_ACTION_ENUM(last),
		kbe_left = 0x0C, ALL_ACTION_ENUM(left),
		kbe_right = 0x0D, ALL_ACTION_ENUM(right),
		kbe_new = 0x0E, ALL_ACTION_ENUM(new),
		kbe_previous = 0x0F, ALL_ACTION_ENUM(previous),
	};

	constexpr kbevent_t flag(kbevent_t ev) noexcept {
		return static_cast<kbevent_t>(ev & 0xF0);
	}

	constexpr int desktop(kbevent_t ev) noexcept {
		return ev & 0x0F;
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t> = nullptr>
	constexpr kbevent_t addingDesktop(kbevent_t ev, T desktop) noexcept {
		return static_cast<kbevent_t>(ev | desktop);
	}

#undef ALL_ACTION_ENUM

}
