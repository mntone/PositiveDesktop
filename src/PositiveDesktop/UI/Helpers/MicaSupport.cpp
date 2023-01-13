#include "pch.h"
#include "MicaSupport.h"

#include <winrt/Windows.UI.ViewManagement.h>

#include "DispatcherQueueSupport.h"
#include "ThemeHelper.h"
#include "UI/UIHelper.h"

namespace resources {
	constexpr std::wstring_view WindowBackgroundFallbackColorBrush = L"WindowBackgroundFallbackColorBrush";
}

namespace winrt {
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::System;
	using namespace ::winrt::Windows::UI::ViewManagement;

	using namespace ::winrt::Microsoft::UI::Composition;
	using namespace ::winrt::Microsoft::UI::Composition::SystemBackdrops;
	using namespace ::winrt::Microsoft::UI::Xaml;
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;
}

using namespace app::ui::helper;

MicaSupport::MicaSupport() noexcept
	: backdropConfiguration_(nullptr)
	, backdropController_(nullptr)
	, activatedRevoker_()
	, actualThemeChangedRevoker_() {
	DispatcherQueueSupport::ensureDispatcherQueueController();
}

MicaSupport::~MicaSupport() {
	close();
}

bool MicaSupport::trysetSystemBackdrop(winrt::Window const& window) {
	if (winrt::AccessibilitySettings().HighContrast()
		|| !enableTransparency_.value()
		|| !winrt::MicaController::IsSupported()) {
		close();
		backdropController_ = nullptr;
		backdropConfiguration_ = nullptr;

		winrt::Panel content { window.Content().try_as<winrt::Panel>() };
		if (content) {
			content.Background(getBrush(content.Resources(), resources::WindowBackgroundFallbackColorBrush));
		}
		return false;
	}
	if (nullptr != backdropController_) {
		return true;
	}

	winrt::SystemBackdropConfiguration configuration { winrt::SystemBackdropConfiguration() };
	configuration.IsInputActive(true);
	backdropConfiguration_ = configuration;

	activatedRevoker_ = window.Activated(winrt::auto_revoke, { this, &MicaSupport::onWindowActivated });

	winrt::FrameworkElement rootElement { window.Content().as<winrt::FrameworkElement>() };
	actualThemeChangedRevoker_ = rootElement.ActualThemeChanged(winrt::auto_revoke, { this, &MicaSupport::onContentThemeChanged });

	winrt::MicaController controller { winrt::MicaController() };
	controller.SetSystemBackdropConfiguration(configuration);
	controller.AddSystemBackdropTarget(window.as<winrt::ICompositionSupportsSystemBackdrop>());
	backdropController_ = controller;

	winrt::Panel content { window.Content().try_as<winrt::Panel>() };
	if (content) {
		content.Background(nullptr);
	}
	return true;
}

void MicaSupport::onSystemSettingsChanged(winrt::Window const& window) {
	enableTransparency_.reset(); // Force update
	trysetSystemBackdrop(window);
}

void MicaSupport::close() {
	actualThemeChangedRevoker_.revoke();
	activatedRevoker_.revoke();
}

void MicaSupport::onWindowActivated(winrt::IInspectable const& /*sender*/, winrt::WindowActivatedEventArgs const& args) const {
	backdropConfiguration_.IsInputActive(winrt::WindowActivationState::Deactivated != args.WindowActivationState());
}

void MicaSupport::onContentThemeChanged(winrt::FrameworkElement const& sender, winrt::IInspectable const& /*args*/) const {
	winrt::SystemBackdropTheme theme { ConvertToSystemBackdropTheme(sender.ActualTheme()) };
	backdropConfiguration_.Theme(theme);
}
