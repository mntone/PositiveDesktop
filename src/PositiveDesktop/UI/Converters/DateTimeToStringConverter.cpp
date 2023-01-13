#include "pch.h"
#include "DateTimeToStringConverter.h"
#if __has_include("UI/Converters/DateTimeToStringConverter.g.cpp")
#include "UI/Converters/DateTimeToStringConverter.g.cpp"
#endif

constexpr std::wstring_view kDateTimeFormatDefault { L"longdate longtime" };

namespace winrt {
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::Globalization::DateTimeFormatting;
	using namespace ::winrt::Windows::UI::Xaml::Interop;
}

using namespace winrt::PositiveDesktop::UI::Converters::implementation;

DateTimeToStringConverter::DateTimeToStringConverter() noexcept
	: format_(kDateTimeFormatDefault)
	, formatter_(nullptr) {
}

winrt::IInspectable DateTimeToStringConverter::Convert(IInspectable const& value, TypeName const& targetType, IInspectable const& /*parameter*/, hstring const& language) {
	WINRT_ASSERT(xaml_typename<hstring>() == targetType);

	if (nullptr == formatter_) {
		if (!language.empty()) {
			std::vector<hstring> languages { language };
			formatter_ = { format_, languages };
		} else {
			formatter_ = { format_ };
		}
	} else if (!language.empty() && formatter_.Languages().GetAt(0) != language) {
		std::vector<hstring> languages { language };
		formatter_ = { format_, languages };
	}

	DateTime datetime { unbox_value<DateTime>(value) };
	hstring string { formatter_.Format(datetime) };
	return box_value(string);
}

winrt::IInspectable DateTimeToStringConverter::ConvertBack(IInspectable const& /*value*/, TypeName const& /*targetType*/, IInspectable const& /*parameter*/, hstring const& /*language*/) {
	throw hresult_not_implemented();
}
