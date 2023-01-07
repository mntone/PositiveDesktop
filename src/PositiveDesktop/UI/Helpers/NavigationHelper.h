#pragma once
#include "UI/Helpers/NavigationHelper.g.h"

namespace winrt::PositiveDesktop::UI::Helpers::implementation {

	struct NavigationHelper: NavigationHelperT<NavigationHelper> {
		static inline winrt::Windows::UI::Xaml::Interop::TypeName GetPageType(winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem const& item) {
			return winrt::unbox_value<winrt::Windows::UI::Xaml::Interop::TypeName>(item.GetValue(PageTypeProperty_));
		}

		static inline void SetPageType(
			winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem const& item,
			winrt::Windows::UI::Xaml::Interop::TypeName const& value) {
			item.SetValue(PageTypeProperty_, winrt::box_value(value));
		}

		static winrt::Microsoft::UI::Xaml::DependencyProperty PageTypeProperty() noexcept { return PageTypeProperty_; }

	private:
		static winrt::Microsoft::UI::Xaml::DependencyProperty PageTypeProperty_;
	};

}

namespace winrt::PositiveDesktop::UI::Helpers::factory_implementation {

	struct NavigationHelper: NavigationHelperT<NavigationHelper, implementation::NavigationHelper> {
	};

}
