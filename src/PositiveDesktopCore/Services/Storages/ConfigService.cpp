#include "pch.h"
#include "ConfigService.h"

using namespace app::storage;

ConfigService::ConfigService(IConfigManager* repository) noexcept
	: repository_(std::move(repository))
	, desktopConfig_(std::make_shared<DesktopConfig>(repository_))
	, keyConfig_(std::make_shared<KeyConfig>(repository_)) {
}
