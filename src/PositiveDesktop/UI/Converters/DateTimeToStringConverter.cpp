#include "pch.h"
#include "DateTimeToStringConverter.h"
#if __has_include("UI/Converters/DateTimeToStringConverter.g.cpp")
#include "UI/Converters/DateTimeToStringConverter.g.cpp"
#endif

constexpr std::wstring_view kDateTimeFormat { L"longdate longtime" };

namespace winrt {
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::Globalization::DateTimeFormatting;
	using namespace ::winrt::Windows::UI::Xaml::Interop;
}

using namespace winrt::PositiveDesktop::UI::Converters::implementation;

DateTimeToStringConverter::DateTimeToStringConverter() noexcept:
	formatter_(kDateTimeFormat) {
}

winrt::IInspectable DateTimeToStringConverter::Convert(IInspectable const& value, TypeName const& targetType, IInspectable const& /*parameter*/, hstring const& language) {
	WINRT_ASSERT(xaml_typename<hstring>() == targetType);

	if (!language.empty() && formatter_.Languages().GetAt(0) != language) {
		std::vector<hstring> languages { language };
		formatter_ = { kDateTimeFormat, languages };
	}

	DateTime datetime { unbox_value<DateTime>(value) };
	hstring string { formatter_.Format(datetime) };
	return box_value(string);
}

winrt::IInspectable DateTimeToStringConverter::ConvertBack(IInspectable const& /*value*/, TypeName const& /*targetType*/, IInspectable const& /*parameter*/, hstring const& /*language*/) {
	throw hresult_not_implemented();
}
