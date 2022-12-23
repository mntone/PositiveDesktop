#pragma once

namespace app::UI {

	extern winrt::Microsoft::UI::Dispatching::DispatcherQueue gDispatchQueue;

	inline winrt::Windows::System::DispatcherQueueController CreateSystemDispatcherQueueController() {
		DispatcherQueueOptions options {
			sizeof(DispatcherQueueOptions),
			DQTYPE_THREAD_CURRENT,
			DQTAT_COM_NONE,
		};

		ABI::Windows::System::IDispatcherQueueController* dispatchQueueController = nullptr;
		winrt::check_hresult(CreateDispatcherQueueController(options, &dispatchQueueController));
		return { dispatchQueueController, winrt::take_ownership_from_abi };
	}

	template<typename TFunction>
	inline bool Dispatch(winrt::Microsoft::UI::Dispatching::DispatcherQueue dispatcherQueue,
						 TFunction callback,
						 winrt::Microsoft::UI::Dispatching::DispatcherQueuePriority priority = winrt::Microsoft::UI::Dispatching::DispatcherQueuePriority::Normal) {
		bool isQueued = false;
		if (dispatcherQueue.HasThreadAccess()) {
			callback();
		} else {
			isQueued = dispatcherQueue.TryEnqueue(priority, callback);
		}
		return isQueued;
	}

}
