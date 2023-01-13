#pragma once
#include <winrt/Windows.Foundation.h>

namespace app::ui::helper {

	class DispatcherQueueSupport final {
	public:
		static void ensureDispatcherQueueController();

	private:
		static winrt::Windows::Foundation::IInspectable dispatcherQueueController_;
	};

}
