#pragma once

#include "NotificationPresenter.h"
#include "Services/Storages/DesktopConfig.h"

app::ui::INotificationPresenter* CreateWinUI3NotificationPresenter(std::shared_ptr<app::storage::DesktopConfig> config, app::ui::NotificationPresenterHint hint);
