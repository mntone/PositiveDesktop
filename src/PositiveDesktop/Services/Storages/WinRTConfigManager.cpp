#include "pch.h"
#include "WinRTConfigManager.h"

#include "Services/Storages/config_interop.h"
#include "Services/Loggers/log_t.h"

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
				load(values, L"backdrop", kBackdropDefault),
				load(values, L"inactiveBackdrop", kInactiveBackdropDefault),
				load(values, L"corner", kCornerDefault),
				packedDuration(load(values, L"duration", kDurationDefaultFloat)),
				load(values, L"positionOrigin", kPositionOriginDefault),
				packedPosition(load(values, L"positionX", kPositionXDefaultFloat)),
				packedPosition(load(values, L"positionY", kPositionYDefaultFloat)),
				packedScale(load(values, L"scale", kScaleDefaultFloat)),
			};
			return desktop;
		}

		template<>
		default_desktop_t load(winrt::Windows::Foundation::Collections::IPropertySet values) {
			default_desktop_t defaultDesktop {
				load<desktop_t>(values),
			};
			return defaultDesktop;
		}

		template<typename T, std::enable_if_t<std::is_same_v<T, default_desktop_t>, std::nullptr_t> = nullptr>
		default_desktop_t load(winrt::Windows::Storage::ApplicationDataContainer container) {
			winrt::Windows::Foundation::Collections::IPropertySet values { container.Values() };
			return load<default_desktop_t>(values);
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

		keymap_t load(winrt::Windows::Foundation::Collections::IPropertySet values, app::keylistener::kbevent_t ev) {
			keymap_t keymap {
				ev,
				load(values, L"key1", kKeyDefault),
				load(values, L"key2", kKeyDefault),
			};
			return keymap;
		}

		template<typename T, std::enable_if_t<std::is_same_v<T, keymaps_t>, std::nullptr_t> = nullptr>
		keymaps_t load(winrt::Windows::Storage::ApplicationDataContainer container) {
			winrt::Windows::Foundation::Collections::IPropertySet values { container.Values() };
			keymaps_t config {
				load(values, L"separate", kSeparateDefault),
			};

			winrt::Windows::Storage::ApplicationDataContainer keymapsContainer { getContainer<winrt::Windows::Storage::ApplicationDataCreateDisposition::Existing>(container, L"keymaps") };
			if (keymapsContainer) {
				winrt::Windows::Foundation::Collections::IPropertySet keymapsValues { keymapsContainer.Values() };
				if (keymapsValues) {
					winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable> keymaps = keymapsValues.GetView();
					uint32_t const size = keymaps.Size();
					config.keymaps.reserve(size);
					for (winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::Windows::Foundation::IInspectable> pair : keymaps) {
						winrt::hstring evstr { pair.Key() };
						app::keylistener::kbevent_t ev { static_cast<app::keylistener::kbevent_t>(std::wcstoul(evstr.data(), nullptr, 10)) };
						winrt::Windows::Storage::ApplicationDataCompositeValue composite { pair.Value().as<winrt::Windows::Storage::ApplicationDataCompositeValue>() };
						keymap_t keymap { load(composite, ev) };
						config.keymaps.push_back(keymap);
					}
				}
			}
			return config;
		}

		template<typename T, std::enable_if_t<std::is_same_v<T, config_t>, std::nullptr_t> = nullptr>
		config_t load(winrt::Windows::Storage::ApplicationDataContainer container) {
			winrt::Windows::Foundation::Collections::IPropertySet values { container.Values() };
			config_t config {
				load(values, L"mode", kNotificationDefault),
				load<default_desktop_t>(values),
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
		void store(winrt::Windows::Foundation::Collections::IPropertySet values, winrt::param::hstring const& key, T value) {
			values.Insert(key, winrt::box_value(value));
		}

		template<typename T>
		void store(winrt::Windows::Foundation::Collections::IPropertySet values, winrt::param::hstring const& key, T value, T defaultValue) {
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
			store(values, L"backdrop", value.backdrop, kBackdropDefault);
			store(values, L"inactiveBackdrop", value.inactiveBackdrop, kInactiveBackdropDefault);
			store(values, L"corner", value.corner, kCornerDefault);
			store(values, L"duration", actualDuration(value.duration), kDurationDefaultFloat);
			store(values, L"positionOrigin", value.positionOrigin, kPositionOriginDefault);
			store(values, L"positionX", actualPosition(value.positionX), kPositionXDefaultFloat);
			store(values, L"positionY", actualPosition(value.positionY), kPositionYDefaultFloat);
			store(values, L"scale", actualScale(value.scale), kScaleDefaultFloat);
		}

		void store(winrt::Windows::Foundation::Collections::IPropertySet values, default_desktop_t value) {
			store(values, value.desktop);
		}

		void store(winrt::Windows::Storage::ApplicationDataContainer container, default_desktop_t value) {
			winrt::Windows::Foundation::Collections::IPropertySet values { container.Values() };
			store(values, value);
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

		void store(winrt::Windows::Foundation::Collections::IPropertySet values, keymap_t const& value) {
			winrt::Windows::Storage::ApplicationDataCompositeValue composite;
			store(composite, L"key1", value.key1.raw, kKeyDefault);
			store(composite, L"key2", value.key2.raw, kKeyDefault);
			values.Insert(winrt::to_hstring(value.ev), composite);
		}

		void store(winrt::Windows::Storage::ApplicationDataContainer container, keymaps_t const& value) {
			winrt::Windows::Foundation::Collections::IPropertySet values { container.Values() };
			store(values, L"separate", value.separate, kSeparateDefault);

			winrt::Windows::Storage::ApplicationDataContainer keymapsContainer { getContainer(container, L"keymaps") };
			winrt::Windows::Foundation::Collections::IPropertySet keymapsValues { keymapsContainer.Values() };
			for (keymap_t const& keymap : value.keymaps) {
				store(keymapsValues, keymap);
			}
		}

		void store(winrt::Windows::Storage::ApplicationDataContainer container, config_t value) {
			winrt::Windows::Foundation::Collections::IPropertySet values { container.Values() };
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
#if _DEBUG
			LOG_TAG(logger::ltg_storage);

			winrt::hstring path { winrt::Windows::Storage::ApplicationData::Current().LocalFolder().Path() };
			LOG_TRACE(logger::lop_info, std::format("Config file path: {}", winrt::to_string(path)));
#endif
		}

		void Reset() override {
			winrt::Windows::Storage::ApplicationData::Current().ClearAsync().get();
		}

		config_t Load() override {
			return interop::load<config_t>(container_);
		}

		default_desktop_t LoadDefaultDesktop() override {
			return interop::load<default_desktop_t>(container_);
		}

		keymaps_t LoadKeymaps() override {
			return interop::load< keymaps_t>(container_);
		}

		void store(config_t value)  const override {
			interop::store(container_, value);
		}

		void store(default_desktop_t value)  const override {
			interop::store(container_, value);
		}

		void store(keymaps_t const& value) const override {
			interop::store(container_, value);
		}

	private:
		winrt::Windows::Storage::ApplicationDataContainer container_;
	};


	app::storage::IConfigManager* CreateWinRTConfigManager() {
		return new WinRTConfigManager();
	}

}
