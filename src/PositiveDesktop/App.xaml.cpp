#include "pch.h"
#include "App.xaml.h"

namespace app::UI {
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
	// Save DispatcherQueue
	app::UI::gDispatchQueue = winrt::Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread();
	if (!app::UI::gDispatchQueue) {
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
	if (serviceBag_) {
		return;
	}

	// Init services.
	auto serviceManager = new struct ServiceManager();
	if (serviceManager->Initialize()) {
		delete serviceManager;
		Exit(); // Note: Not working because of Windows App SDK issue.
	} else {
		serviceBag_.reset(serviceManager);
	}
}
