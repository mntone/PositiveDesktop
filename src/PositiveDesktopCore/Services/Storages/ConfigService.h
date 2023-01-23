#pragma once
#include "config_t.h"
#include "DesktopConfig.h"

namespace app::storage {

	class ConfigService final {
	public:
		ConfigService(IConfigManager* repository) noexcept;

		std::shared_ptr<DesktopConfig> desktop() const noexcept { return desktopConfig_; }

	private:
		IConfigManager* repository_;
		std::shared_ptr<DesktopConfig> desktopConfig_;
	};

}
