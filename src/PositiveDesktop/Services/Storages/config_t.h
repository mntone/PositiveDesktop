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
	static notification_mode_t const kNotificationDefault = nfm_default;

	enum theme_t: unsigned int {
		thm_default,
		thm_system,
		thm_light,
		thm_dark,
		thm_accent,
	};
	static theme_t const kThemeDefault = thm_default;

	enum corner_t: unsigned int {
		cnr_default,
		cnr_rounded,
		cnr_rounded_small,
		cnr_squared,
	};
	static corner_t const kCornerDefault = cnr_default;

	static int const kDurationDefault = 9; /* 3s */
	static int const kPositionXDefault = 21; /* center */
	static int const kPositionYDefault = 21; /* center */
	static float const kDurationDefaultFloat = 3.f; /* 3s */
	static float const kPositionXDefaultFloat = 0.5; /* center */
	static float const kPositionYDefaultFloat = 0.5; /* center */
	struct desktop_t final {
#pragma pack(4)
		theme_t theme : 3 { kNotificationDefault }; // theme
		int : 5;
		corner_t corner : 2 { kCornerDefault };  // [Windows 11] square corner
		unsigned int duration : 6 { kDurationDefault };   // 0 → parent, 1 → 0.25, 2 → 0.75, 3-63 → 0.0, 0.5, ..., 30.0 [s]

		// 0 → parent
		// 1 → window to left of the cursor if available (serialize as -Inf)
		// 2 → cursor (serialize as NaN)
		// 3 → window to right of the cursor if available (serialize as +Inf)
		// 4-10 → reserved
		// 11-31 → 0.0～1.0
		unsigned int positionX : 5 { kPositionXDefault };
		int : 3;

		// 0 → parent
		// 1 → window over the cursor if available (serialize as -Inf)
		// 2 → cursor (serialize as NaN)
		// 3 → window under the cursor if available (serialize as +Inf)
		// 4-10 → reserved
		// 11-31 → 0.0～1.0
		unsigned int positionY : 5 { kPositionYDefault };
		int : 3;
#pragma pack()
	};

	inline float actualDuration(unsigned int packedDuration, float parentDuration = 3.0f) noexcept {
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

	inline unsigned int packedDuration(float actualDuration) noexcept {
		if (0.125 < actualDuration && actualDuration < 0.375) {
			return 1;
		} else if (0.625 < actualDuration && actualDuration < 0.875) {
			return 2;
		} else {
			return std::min(63u, static_cast<unsigned int>(std::lround(2.0f * actualDuration)) + 3u);
		}
	}

	inline float actualPosition(unsigned int packedPosition, float parentPosition = 0.5) noexcept {
		if (packedPosition >= 11) {
			return 0.05f * static_cast<float>(packedPosition - 11);
		} else {
			switch (packedPosition) {
			case 1: return -std::numeric_limits<float>::infinity();
			case 2: return std::numeric_limits<float>::quiet_NaN();
			case 3: return std::numeric_limits<float>::infinity();
			default: return parentPosition;
			}
		}
	}

	inline unsigned int packedPosition(float actualPosition) noexcept {
		if (!std::isfinite(actualPosition)) {
			if (std::isinf(actualPosition)) {
				return 3;
			} else if (std::isnan(actualPosition)) {
				return 2;
			} else {
				return 1;
			}
		} else {
			return std::min(31u, static_cast<unsigned int>(std::lround(20.f * actualPosition)) + 11u);
		}
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

	static bool const kEnabledDefault = true;
	static unsigned int const kIndexDefault = 0;
	static wchar_t const* const kNameDefault = L"";
	struct override_desktop_t final {
		winrt::guid uuid;
		bool enabled : 1 { kEnabledDefault };
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
		notification_mode_t mode { nfm_default };
		desktop_t defaultDesktop;
		std::vector<override_desktop_t> desktops;
	};

	class IConfigManager {
	public:
		virtual ~IConfigManager() { }

		virtual void Reset() = 0;
		virtual config_t Load() = 0;
		virtual void Store(config_t config) = 0;
	};

}
