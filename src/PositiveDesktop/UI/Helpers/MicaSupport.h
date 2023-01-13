#pragma once
#include "UI/register_value_t.h"

#include <winrt/Windows.Foundation.h>

#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Composition.SystemBackdrops.h>

namespace app::ui::helper {

	class MicaSupport {
	public:
		MicaSupport() noexcept;
		~MicaSupport();

		bool trysetSystemBackdrop(winrt::Microsoft::UI::Xaml::Window const& window);
		void close();

		void onSystemSettingsChanged(winrt::Microsoft::UI::Xaml::Window const& window);

	private:
		void onWindowActivated(
			winrt::Windows::Foundation::IInspectable const& sender,
			winrt::Microsoft::UI::Xaml::WindowActivatedEventArgs const& args) const;
		void onContentThemeChanged(
			winrt::Microsoft::UI::Xaml::FrameworkElement const& sender,
			winrt::Windows::Foundation::IInspectable const& /*args*/) const;

	private:
		app::ui::enable_transparency_t enableTransparency_;

		winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropConfiguration backdropConfiguration_;
		winrt::Microsoft::UI::Composition::SystemBackdrops::MicaController backdropController_;

		winrt::Microsoft::UI::Xaml::Window::Activated_revoker activatedRevoker_;
		winrt::Microsoft::UI::Xaml::FrameworkElement::ActualThemeChanged_revoker actualThemeChangedRevoker_;
	};

}
