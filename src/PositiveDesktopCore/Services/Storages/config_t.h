#pragma once

#include "Services/KeyListeners/kbevent_t.h"

#include <vector>

namespace app::storage {

	enum notification_mode_t: unsigned int {
		nfm_default,
		nfm_disabled,
		nfm_primary,
		nfm_cursor,
		nfm_all,
	};
	constexpr notification_mode_t kNotificationDefault { nfm_default };

	enum theme_t: unsigned int {
		thm_default,
		thm_system,
		thm_light,
		thm_dark,
		thm_accent,
	};
	constexpr theme_t kThemeDefault { thm_default };

	enum corner_t: unsigned int {
		cnr_default,
		cnr_rounded,
		cnr_rounded_small,
		cnr_squared,
	};
	constexpr corner_t kCornerDefault { cnr_default };

	using duration_t = unsigned int;
	constexpr unsigned int kDurationParent = 0; /* parent */
	constexpr unsigned int kDurationDefault = 9; /* 3s */
	constexpr float kDurationDefaultFloat = 3.f; /* 3s */

	enum backdrop_t: unsigned int {
		bkd_default,
		bkd_plain,
		bkd_acrylic,
		bkd_mica,
	};
	constexpr backdrop_t kBackdropDefault { bkd_default };

	enum inactive_backdrop_t: unsigned int {
		ibd_default,
		ibd_disabled,
		ibd_enabled,
	};
	constexpr inactive_backdrop_t kInactiveBackdropDefault { ibd_default };

	enum position_origin_t: unsigned int {
		pon_default,
		pon_workArea,
		pon_activeWindow,
		pon_cursor,
	};
	constexpr position_origin_t kPositionOriginDefault { pon_default };

	using position_t = unsigned int;
	constexpr unsigned int kPositionXParent = 0; /* parent */
	constexpr unsigned int kPositionYParent = 0; /* parent */
	constexpr unsigned int kPositionXDefault = 5; /* center */
	constexpr unsigned int kPositionYDefault = 5; /* center */
	constexpr float kPositionXDefaultFloat = 0.f; /* center */
	constexpr float kPositionYDefaultFloat = 0.f; /* center */

	using scale_t = unsigned int;
	constexpr unsigned int kScaleDefault = 5; /* 100% */
	constexpr float kScaleDefaultFloat = 1.0; /* 100% */

	struct desktop_t final {
#pragma pack(4)
		theme_t theme : 3 { kThemeDefault }; // theme
		int : 1;
		backdrop_t backdrop : 2 { kBackdropDefault };
		inactive_backdrop_t inactiveBackdrop : 2 { kInactiveBackdropDefault };
		corner_t corner : 2 { kCornerDefault };  // [Windows 11] square corner
		duration_t duration : 6 { kDurationDefault };   // 0 → parent, 1 → 0.25, 2 → 0.75, 3-63 → 0.0, 0.5, ..., 30.0 [s]

		position_origin_t positionOrigin : 2 { kPositionOriginDefault };
		position_t positionX : 3 { kPositionXDefault }; // 0 → parent, 1, 2 → reserved, 3-7 → 0.0～1.0
		position_t positionY : 3 { kPositionYDefault }; // 0 → parent, 1, 2 → reserved, 3-7 → 0.0～1.0

		scale_t scale : 4 { kScaleDefault }; // 0 → parent, 1-15 → 80%～150% (interval is 5%)
		int : 4;
#pragma pack()
	};

	struct default_desktop_t final {
		desktop_t desktop;
	};

	enum override_mode_t: unsigned int {
		orm_display_index, // Display index
		orm_desktop_index, // Desktop index
		orm_desktop_name,  // Desktop name
		orm_default = orm_display_index,
	};

	enum override_text_mode_t: unsigned int {
		ttt_equals,
		ttt_contains,
		ttt_startswith,
		ttt_endswith,
		ttt_default = ttt_equals,
	};

	constexpr unsigned int kEnabledDefault = 1; /* true */
	constexpr unsigned int kIndexDefault = 0;
	constexpr wchar_t const* kNameDefault = L"";
	struct override_desktop_t final {
		winrt::guid uuid;
		unsigned int enabled : 1 { kEnabledDefault };
		override_text_mode_t textMode : 3 { ttt_default };
		override_mode_t overrideMode : 4 { orm_default };
		desktop_t desktop;
		union {
			unsigned int index { kIndexDefault };
			wchar_t const* text;
		};

		~override_desktop_t() {
			switch (overrideMode) {
			case orm_desktop_name:
				wchar_t const* text2 = std::exchange(text, nullptr);
				if (text2) {
					delete[] text2;
				}
				break;
			}
		}
	};

	enum desktop_property_t: int {
		prp_enabled = 1 << 0,
		prp_theme = 1 << 1,
		prp_backdrop = 1 << 2,
		prp_inactiveBackdrop = 1 << 3,
		prp_corner = 1 << 4,
		prp_duration = 1 << 5,
		prp_positionMode = 1 << 6,
		prp_positionX = 1 << 7,
		prp_positionY = 1 << 8,
		prp_position = prp_positionMode | prp_positionX | prp_positionY,
		prp_scale = 1 << 9,
		prp_all = std::numeric_limits<int>::max(),
	};

	struct desktop_update_t final {
		desktop_property_t property;
		union {
			theme_t theme;
			backdrop_t backdrop;
			inactive_backdrop_t inactiveBackdrop;
			corner_t corner;
			duration_t duration;
			position_origin_t positionOrigin;
			position_t positionX, positionY;
			scale_t scale;
		} newValue;
	};

	struct config_t final {
		notification_mode_t mode { kNotificationDefault };
		default_desktop_t defaultDesktop;
		std::vector<override_desktop_t> desktops;
	};

	constexpr short kKeyDefault = 0;
	struct key_t final {
		short raw;

		key_t(): raw(0) { }
		key_t(short raw): raw(raw) { }
		key_t(char key, char extra): raw(extra << 8 | key) { }

		constexpr char key() const noexcept { return (0x00FF & raw); }
		constexpr char extra() const noexcept { return (0xFF00 & raw) >> 8; }

		constexpr bool empty() const noexcept { return raw == 0; }

		constexpr bool leftCtrl() const noexcept { return 0x2000 & raw; }
		constexpr bool rightCtrl() const noexcept { return 0x1000 & raw; }
		constexpr bool leftWin() const noexcept { return 0x0800 & raw; }
		constexpr bool rightWin() const noexcept { return 0x0400 & raw; }
		constexpr bool leftAlt() const noexcept { return 0x0200 & raw; }
		constexpr bool rightAlt() const noexcept { return 0x0100 & raw; }
	};

	struct keymap_t final {
		app::keylistener::kbevent_t ev;
		key_t key1;
		key_t key2;

		keymap_t(app::keylistener::kbevent_t ev): ev(ev), key1(), key2() { }
		keymap_t(app::keylistener::kbevent_t ev, key_t key1): ev(ev), key1(key1), key2() { }
		keymap_t(app::keylistener::kbevent_t ev, key_t key1, key_t key2): ev(ev), key1(key1), key2(key2) { }
	};

	constexpr bool kSeparateDefault = true;
	struct keymaps_t final {
		bool separate;
		std::vector<app::storage::keymap_t> keymaps;
	};

	struct keymap_update_t final {
	};

	class IConfigManager {
	public:
		virtual ~IConfigManager() = default;

		virtual void Reset() = 0;

		virtual config_t Load() = 0;
		virtual default_desktop_t LoadDefaultDesktop() = 0;
		virtual keymaps_t LoadKeymaps() = 0;

		virtual void store(config_t config)  const = 0;
		virtual void store(default_desktop_t config)  const = 0;
		virtual void store(keymaps_t const& config) const = 0;
	};

}
