#pragma once
#include "config_t.h"
#include "DesktopConfig.h"
#include "KeyConfig.h"

namespace app::storage {

	class ConfigService final {
	public:
		ConfigService(IConfigManager* repository) noexcept;

		std::shared_ptr<DesktopConfig> desktop() const noexcept { return desktopConfig_; }
		std::shared_ptr<KeyConfig> key() const noexcept { return keyConfig_; }

	private:
		IConfigManager* repository_;
		std::shared_ptr<DesktopConfig> desktopConfig_;
		std::shared_ptr<KeyConfig> keyConfig_;
	};

}
