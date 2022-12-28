#include <utility>
#include <winrt/base.h>

#include "WinRTConfigManager.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.h>

namespace app::storage {

	namespace interop {

		template<winrt::Windows::Storage::ApplicationDataCreateDisposition createFlag = winrt::Windows::Storage::ApplicationDataCreateDisposition::Always>
		winrt::Windows::Storage::ApplicationDataContainer getContainer(
			winrt::Windows::Storage::ApplicationDataContainer root,
			winrt::param::hstring key) {
			if constexpr (winrt::Windows::Storage::ApplicationDataCreateDisposition::Existing == createFlag) {
				winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Storage::ApplicationDataContainer> containers { root.Containers() };
				return containers.TryLookup(key);
			} else {
				return root.CreateContainer(key, createFlag);
			}
		}

#pragma region Load functions

		template<typename T>
		T load(winrt::Windows::Foundation::Collections::IPropertySet values, winrt::param::hstring key) {
			return winrt::unbox_value<T>(values.Lookup(key));
		}

		template<typename T>
		T load(winrt::Windows::Foundation::Collections::IPropertySet values, winrt::param::hstring key, T defaultValue) noexcept {
			if (values.HasKey(key)) {
				if constexpr (std::is_enum_v<T>) {
					using B = std::underlying_type_t<T>;
					return static_cast<T>(winrt::unbox_value_or<B>(values.TryLookup(key), static_cast<B>(defaultValue)));
				} else {
					return winrt::unbox_value_or(values.TryLookup(key), defaultValue);
				}
			} else {
				return defaultValue;
			}
		}

		template<typename T>
		T load(winrt::Windows::Foundation::Collections::IPropertySet values) {
			static_assert(true, "This function has to be implemented for desired type");
		}

		template<>
		desktop_t load(winrt::Windows::Foundation::Collections::IPropertySet values) {
			desktop_t desktop {
				load(values, L"theme", kThemeDefault),
				load(values, L"corner", kCornerDefault),
				packedDuration(load(values, L"duration", kDurationDefaultFloat)),
				load(values, L"positionMode", kPositionModeDefault),
				packedPosition(load(values, L"positionX", kPositionXDefaultFloat)),
				packedPosition(load(values, L"positionY", kPositionYDefaultFloat)),
			};
			return desktop;
		}

		override_desktop_t load(winrt::Windows::Foundation::Collections::IPropertySet values, winrt::guid uuid) {
			override_desktop_t desktop {
				uuid,
				load(values, L"enabled", kEnabledDefault),
				load(values, L"textMode", ttt_default),
				load(values, L"overrideMode", orm_default),
				load<desktop_t>(values),
			};
			switch (desktop.overrideMode) {
			case orm_display_index:
			case orm_desktop_index:
				desktop.index = load(values, L"index", kIndexDefault);
				break;
			case orm_desktop_name:
				winrt::hstring text = load<winrt::hstring>(values, L"text", kNameDefault);
				uint32_t const size = text.size();
				wchar_t* rawText = new wchar_t[size + 1];
				errno_t err = wmemcpy_s(rawText, size, text.c_str(), size);
				if (err) {
					delete[] rawText;
				} else {
					rawText[size] = L'\0';
					desktop.text = rawText;
				}
				break;
			}
			return desktop;
		}

		config_t load(winrt::Windows::Storage::ApplicationDataContainer container) {
			winrt::Windows::Foundation::Collections::IPropertySet values { container.Values() };
			config_t config {
				load(values, L"mode", kNotificationDefault),
				load<desktop_t>(values),
			};

			winrt::Windows::Storage::ApplicationDataContainer desktopsContainer { getContainer<winrt::Windows::Storage::ApplicationDataCreateDisposition::Existing>(container, L"desktops") };
			if (desktopsContainer) {
				winrt::Windows::Foundation::Collections::IPropertySet desktopsValues { desktopsContainer.Values() };
				if (desktopsContainer) {
					winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> desktops = desktopsValues.GetView();
					uint32_t const size = desktops.Size();
					config.desktops.reserve(size);
					for (winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::Windows::Foundation::IInspectable> pair : desktops) {
						winrt::guid uuid { pair.Key() };
						winrt::Windows::Storage::ApplicationDataCompositeValue composite { pair.Value().as<winrt::Windows::Storage::ApplicationDataCompositeValue>() };
						override_desktop_t desktop { load(composite, uuid) };
						config.desktops.push_back(desktop);
					}
				}
			}
			return config;
		}

#pragma endregion

#pragma region Store functions

		template<typename T>
		void store(winrt::Windows::Foundation::Collections::IPropertySet values, winrt::param::hstring key, T value) {
			values.Insert(key, winrt::box_value(value));
		}

		template<typename T>
		void store(winrt::Windows::Foundation::Collections::IPropertySet values, winrt::param::hstring key, T value, T defaultValue) {
			if (value != defaultValue) {
				if constexpr (std::is_enum_v<T>) {
					using B = std::underlying_type_t<T>;
					values.Insert(key, winrt::box_value(static_cast<B>(value)));
				} else {
					values.Insert(key, winrt::box_value(value));
				}
			} else {
				values.TryRemove(key);
			}
		}

		void store(winrt::Windows::Foundation::Collections::IPropertySet values, desktop_t value) {
			store(values, L"theme", value.theme, kThemeDefault);
			store(values, L"corner", value.corner, kCornerDefault);
			store(values, L"duration", actualDuration(value.duration), kDurationDefaultFloat);
			store(values, L"positionMode", value.positionMode, kPositionModeDefault);
			store(values, L"positionX", actualPosition(value.positionX), kPositionXDefaultFloat);
			store(values, L"positionY", actualPosition(value.positionY), kPositionYDefaultFloat);
		}

		void store(winrt::Windows::Foundation::Collections::IPropertySet values, override_desktop_t value) {
			winrt::Windows::Storage::ApplicationDataCompositeValue composite;
			store(composite, L"enabled", value.enabled, kEnabledDefault);
			store(composite, L"textMode", value.textMode, ttt_default);
			store(composite, L"overrideMode", value.overrideMode, orm_default);
			store(composite, value.desktop);
			switch (value.overrideMode) {
			case orm_display_index:
			case orm_desktop_index:
				store(composite, L"index", value.index, kIndexDefault);
				composite.TryRemove(L"text");
				break;
			case orm_desktop_name:
				composite.TryRemove(L"index");
				store<winrt::hstring>(composite, L"text", value.text, kNameDefault);
				break;
			}

			winrt::hstring uuidText = winrt::to_hstring(value.uuid);
			values.Insert(uuidText, composite);
		}

		void store(winrt::Windows::Storage::ApplicationDataContainer container, config_t value) {
			winrt::Windows::Foundation::Collections::IPropertySet values { container.Values() };
			store(values, L"mode", value.mode, kNotificationDefault);
			store(values, value.defaultDesktop);

			winrt::Windows::Storage::ApplicationDataContainer desktopsContainer { getContainer(container, L"desktops") };
			winrt::Windows::Foundation::Collections::IPropertySet desktopsValues { desktopsContainer.Values() };
			for (override_desktop_t desktop : value.desktops) {
				store(desktopsValues, desktop);
			}
		}

#pragma endregion

	}

	class WinRTConfigManager final: public IConfigManager {
	public:
		WinRTConfigManager()
			: container_(winrt::Windows::Storage::ApplicationData::Current().LocalSettings()) {
		}

		~WinRTConfigManager() {
			container_.Close();
		}

		void Reset() override {
			winrt::Windows::Storage::ApplicationData::Current().ClearAsync().get();
		}

		config_t Load() override {
			return interop::load(container_);
		}

		void Store(config_t value) override {
			interop::store(container_, value);
		}

	private:
		winrt::Windows::Storage::ApplicationDataContainer container_;
	};


	app::storage::IConfigManager* CreateWinRTConfigManager() {
		return new WinRTConfigManager();
	}

}
