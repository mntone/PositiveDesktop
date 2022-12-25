#pragma once

#include "NotificationPresenter.h"
#include "Services/Storages/config_t.h"

app::UI::INotificationPresenter* CreateWinUI3NotificationPresenter(app::storage::config_t const& config, app::UI::NotificationPresenterHint hint);
