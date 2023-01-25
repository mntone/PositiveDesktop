#pragma once
#include <winrt/Microsoft.UI.Xaml.Controls.AnimatedVisuals.h>
#include <winrt/Mntone.AngelUmbrella.Navigation.h>

#include "UI/Controls/KeyInputBox.xaml.h"
#include "UI/Controls/KeyVisual.xaml.h"

#include "App.xaml.g.h"

#include "Services/app_t.h"

namespace winrt::PositiveDesktop::implementation {

	struct App: AppT<App> {
		App();

		void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
		void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);

	public:  // - Properties
		app::app_t& Context() noexcept { return app_; }

	private:
		bool initialized_ { false };
		app::app_t app_;
		winrt::Microsoft::UI::Xaml::Window window_ { nullptr };
	};

}
