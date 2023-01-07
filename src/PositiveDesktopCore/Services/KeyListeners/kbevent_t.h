#pragma once

namespace app::keylistener {

#define ALL_ACTION_ENUM(__type) \
	kbe_switch_##__type = kbe_##__type | kbe_switch, \
	kbe_move_window_##__type = kbe_##__type | kbe_move_window, \
	kbe_move_window_and_switch_##__type = kbe_##__type | kbe_move_window_and_switch, \
	kbe_move_window_exclude_##__type = kbe_##__type | kbe_move_window_exclude, \
	kbe_move_window_exclude_and_switch_##__type = kbe_##__type | kbe_move_window_exclude_and_switch, \
	kbe_move_desktop_##__type = kbe_##__type | kbe_move_desktop

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

		kbe_close_desktop,
		kbe_close_desktop_exclude,

		kbe_switch = 0x20,
		kbe_move_window = 0x40,
		kbe_move_window_and_switch = 0x60,
		kbe_move_window_exclude = 0x80,
		kbe_move_window_exclude_and_switch = 0xA0,
		kbe_move_desktop = 0xC0,
		kbe_op_reserved = 0xE0,

		kbe_target_mask = 0x1F,
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
		kbe_desktop11 = 0x0A, ALL_ACTION_ENUM(desktop11),
		kbe_desktop12 = 0x0B, ALL_ACTION_ENUM(desktop12),
		kbe_desktop13 = 0x0C, ALL_ACTION_ENUM(desktop13),
		kbe_desktop14 = 0x0D, ALL_ACTION_ENUM(desktop14),
		kbe_desktop15 = 0x0E, ALL_ACTION_ENUM(desktop15),
		kbe_desktop16 = 0x0F, ALL_ACTION_ENUM(desktop16),
		kbe_desktop17 = 0x10, ALL_ACTION_ENUM(desktop17),
		kbe_desktop18 = 0x11, ALL_ACTION_ENUM(desktop18),
		kbe_desktop19 = 0x12, ALL_ACTION_ENUM(desktop19),
		kbe_desktop20 = 0x13, ALL_ACTION_ENUM(desktop20),
		kbe_first = 0x14, ALL_ACTION_ENUM(first),
		kbe_last = 0x15, ALL_ACTION_ENUM(last),
		kbe_left = 0x16, ALL_ACTION_ENUM(left),
		kbe_right = 0x17, ALL_ACTION_ENUM(right),
		kbe_new_first = 0x18, ALL_ACTION_ENUM(new_first),
		kbe_new_last = 0x19, ALL_ACTION_ENUM(new_last),
		kbe_new_left = 0x1A, ALL_ACTION_ENUM(new_left),
		kbe_new_right = 0x1B, ALL_ACTION_ENUM(new_right),
		kbe_previous = 0x1C, ALL_ACTION_ENUM(previous),
		kbe_forward = 0x1D, ALL_ACTION_ENUM(forward),
		kbe_desktop_reserved1 = 0x1E, ALL_ACTION_ENUM(desktop_reserved1),
		kbe_desktop_reserved2 = 0x1F, ALL_ACTION_ENUM(desktop_reserved2),
	};

	constexpr kbevent_t flag(kbevent_t ev) noexcept {
		return static_cast<kbevent_t>(ev & 0xE0);
	}

	constexpr int desktop(kbevent_t ev) noexcept {
		return ev & 0x1F;
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t> = nullptr>
	constexpr kbevent_t addingDesktop(kbevent_t ev, T desktop) noexcept {
		return static_cast<kbevent_t>(ev | desktop);
	}

#undef ALL_ACTION_ENUM

}
