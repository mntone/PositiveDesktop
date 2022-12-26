#pragma once

namespace app::ui {

	namespace __impl {

		template<typename T>
		struct register_data_t final {
			static_assert(true, "This class has to be implemented for desired type");
		};
		template<> struct register_data_t<DWORD> final { static constexpr DWORD flags = RRF_RT_REG_DWORD; };
		template<> struct register_data_t<unsigned long long int> final { static constexpr DWORD flags = RRF_RT_REG_QWORD; };

		struct basic_app_uses_light_theme_t final {
			using value_t = DWORD;
			static constexpr HKEY target { HKEY_CURRENT_USER };
			static constexpr wchar_t const* subKey { L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize" };
			static constexpr wchar_t const* value { L"AppUsesLightTheme" };
		};

		struct basic_system_uses_light_theme_t final {
			using value_t = DWORD;
			static constexpr HKEY target { HKEY_CURRENT_USER };
			static constexpr wchar_t const* subKey { L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize" };
			static constexpr wchar_t const* value { L"SystemUsesLightTheme" };
		};

		struct basic_color_prevalence_t final {
			using value_t = DWORD;
			static constexpr HKEY target { HKEY_CURRENT_USER };
			static constexpr wchar_t const* subKey { L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize" };
			static constexpr wchar_t const* value { L"ColorPrevalence" };
		};

		struct basic_enable_transparency_t final {
			using value_t = DWORD;
			static constexpr HKEY target { HKEY_CURRENT_USER };
			static constexpr wchar_t const* subKey { L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize" };
			static constexpr wchar_t const* value { L"EnableTransparency" };
		};

	}

	template<class Key>
	struct register_value_t final {
	public:
		using value_type = Key::value_t;

		void reset() noexcept {
			cache_.reset();
		}

		void update() {
			value_type value;
			DWORD size = sizeof(value_type);
			winrt::check_win32(RegGetValueW(
				Key::target,
				Key::subKey,
				Key::value,
				__impl::register_data_t<value_type>::flags,
				nullptr,
				&value,
				&size));
			WINRT_ASSERT(sizeof(value_type) == size);
			cache_ = value;
		}

		value_type value() {
			if (!cache_) {
				update();
			}
			return cache_.value();
		}

	private:
		std::optional<value_type> cache_;
	};

	using app_uses_light_theme_t = register_value_t<__impl::basic_app_uses_light_theme_t>;
	using system_uses_light_theme_t = register_value_t<__impl::basic_system_uses_light_theme_t>;
	using color_prevalence_t = register_value_t<__impl::basic_color_prevalence_t>;
	using enable_transparency_t = register_value_t<__impl::basic_enable_transparency_t>;
}
