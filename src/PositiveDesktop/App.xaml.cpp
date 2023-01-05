#include "pch.h"
#include "App.xaml.h"

namespace app::ui {
	winrt::Microsoft::UI::Dispatching::DispatcherQueue gDispatchQueue = nullptr;
}

using namespace winrt;

using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace Windows::Foundation;

using namespace PositiveDesktop::implementation;

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App() {
	// Init Logger
	app::logger::initLogger();

	// Save DispatcherQueue
	app::ui::gDispatchQueue = winrt::Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread();
	if (!app::ui::gDispatchQueue) {
		Exit();
		return;
	}

	InitializeComponent();

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
	UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e) {
		if (IsDebuggerPresent()) {
			auto errorMessage = e.Message();
			__debugbreak();
		}
	});
#endif
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used such as when the application is launched to open a specific file.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs const&) {
	if (initialized_) {
		return;
	}
	initialized_ = true;

	// Init app
	try {
		app_.initialize();
	} catch (winrt::hresult_error const& /*err*/) {
		Exit(); // Note: Not working because of Windows App SDK issue.
	}
}
