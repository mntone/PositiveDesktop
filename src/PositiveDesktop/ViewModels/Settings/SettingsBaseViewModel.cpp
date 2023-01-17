#include "pch.h"
#include "SettingsBaseViewModel.h"
#if __has_include("ViewModels/Settings/SettingsBaseViewModel.g.cpp")
#include "ViewModels/Settings/SettingsBaseViewModel.g.cpp"
#endif

#include <ppltasks.h>
#include <wil/cppwinrt.h>
#include <wil/cppwinrt_helpers.h>
#include "UI/Helpers/UIHelper.h"

namespace winrt {
	using namespace ::winrt::Windows::Foundation;

	using namespace ::winrt::Microsoft::UI::Dispatching;
	using namespace ::winrt::Microsoft::UI::Xaml::Data;

	using namespace ::winrt::PositiveDesktop::ViewModels::Settings;
}

using namespace winrt::PositiveDesktop::ViewModels::Settings::implementation;


winrt::IAsyncOperation<winrt::SettingsSavedStatus> SettingsBaseViewModel::SaveAsync() {
	using namespace std::chrono_literals;
	co_await resume_background();
	SettingsSavedStatus status { SaveCore() };
	co_await wil::resume_foreground(UI::Helpers::implementation::gDispatchQueue, DispatcherQueuePriority::Low);
	co_return status;
}

void SettingsBaseViewModel::RaisePropertyChanged(param::hstring const& propertyName) {
	propertyChanged_(*this, PropertyChangedEventArgs { propertyName });

	SaveAsync();
}
