#pragma once
#include "ViewModels/Settings/SettingsBaseViewModel.g.h"

namespace winrt::PositiveDesktop::ViewModels::Settings::implementation {

	struct SettingsBaseViewModel: SettingsBaseViewModelT<SettingsBaseViewModel> {
		SettingsBaseViewModel() noexcept = default;

		SettingsSavedStatus Save();

	protected:
		winrt::fire_and_forget RaisePropertyChanged(param::hstring const& propertyName);

		virtual SettingsSavedStatus SaveCore() = 0;

	public:  // - Properties
		inline event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value) {
			return propertyChanged_.add(value);
		}
		void PropertyChanged(event_token const& token) {
			propertyChanged_.remove(token);
		}

		inline event_token Saved(SettingsSavedHandler const& value) {
			return saved_.add(value);
		}
		void Saved(event_token const& token) {
			saved_.remove(token);
		}

	private:
		event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> propertyChanged_;
		event<SettingsSavedHandler> saved_;
		cancellable_promise promise_;
	};

}
