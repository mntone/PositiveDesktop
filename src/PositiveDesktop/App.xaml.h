#pragma once
#include <winrt/Microsoft.UI.Xaml.Controls.AnimatedVisuals.h>

#include "UI/Controls/SettingsCard.h"
#include "UI/Controls/SettingsExpander.h"
#include "UI/Converters/DateTimeToStringConverter.h"
#include "UI/Helpers/NavigationHelper.h"

#include "App.xaml.g.h"

#include "Services/app_t.h"

namespace winrt::PositiveDesktop::implementation {

	struct App: AppT<App> {
		App();

		void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
		void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);

	private:
		bool initialized_ { false };
		app::app_t app_;
		winrt::Microsoft::UI::Xaml::Window window_ { nullptr };
	};

}
