#pragma once
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.h>

namespace winrt::PositiveDesktop::UI::Helpers::implementation {

	extern winrt::Microsoft::UI::Dispatching::DispatcherQueue gDispatchQueue;

	inline void EnsureUIThread() noexcept {
		WINRT_ASSERT(gDispatchQueue.HasThreadAccess());
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

	template<typename TFunction>
	inline bool Dispatch(TFunction callback,
		winrt::Microsoft::UI::Dispatching::DispatcherQueuePriority priority = winrt::Microsoft::UI::Dispatching::DispatcherQueuePriority::Normal) {
		bool isQueued = false;
		if (gDispatchQueue.HasThreadAccess()) {
			callback();
		} else {
			isQueued = gDispatchQueue.TryEnqueue(priority, callback);
		}
		return isQueued;
	}

	inline winrt::Windows::UI::Color GetColor(winrt::Microsoft::UI::Xaml::ResourceDictionary dict, winrt::param::hstring const& key) {
		winrt::Windows::Foundation::IInspectable iColor { dict.Lookup(winrt::box_value(key)) };
		return winrt::unbox_value<winrt::Windows::UI::Color>(iColor);
	}

	inline double GetDouble(winrt::Microsoft::UI::Xaml::ResourceDictionary dict, winrt::param::hstring const& key) {
		winrt::Windows::Foundation::IInspectable iSingle { dict.Lookup(winrt::box_value(key)) };
		return winrt::unbox_value<double>(iSingle);
	}

	inline winrt::Microsoft::UI::Xaml::Media::Brush GetBrush(winrt::Microsoft::UI::Xaml::ResourceDictionary dict, winrt::param::hstring const& key) {
		winrt::Windows::Foundation::IInspectable iBrush { dict.Lookup(winrt::box_value(key)) };
		return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Media::Brush>(iBrush);
	}

}
