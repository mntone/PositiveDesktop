#pragma once

#include "NotificationPresenter.h"
#include "Services/Storages/config_t.h"

app::ui::INotificationPresenter* CreateWinUI3NotificationPresenter(app::storage::config_t const& config, app::ui::NotificationPresenterHint hint);
