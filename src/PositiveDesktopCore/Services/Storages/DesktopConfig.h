#pragma once
#include "config_t.h"
#include "Common/Reps.h"

namespace app::storage {

	class DesktopConfig: public reps::subject_t<desktop_update_t> {
	public:
		DesktopConfig(IConfigManager* repository);

		void store() const;

	public:  // - Properties
		constexpr default_desktop_t& raw() noexcept { return raw_; }
		constexpr default_desktop_t const& raw() const noexcept { return raw_; }

		constexpr theme_t theme() const noexcept { return raw_.desktop.theme; }
		bool theme(theme_t value) noexcept;

		constexpr backdrop_t backdrop() const noexcept { return raw_.desktop.backdrop; }
		bool backdrop(backdrop_t value) noexcept;

		constexpr inactive_backdrop_t inactiveBackdrop() const noexcept { return raw_.desktop.inactiveBackdrop; }
		bool inactiveBackdrop(inactive_backdrop_t value) noexcept;

		constexpr corner_t corner() const noexcept { return raw_.desktop.corner; }
		bool corner(corner_t value) noexcept;

		float duration() const noexcept;
		bool duration(float value) noexcept;

		constexpr position_origin_t positionOrigin() const noexcept { return raw_.desktop.positionOrigin; }
		bool positionOrigin(position_origin_t value) noexcept;

		float positionX() const noexcept;
		bool positionX(float value) noexcept;

		float positionY() const noexcept;
		bool positionY(float value) noexcept;

		constexpr scale_t scale() const noexcept { return raw_.desktop.scale; }
		bool scale(scale_t value) noexcept;

	private:
		IConfigManager* repository_;
		default_desktop_t raw_;
	};

}
