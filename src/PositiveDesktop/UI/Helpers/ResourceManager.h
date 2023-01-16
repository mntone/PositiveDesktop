#pragma once
#include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>

namespace winrt::PositiveDesktop::UI::Helpers::implementation {

	enum class R: unsigned char {
		Notification_Caption_VirtualDesktopChanged,
		Notification_Message_DesktopIndex,
		Notification_Message_DesktopIndexAndNumber,
		MaxCount,
	};

	namespace __impl {

		template<R T>
		struct resource_data_t final {
			static_assert(true, "This class has to be implemented for desired type");
		};
		template<> struct resource_data_t<R::Notification_Caption_VirtualDesktopChanged> final {
			static constexpr std::wstring_view resourceName { L"Notification_Caption_VirtualDesktopChanged" };
		};
		template<> struct resource_data_t<R::Notification_Message_DesktopIndex> final {
			static constexpr std::wstring_view resourceName { L"Notification_Message_DesktopIndex" };
		};
		template<> struct resource_data_t<R::Notification_Message_DesktopIndexAndNumber> final {
			static constexpr std::wstring_view resourceName { L"Notification_Message_DesktopIndexAndNumber" };
		};

		struct Formatter final {
			Formatter(): buffer_(63) {
				buffer_.push_back('\0');
			}

			template<class... Args>
			winrt::hstring format(winrt::hstring format, Args... args) {
				wchar_t const* fmtstr = format.c_str();
				int len = _scwprintf(fmtstr, args...);
				WINRT_ASSERT(len != -1);
				buffer_.resize(len + 1);

				int ret = swprintf_s(buffer_.data(), buffer_.size(), fmtstr, args...);
				WINRT_ASSERT(ret != -1);
				return winrt::hstring(buffer_.data(), len);
			}

		private:
			std::vector<wchar_t> buffer_;
		};

	}

	struct ResourceManager final {
		ResourceManager() = delete;
		ResourceManager(std::wstring_view resourceName)
			: resourceManager_()
			, resources_(resourceManager_.MainResourceMap().GetSubtree(resourceName)) {
			cache_.reserve(static_cast<size_t>(R::MaxCount));
		}

		void ChangeLanguage(winrt::param::hstring const& language) {
			cache_.clear();

			winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceContext resourceContext = resourceManager_.CreateResourceContext();
			resourceContext.QualifierValues().Insert(L"Language", language);
		}

		template<R T>
		winrt::hstring Get() noexcept {
			auto itr = cache_.find(T);
			if (itr == cache_.cend()) {
				winrt::hstring resourceValue { resources_.GetValue(__impl::resource_data_t<T>::resourceName).ValueAsString() };
				cache_.emplace(T, resourceValue);
				return resourceValue;
			}
			return itr->second;
		}

		template<R T, class... Args>
		winrt::hstring Get(Args... args) noexcept {
			auto itr = cache_.find(T);
			if (itr == cache_.cend()) {
				winrt::hstring resourceValue { resources_.GetValue(__impl::resource_data_t<T>::resourceName).ValueAsString() };
				cache_.emplace(T, resourceValue);

				winrt::hstring formattedValue { formatter_.format(resourceValue, args...) };
				return formattedValue;
			}

			winrt::hstring formattedValue { formatter_.format(itr->second, args...) };
			return formattedValue;
		}

	private:
		winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceManager resourceManager_;
		winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceMap resources_;
		std::unordered_map<R, winrt::hstring> cache_;
		__impl::Formatter formatter_;
	};

}
