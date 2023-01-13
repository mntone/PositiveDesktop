#pragma once
#include <winrt/Windows.Globalization.DateTimeFormatting.h>

#include "UI/Converters/DateTimeToStringConverter.g.h"

namespace winrt::PositiveDesktop::UI::Converters::implementation {

	struct DateTimeToStringConverter: DateTimeToStringConverterT<DateTimeToStringConverter> {
		DateTimeToStringConverter() noexcept;

		Windows::Foundation::IInspectable Convert(Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const& targetType, Windows::Foundation::IInspectable const& parameter, hstring const& language);
		Windows::Foundation::IInspectable ConvertBack(Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const& targetType, Windows::Foundation::IInspectable const& parameter, hstring const& language);

		inline hstring Format() const noexcept {
			return format_;
		}
		inline void Format(param::hstring const& value) noexcept {
			formatter_ = nullptr;
			format_ = value;
		}

	private:
		hstring format_;
		Windows::Globalization::DateTimeFormatting::DateTimeFormatter formatter_;
	};

}

namespace winrt::PositiveDesktop::UI::Converters::factory_implementation {

	struct DateTimeToStringConverter: DateTimeToStringConverterT<DateTimeToStringConverter, implementation::DateTimeToStringConverter> {
	};

}
