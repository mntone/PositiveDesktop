#pragma once
#include <winrt/Windows.Foundation.h>

namespace winrt::PositiveDesktop::UI::Helpers::implementation {

	class DispatcherQueueSupport final {
	public:
		static void EnsureDispatcherQueueController();

	private:
		static winrt::Windows::Foundation::IInspectable dispatcherQueueController_;
	};

}
