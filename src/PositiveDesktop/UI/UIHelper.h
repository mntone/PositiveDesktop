#pragma once

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
