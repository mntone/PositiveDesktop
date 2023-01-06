#include "pch.h"
#include "DateTimeToStringConverter.h"
#if __has_include("UI/Converters/DateTimeToStringConverter.g.cpp")
#include "UI/Converters/DateTimeToStringConverter.g.cpp"
#endif

#include <winrt/Windows.UI.Xaml.Interop.h>

constexpr std::wstring_view kDateTimeFormat { L"longdate longtime" };

namespace winrt {

	using namespace Windows::Foundation;
	using namespace Windows::Globalization::DateTimeFormatting;
	using namespace Windows::UI::Xaml::Interop;

}

using namespace winrt::PositiveDesktop::UI::Converters::implementation;

DateTimeToStringConverter::DateTimeToStringConverter() noexcept:
	formatter_(kDateTimeFormat) {
}

winrt::IInspectable DateTimeToStringConverter::Convert(winrt::IInspectable const& value, winrt::TypeName const& targetType, winrt::IInspectable const& /*parameter*/, winrt::hstring const& language) {
	WINRT_ASSERT(winrt::xaml_typename<winrt::hstring>() == targetType);

	if (!language.empty() && formatter_.Languages().GetAt(0) != language) {
		std::vector<winrt::hstring> languages { language };
		formatter_ = { kDateTimeFormat, languages };
	}

	winrt::DateTime datetime { winrt::unbox_value<winrt::DateTime>(value) };
	winrt::hstring string { formatter_.Format(datetime) };
	return winrt::box_value(string);
}

winrt::IInspectable DateTimeToStringConverter::ConvertBack(winrt::IInspectable const& /*value*/, winrt::TypeName const& /*targetType*/, winrt::IInspectable const& /*parameter*/, winrt::hstring const& /*language*/) {
	throw winrt::hresult_not_implemented();
}
