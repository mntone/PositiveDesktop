#include "pch.h"
#include "ConfigService.h"

using namespace app::storage;

ConfigService::ConfigService(IConfigManager* repository) noexcept
	: repository_(std::move(repository))
	, desktopConfig_(std::make_shared<DesktopConfig>(repository_)) {
}
