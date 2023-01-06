#pragma once
#include <winrt/Windows.Globalization.DateTimeFormatting.h>

#include "UI/Converters/DateTimeToStringConverter.g.h"

namespace winrt::PositiveDesktop::UI::Converters::implementation {

	struct DateTimeToStringConverter: DateTimeToStringConverterT<DateTimeToStringConverter> {
		DateTimeToStringConverter() noexcept;

		winrt::Windows::Foundation::IInspectable Convert(winrt::Windows::Foundation::IInspectable const& value, winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, winrt::Windows::Foundation::IInspectable const& parameter, hstring const& language);
		winrt::Windows::Foundation::IInspectable ConvertBack(winrt::Windows::Foundation::IInspectable const& value, winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, winrt::Windows::Foundation::IInspectable const& parameter, hstring const& language);

	private:
		winrt::Windows::Globalization::DateTimeFormatting::DateTimeFormatter formatter_;
	};

}

namespace winrt::PositiveDesktop::UI::Converters::factory_implementation {

	struct DateTimeToStringConverter: DateTimeToStringConverterT<DateTimeToStringConverter, implementation::DateTimeToStringConverter> {
	};

}
