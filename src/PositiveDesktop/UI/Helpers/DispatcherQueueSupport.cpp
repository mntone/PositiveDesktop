#include "pch.h"
#include "DispatcherQueueSupport.h"

#include <DispatcherQueue.h>

namespace winrt {
	using namespace ::winrt::Windows::Foundation;
}

using namespace winrt::PositiveDesktop::UI::Helpers::implementation;

winrt::IInspectable DispatcherQueueSupport::dispatcherQueueController_ { nullptr };

void DispatcherQueueSupport::EnsureDispatcherQueueController() {
	if (nullptr != dispatcherQueueController_) return;

	DispatcherQueueOptions options {
		sizeof(DispatcherQueueOptions),
		DQTYPE_THREAD_CURRENT,
		DQTAT_COM_NONE,
	};

	ABI::Windows::System::IDispatcherQueueController* dispatchQueueController = nullptr;
	winrt::check_hresult(CreateDispatcherQueueController(options, &dispatchQueueController));
	dispatcherQueueController_ = { dispatchQueueController, winrt::take_ownership_from_abi };
}
