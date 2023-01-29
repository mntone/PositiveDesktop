#pragma once
#include "ViewModels/ObservableObject.g.h"

namespace winrt::PositiveDesktop::ViewModels::implementation {

	struct ObservableObject: ObservableObjectT<ObservableObject> {
		ObservableObject() noexcept = default;

	protected:
		inline void RaisePropertyChanged(param::hstring const& propertyName) {
			propertyChanged_(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
		}

		inline void RaisePropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& args) {
			propertyChanged_(*this, args);
		}

	public:  // - Properties
		inline event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& value) {
			return propertyChanged_.add(value);
		}
		void PropertyChanged(event_token const& token) {
			propertyChanged_.remove(token);
		}

	private:
		event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> propertyChanged_;
	};

}
