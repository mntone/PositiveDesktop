#pragma once

#include <cmath>
#include <vector>

namespace app::storage {

	enum notification_mode_t: unsigned int {
		nfm_default,
		nfm_disabled,
		nfm_primary,
		nfm_cursor,
		nfm_all,
	};
	constexpr notification_mode_t kNotificationDefault = nfm_default;

	enum theme_t: unsigned int {
		thm_default,
		thm_system,
		thm_light,
		thm_dark,
		thm_accent,
	};
	constexpr theme_t kThemeDefault = thm_default;

	enum corner_t: unsigned int {
		cnr_default,
		cnr_rounded,
		cnr_rounded_small,
		cnr_squared,
	};
	constexpr corner_t kCornerDefault = cnr_default;

	enum inactive_backdrop_t: unsigned int {
		ibd_default,
		ibd_disabled,
		ibd_enabled,
	};
	constexpr inactive_backdrop_t kInactiveBackdropDefault = ibd_default;

	enum position_mode_t: unsigned int {
		psn_default,
		psn_workarea,
		psn_window,
		psn_cursor,
	};
	constexpr position_mode_t kPositionModeDefault = psn_default;

	constexpr unsigned int kDurationParent = 0; /* parent */
	constexpr unsigned int kPositionXParent = 0; /* parent */
	constexpr unsigned int kPositionYParent = 0; /* parent */

	constexpr unsigned int kInactiveBackdrop = 1; /* true */
	constexpr unsigned int kDurationDefault = 9; /* 3s */
	constexpr unsigned int kPositionXDefault = 5; /* center */
	constexpr unsigned int kPositionYDefault = 5; /* center */
	constexpr float kDurationDefaultFloat = 3.f; /* 3s */
	constexpr float kPositionXDefaultFloat = 0.f; /* center */
	constexpr float kPositionYDefaultFloat = 0.f; /* center */
	struct desktop_t final {
#pragma pack(4)
		theme_t theme : 3 { kNotificationDefault }; // theme
		int : 3;
		inactive_backdrop_t inactiveBackdrop : 2 { kInactiveBackdropDefault };
		corner_t corner : 2 { kCornerDefault };  // [Windows 11] square corner
		unsigned int duration : 6 { kDurationDefault };   // 0 → parent, 1 → 0.25, 2 → 0.75, 3-63 → 0.0, 0.5, ..., 30.0 [s]

		position_mode_t positionMode : 2 { kPositionModeDefault };
		unsigned int positionX : 3 { kPositionXDefault }; // 0 → parent, 1, 2 → reserved, 3-7 → 0.0～1.0
		unsigned int positionY : 3 { kPositionYDefault }; // 0 → parent, 1, 2 → reserved, 3-7 → 0.0～1.0

		int : 8;
#pragma pack()
	};

	constexpr float actualDuration(unsigned int packedDuration, float parentDuration = 3.0f) noexcept {
		if (packedDuration >= 3) {
			return 0.5f * static_cast<float>(packedDuration - 3);
		} else {
			switch (packedDuration) {
			case 1: return 0.25f;
			case 2: return 0.75f;
			default: return parentDuration;
			}
		}
	}

	constexpr unsigned int packedDuration(float actualDuration) noexcept {
		if (0.125 < actualDuration && actualDuration < 0.375) {
			return 1;
		} else if (0.625 < actualDuration && actualDuration < 0.875) {
			return 2;
		} else {
			return std::min(63u, static_cast<unsigned int>(std::lround(2.0f * actualDuration)) + 3u);
		}
	}

	constexpr float actualPosition(unsigned int packedPosition, float parentPosition = 0) noexcept {
		if (packedPosition >= 3) {
			return 0.5f * static_cast<float>(packedPosition - 3) - 1.f;
		} else {
			return parentPosition;
		}
	}

	constexpr unsigned int packedPosition(float actualPosition) noexcept {
		return std::min(7u, static_cast<unsigned int>(std::lround(2.f * (1.f + actualPosition))) + 3u);
	}

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

	struct config_t final {
		notification_mode_t mode { kNotificationDefault };
		desktop_t defaultDesktop;
		std::vector<override_desktop_t> desktops;
	};

	class IConfigManager {
	public:
		virtual ~IConfigManager() = default;

		virtual void Reset() = 0;
		virtual config_t Load() = 0;
		virtual void Store(config_t config) = 0;
		virtual void Store(desktop_t config) = 0;
	};

}
