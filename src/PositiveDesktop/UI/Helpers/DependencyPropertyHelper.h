#pragma once

namespace winrt {
	using IInspectable = ::winrt::Windows::Foundation::IInspectable;
	using PropertyChangedCallback = ::winrt::Microsoft::UI::Xaml::PropertyChangedCallback;
	using PropertyMetadata = ::winrt::Microsoft::UI::Xaml::PropertyMetadata;
}

template<typename T, typename Enable = void>
struct ValueHelper final {
	static constexpr ::winrt::IInspectable defaultValue() noexcept {
		return T { };
	}
};

template<typename T>
struct ValueHelper<T, std::enable_if_t<std::is_base_of_v<::winrt::IInspectable, T>>> final {
	static constexpr ::winrt::IInspectable defaultValue() noexcept {
		return ::winrt::IInspectable { nullptr };
	}
};

template<>
struct ValueHelper<::winrt::hstring, void> final {
private:
	static constexpr ::std::wstring_view empty_ { L"" };

public:
	static inline ::winrt::IInspectable defaultValue() noexcept {
		return ::winrt::box_value(::winrt::hstring(empty_));
	}
};

template<>
struct ValueHelper<bool, void> final {
	static inline ::winrt::IInspectable defaultValue() noexcept {
		return ::winrt::box_value<bool>(false);
	}
};

#define DEFINE_PARENT_TYPENAME(__TYPE__) ::winrt::Windows::UI::Xaml::Interop::TypeName const __parentTypeName { ::winrt::xaml_typename<projection::__TYPE__>() }

#ifdef DELAY_INIT_DP

#define DEFINE_DP_METADATA(__PROPNAME__, __TYPE__, __PARENT_TYPE__, __METADATA__) \
	__PROPNAME__ = ::winrt::Microsoft::UI::Xaml::DependencyProperty::Register( \
			L#__PROPNAME__, \
			::winrt::xaml_typename<__TYPE__>(), \
			__parentTypeName, \
			__METADATA__ )

#else

#define DEFINE_DP_METADATA(__PROPNAME__, __TYPE__, __PARENT_TYPE__, __METADATA__) \
	::winrt::Microsoft::UI::Xaml::DependencyProperty __PARENT_TYPE__::__PROPNAME__##Property_ { \
		::winrt::Microsoft::UI::Xaml::DependencyProperty::Register( \
			L#__PROPNAME__, \
			::winrt::xaml_typename<__TYPE__>(), \
			::winrt::xaml_typename<projection::__PARENT_TYPE__>(), \
			__METADATA__ ) \
	}

#endif

#define DEFINE_DP(__PROPNAME__, __TYPE__, __PARENT_TYPE__) \
	DEFINE_DP_METADATA(__PROPNAME__, __TYPE__, __PARENT_TYPE__, ::winrt::PropertyMetadata(ValueHelper<__TYPE__>::defaultValue()))

#define DEFINE_DP_CALLBACK(__PROPNAME__, __TYPE__, __PARENT_TYPE__, __CALLBACK__) \
	DEFINE_DP_METADATA(__PROPNAME__, __TYPE__, __PARENT_TYPE__, \
		::winrt::PropertyMetadata(ValueHelper<__TYPE__>::defaultValue(), ::winrt::PropertyChangedCallback(__CALLBACK__)))
