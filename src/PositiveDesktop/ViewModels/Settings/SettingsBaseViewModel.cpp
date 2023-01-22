#include "pch.h"
#include "SettingsBaseViewModel.h"
#if __has_include("ViewModels/Settings/SettingsBaseViewModel.g.cpp")
#include "ViewModels/Settings/SettingsBaseViewModel.g.cpp"
#endif

#include <wil/cppwinrt_helpers.h>

#include "UI/Helpers/UIHelper.h"
#include "SettingsSavedEventArgs.h"

namespace nonlocalized {
	constexpr std::string_view TraceMessage_Save { "Saved." };

	constexpr std::string_view ErrorMessage_Save { "Failed to save." };
}

namespace winrt {
	using namespace ::winrt::Windows::Foundation;

	using namespace ::winrt::Microsoft::UI::Dispatching;
	using namespace ::winrt::Microsoft::UI::Xaml::Data;

	using namespace ::winrt::PositiveDesktop::ViewModels::Settings;
}

using namespace winrt::PositiveDesktop::ViewModels::Settings::implementation;

winrt::SettingsSavedStatus SettingsBaseViewModel::Save() {
	SettingsSavedStatus status { SaveCore() };
	return status;
}

winrt::fire_and_forget SettingsBaseViewModel::RaisePropertyChanged(param::hstring const& propertyName) {
	LOG_TAG(app::logger::ltg_presenter);

	propertyChanged_(*this, PropertyChangedEventArgs { propertyName });

	auto that { get_strong() };
	promise_.cancel();
	promise_.revoke_canceller();

	auto await { resume_after(std::chrono::seconds(3)) };
	await.enable_cancellation(&promise_);
	try {
		co_await await;

		SettingsSavedStatus status { that->Save() };
		if (SettingsSavedStatus::Succeeded == status) {
			LOG_TRACE(app::logger::lop_info, nonlocalized::TraceMessage_Save);
		} else {
			LOG_ERROR(nonlocalized::ErrorMessage_Save, E_FAIL);
		}

		if (that->saved_) {
			co_await wil::resume_foreground(UI::Helpers::implementation::gDispatchQueue, DispatcherQueuePriority::Low);
			that->saved_(*this, make<SettingsSavedEventArgs>(status));
		}
	} catch (hresult_canceled const&) { }
}
