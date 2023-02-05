#include "pch.h"
#include "ShortcutKeyViewModel.h"
#if __has_include("ViewModels/Settings/ShortcutKeyViewModel.g.cpp")
#include "ViewModels/Settings/ShortcutKeyViewModel.g.cpp"
#endif

namespace winrt {
	using namespace ::winrt::Windows::Foundation;

	using namespace ::winrt::PositiveDesktop::ViewModels::Settings;
}

using namespace winrt::PositiveDesktop::ViewModels::Settings::implementation;

#define MAP_KEYMAP(__EV, __VAR) \
	case __EV ## _desktop1: \
		get_self<KeyViewModel>(__VAR ## Desktop1_)->Bind(keymap); \
		break; \
	case __EV ## _desktop2: \
		get_self<KeyViewModel>(__VAR ## Desktop2_)->Bind(keymap); \
		break; \
	case __EV ## _desktop3: \
		get_self<KeyViewModel>(__VAR ## Desktop3_)->Bind(keymap); \
		break; \
	case __EV ## _desktop4: \
		get_self<KeyViewModel>(__VAR ## Desktop4_)->Bind(keymap); \
		break; \
	case __EV ## _desktop5: \
		get_self<KeyViewModel>(__VAR ## Desktop5_)->Bind(keymap); \
		break; \
	case __EV ## _desktop6: \
		get_self<KeyViewModel>(__VAR ## Desktop6_)->Bind(keymap); \
		break; \
	case __EV ## _desktop7: \
		get_self<KeyViewModel>(__VAR ## Desktop7_)->Bind(keymap); \
		break; \
	case __EV ## _desktop8: \
		get_self<KeyViewModel>(__VAR ## Desktop8_)->Bind(keymap); \
		break; \
	case __EV ## _desktop9: \
		get_self<KeyViewModel>(__VAR ## Desktop9_)->Bind(keymap); \
		break; \
	case __EV ## _desktop10: \
		get_self<KeyViewModel>(__VAR ## Desktop10_)->Bind(keymap); \
		break; \
	case __EV ## _desktop11: \
		get_self<KeyViewModel>(__VAR ## Desktop11_)->Bind(keymap); \
		break; \
	case __EV ## _desktop12: \
		get_self<KeyViewModel>(__VAR ## Desktop12_)->Bind(keymap); \
		break; \
	case __EV ## _desktop13: \
		get_self<KeyViewModel>(__VAR ## Desktop13_)->Bind(keymap); \
		break; \
	case __EV ## _desktop14: \
		get_self<KeyViewModel>(__VAR ## Desktop14_)->Bind(keymap); \
		break; \
	case __EV ## _desktop15: \
		get_self<KeyViewModel>(__VAR ## Desktop15_)->Bind(keymap); \
		break; \
	case __EV ## _desktop16: \
		get_self<KeyViewModel>(__VAR ## Desktop16_)->Bind(keymap); \
		break; \
	case __EV ## _desktop17: \
		get_self<KeyViewModel>(__VAR ## Desktop17_)->Bind(keymap); \
		break; \
	case __EV ## _desktop18: \
		get_self<KeyViewModel>(__VAR ## Desktop18_)->Bind(keymap); \
		break; \
	case __EV ## _desktop19: \
		get_self<KeyViewModel>(__VAR ## Desktop19_)->Bind(keymap); \
		break; \
	case __EV ## _desktop20: \
		get_self<KeyViewModel>(__VAR ## Desktop20_)->Bind(keymap); \
		break; \
	case __EV ## _first: \
		get_self<KeyViewModel>(__VAR ## FirstDesktop_)->Bind(keymap); \
		break; \
	case __EV ## _last: \
		get_self<KeyViewModel>(__VAR ## LastDesktop_)->Bind(keymap); \
		break; \
	case __EV ## _left: \
		get_self<KeyViewModel>(__VAR ## LeftDesktop_)->Bind(keymap); \
		break; \
	case __EV ## _right: \
		get_self<KeyViewModel>(__VAR ## RightDesktop_)->Bind(keymap); \
		break

ShortcutKeyViewModel::ShortcutKeyViewModel(std::shared_ptr<app::storage::KeyConfig> config) noexcept
	: config_(std::move(config)) {
	using namespace app::keylistener;

	for (app::storage::keymap_t const& keymap : config_->raw().keymaps) {
		switch (keymap.ev) {
		case kbe_exit:
			get_self<KeyViewModel>(ExitApplication_)->Bind(keymap);
			break;
		case kbe_rename:
			get_self<KeyViewModel>(RenameDesktop_)->Bind(keymap);
			break;
		case kbe_settings:
			get_self<KeyViewModel>(OpenSettings_)->Bind(keymap);
			break;
		case kbe_taskview:
			get_self<KeyViewModel>(OpenTaskView_)->Bind(keymap);
			break;
		case kbe_topmost_toggle:
			get_self<KeyViewModel>(SetOrRemoveTopmost_)->Bind(keymap);
			break;
		case kbe_pin_toggle:
			get_self<KeyViewModel>(PinOrUnpinWindow_)->Bind(keymap);
			break;

		MAP_KEYMAP(kbe_switch, Switch);
		MAP_KEYMAP(kbe_move_window, MoveWindowTo);
		MAP_KEYMAP(kbe_move_window_and_switch, MoveWindowAndSwitch);

		default:
			break;
		}
	}
}

winrt::SettingsSavedStatus ShortcutKeyViewModel::SaveCore() {
	return SettingsSavedStatus::Succeeded;
}
