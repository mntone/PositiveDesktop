#pragma once
#include "config_t.h"
#include "Common/Reps.h"

namespace app::storage {

	class KeyConfig: public reps::subject_t<keymap_update_t> {
	public:
		KeyConfig(IConfigManager* repository);

		void reset();
		void load();
		void store() const;

	public:  // - Properties
		constexpr keymaps_t& raw() noexcept { return raw_; }
		constexpr keymaps_t const& raw() const noexcept { return raw_; }

	private:
		IConfigManager* repository_;
		keymaps_t raw_;
	};

}
