#include "pch.h"
#include "App.xaml.h"

namespace winrt {
	using namespace ::winrt::Windows::Foundation;

	using namespace ::winrt::Microsoft::UI::Dispatching;
	using namespace ::winrt::Microsoft::UI::Xaml;
}

namespace winrt::PositiveDesktop::UI::Helpers::implementation {
	winrt::DispatcherQueue gDispatchQueue { nullptr };
}

using namespace winrt::PositiveDesktop::implementation;

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App() {
	// Init Logger
	app::logger::initLogger();

	// Save DispatcherQueue
	UI::Helpers::implementation::gDispatchQueue = DispatcherQueue::GetForCurrentThread();
	if (!UI::Helpers::implementation::gDispatchQueue) {
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
	} catch (hresult_error const& /*err*/) {
		Exit(); // Note: Not working because of Windows App SDK issue.
	}
}
