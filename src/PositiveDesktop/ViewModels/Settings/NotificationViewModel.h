#pragma once
#include "ViewModels/Settings/NotificationViewModel.g.h"

#include "SettingsBaseViewModel.h"

#include "Services/Storages/config_t.h"

namespace winrt::PositiveDesktop::ViewModels::Settings::implementation {

	struct NotificationViewModel: NotificationViewModelT<NotificationViewModel, SettingsBaseViewModel> {
		NotificationViewModel() noexcept;
		NotificationViewModel(app::storage::desktop_t config) noexcept;

		void Sync(app::storage::desktop_t config) noexcept;

		SettingsSavedStatus SaveCore() override final;

	public:  // - Properties
		constexpr NotificationTheme Theme() const noexcept {
			return theme_;
		}
		void Theme(NotificationTheme value) noexcept;

		inline Windows::Foundation::IReference<bool> InactiveBackdrop() const noexcept {
			return inactiveBackdrop_;
		}
		void InactiveBackdrop(Windows::Foundation::IReference<bool> const& value) noexcept;

		constexpr NotificationCorner Corner() const noexcept {
			return corner_;
		}
		void Corner(NotificationCorner value) noexcept;

		inline bool UseParentDuration() const noexcept {
			return useParentDuration_;
		}
		void UseParentDuration(bool value) noexcept;

		constexpr float Duration() const noexcept {
			return duration_;
		}
		void Duration(float value) noexcept;

		constexpr NotificationPositionOrigin PositionOrigin() const noexcept {
			return positionOrigin_;
		}
		void PositionOrigin(NotificationPositionOrigin value) noexcept;

		constexpr float PositionX() const noexcept {
			return positionX_;
		}
		void PositionX(float value) noexcept;

		constexpr float PositionY() const noexcept {
			return positionY_;
		}
		void PositionY(float value) noexcept;

	private:
		NotificationTheme theme_;
		Windows::Foundation::IReference<bool> inactiveBackdrop_;
		NotificationCorner corner_;
		bool useParentDuration_;
		float duration_;
		NotificationPositionOrigin positionOrigin_;
		float positionX_, positionY_;

	};

}
