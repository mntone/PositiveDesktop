#pragma once

namespace app::ui {

	extern winrt::Microsoft::UI::Dispatching::DispatcherQueue gDispatchQueue;

	inline winrt::Windows::System::DispatcherQueueController createSystemDispatcherQueueController() {
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
	inline bool dispatch(winrt::Microsoft::UI::Dispatching::DispatcherQueue dispatcherQueue,
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
	inline bool dispatch(TFunction callback,
		winrt::Microsoft::UI::Dispatching::DispatcherQueuePriority priority = winrt::Microsoft::UI::Dispatching::DispatcherQueuePriority::Normal) {
		bool isQueued = false;
		if (gDispatchQueue.HasThreadAccess()) {
			callback();
		} else {
			isQueued = gDispatchQueue.TryEnqueue(priority, callback);
		}
		return isQueued;
	}

	inline winrt::Windows::UI::Color getColor(winrt::Microsoft::UI::Xaml::ResourceDictionary dict, winrt::param::hstring const& key) {
		winrt::Windows::Foundation::IInspectable iColor { dict.Lookup(winrt::box_value(key)) };
		return winrt::unbox_value<winrt::Windows::UI::Color>(iColor);
	}

	inline double getDouble(winrt::Microsoft::UI::Xaml::ResourceDictionary dict, winrt::param::hstring const& key) {
		winrt::Windows::Foundation::IInspectable iSingle { dict.Lookup(winrt::box_value(key)) };
		return winrt::unbox_value<double>(iSingle);
	}

	inline winrt::Microsoft::UI::Xaml::Media::Brush getBrush(winrt::Microsoft::UI::Xaml::ResourceDictionary dict, winrt::param::hstring const& key) {
		winrt::Windows::Foundation::IInspectable iBrush { dict.Lookup(winrt::box_value(key)) };
		return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Media::Brush>(iBrush);
	}

}
