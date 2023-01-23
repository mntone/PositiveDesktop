#pragma once
#include <winrt/impl/PositiveDesktop.ViewModels.Settings.0.h>
#include "ViewModels/Settings/NotificationViewModel.g.h"

#include "SettingsBaseViewModel.h"

#include "Common/VersionHelper.h"
#include "Services/Storages/DesktopConfig.h"

namespace winrt::PositiveDesktop::ViewModels::Settings::implementation {

	struct NotificationViewModel: NotificationViewModelT<NotificationViewModel, SettingsBaseViewModel> {
		NotificationViewModel(std::shared_ptr<app::storage::DesktopConfig> config) noexcept;

		SettingsSavedStatus SaveCore() override final;

	public:  // - Properties
		constexpr bool IsWindows11() const noexcept {
			return app::VersionHelper::isWindows11();
		}

		constexpr int ThemeIndex() const noexcept {
			return static_cast<int>(config_->theme()) - 1;
		}
		void ThemeIndex(int value) noexcept;

		constexpr NotificationBackdrop Backdrop() const noexcept {
			return static_cast<NotificationBackdrop>(config_->backdrop());
		}
		void Backdrop(NotificationBackdrop value) noexcept;

		Windows::Foundation::IReference<bool> InactiveBackdrop() const noexcept;
		void InactiveBackdrop(Windows::Foundation::IReference<bool> const& value) noexcept;

		constexpr int CornerIndex() const noexcept {
			app::storage::corner_t value { config_->corner() };
			if (!app::VersionHelper::isWindows11() && app::storage::cnr_squared != value) {
				return static_cast<int>(NotificationCorner::Squared) - 1;
			}
			return static_cast<int>(value) - 1;
		}
		void CornerIndex(int value) noexcept;

		inline bool UseParentDuration() const noexcept {
			return false;
		}
		void UseParentDuration(bool value) noexcept;

		inline float Duration() const noexcept {
			return config_->duration();
		}
		void Duration(float value) noexcept;

		constexpr NotificationPositionOrigin PositionOrigin() const noexcept {
			return static_cast<NotificationPositionOrigin>(config_->positionOrigin());
		}
		void PositionOrigin(NotificationPositionOrigin value) noexcept;

		inline float PositionX() const noexcept {
			return config_->positionX();
		}
		void PositionX(float value) noexcept;

		inline float PositionY() const noexcept {
			return config_->positionY();
		}
		void PositionY(float value) noexcept;

	private:
		std::shared_ptr<app::storage::DesktopConfig> config_;
	};

}
