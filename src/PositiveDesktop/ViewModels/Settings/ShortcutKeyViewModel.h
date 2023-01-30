#pragma once
#include "ViewModels/Settings/ShortcutKeyViewModel.g.h"

#include "SettingsBaseViewModel.h"
#include "KeyViewModel.h"

#define DEFINE_PROP(__NAME__) \
	public:  inline ::winrt::PositiveDesktop::ViewModels::Settings::KeyViewModel __NAME__##() const noexcept { return __NAME__##_; } \
	private: ::winrt::PositiveDesktop::ViewModels::Settings::KeyViewModel __NAME__##_ { make<::winrt::PositiveDesktop::ViewModels::Settings::implementation::KeyViewModel>() }

namespace winrt::PositiveDesktop::ViewModels::Settings::implementation {

	struct ShortcutKeyViewModel: ShortcutKeyViewModelT<ShortcutKeyViewModel, SettingsBaseViewModel> {
		ShortcutKeyViewModel() noexcept = default;

		SettingsSavedStatus SaveCore() override final;

	public:  // - Properties
		DEFINE_PROP(ExitApplication);
		DEFINE_PROP(RenameDesktop);
		DEFINE_PROP(OpenSettings);
		DEFINE_PROP(OpenTaskView);
		DEFINE_PROP(SetOrRemoveTopmost);
		DEFINE_PROP(PinOrUnpinWindow);

		DEFINE_PROP(SwitchDesktop1);
		DEFINE_PROP(SwitchDesktop2);
		DEFINE_PROP(SwitchDesktop3);
		DEFINE_PROP(SwitchDesktop4);
		DEFINE_PROP(SwitchDesktop5);
		DEFINE_PROP(SwitchDesktop6);
		DEFINE_PROP(SwitchDesktop7);
		DEFINE_PROP(SwitchDesktop8);
		DEFINE_PROP(SwitchDesktop9);
		DEFINE_PROP(SwitchDesktop10);
		DEFINE_PROP(SwitchDesktop11);
		DEFINE_PROP(SwitchDesktop12);
		DEFINE_PROP(SwitchDesktop13);
		DEFINE_PROP(SwitchDesktop14);
		DEFINE_PROP(SwitchDesktop15);
		DEFINE_PROP(SwitchDesktop16);
		DEFINE_PROP(SwitchDesktop17);
		DEFINE_PROP(SwitchDesktop18);
		DEFINE_PROP(SwitchDesktop19);
		DEFINE_PROP(SwitchDesktop20);
		DEFINE_PROP(SwitchFirstDesktop);
		DEFINE_PROP(SwitchLastDesktop);
		DEFINE_PROP(SwitchLeftDesktop);
		DEFINE_PROP(SwitchRightDesktop);
		DEFINE_PROP(SwitchNewFirstDesktop);
		DEFINE_PROP(SwitchNewLastDesktop);
		DEFINE_PROP(SwitchNewLeftDesktop);
		DEFINE_PROP(SwitchNewRightDesktop);

		DEFINE_PROP(MoveWindowToDesktop1);
		DEFINE_PROP(MoveWindowToDesktop2);
		DEFINE_PROP(MoveWindowToDesktop3);
		DEFINE_PROP(MoveWindowToDesktop4);
		DEFINE_PROP(MoveWindowToDesktop5);
		DEFINE_PROP(MoveWindowToDesktop6);
		DEFINE_PROP(MoveWindowToDesktop7);
		DEFINE_PROP(MoveWindowToDesktop8);
		DEFINE_PROP(MoveWindowToDesktop9);
		DEFINE_PROP(MoveWindowToDesktop10);
		DEFINE_PROP(MoveWindowToDesktop11);
		DEFINE_PROP(MoveWindowToDesktop12);
		DEFINE_PROP(MoveWindowToDesktop13);
		DEFINE_PROP(MoveWindowToDesktop14);
		DEFINE_PROP(MoveWindowToDesktop15);
		DEFINE_PROP(MoveWindowToDesktop16);
		DEFINE_PROP(MoveWindowToDesktop17);
		DEFINE_PROP(MoveWindowToDesktop18);
		DEFINE_PROP(MoveWindowToDesktop19);
		DEFINE_PROP(MoveWindowToDesktop20);
		DEFINE_PROP(MoveWindowToFirstDesktop);
		DEFINE_PROP(MoveWindowToLastDesktop);
		DEFINE_PROP(MoveWindowToLeftDesktop);
		DEFINE_PROP(MoveWindowToRightDesktop);
		DEFINE_PROP(MoveWindowToNewFirstDesktop);
		DEFINE_PROP(MoveWindowToNewLastDesktop);
		DEFINE_PROP(MoveWindowToNewLeftDesktop);
		DEFINE_PROP(MoveWindowToNewRightDesktop);

		DEFINE_PROP(MoveWindowAndSwitchDesktop1);
		DEFINE_PROP(MoveWindowAndSwitchDesktop2);
		DEFINE_PROP(MoveWindowAndSwitchDesktop3);
		DEFINE_PROP(MoveWindowAndSwitchDesktop4);
		DEFINE_PROP(MoveWindowAndSwitchDesktop5);
		DEFINE_PROP(MoveWindowAndSwitchDesktop6);
		DEFINE_PROP(MoveWindowAndSwitchDesktop7);
		DEFINE_PROP(MoveWindowAndSwitchDesktop8);
		DEFINE_PROP(MoveWindowAndSwitchDesktop9);
		DEFINE_PROP(MoveWindowAndSwitchDesktop10);
		DEFINE_PROP(MoveWindowAndSwitchDesktop11);
		DEFINE_PROP(MoveWindowAndSwitchDesktop12);
		DEFINE_PROP(MoveWindowAndSwitchDesktop13);
		DEFINE_PROP(MoveWindowAndSwitchDesktop14);
		DEFINE_PROP(MoveWindowAndSwitchDesktop15);
		DEFINE_PROP(MoveWindowAndSwitchDesktop16);
		DEFINE_PROP(MoveWindowAndSwitchDesktop17);
		DEFINE_PROP(MoveWindowAndSwitchDesktop18);
		DEFINE_PROP(MoveWindowAndSwitchDesktop19);
		DEFINE_PROP(MoveWindowAndSwitchDesktop20);
		DEFINE_PROP(MoveWindowAndSwitchFirstDesktop);
		DEFINE_PROP(MoveWindowAndSwitchLastDesktop);
		DEFINE_PROP(MoveWindowAndSwitchLeftDesktop);
		DEFINE_PROP(MoveWindowAndSwitchRightDesktop);
		DEFINE_PROP(MoveWindowAndSwitchNewFirstDesktop);
		DEFINE_PROP(MoveWindowAndSwitchNewLastDesktop);
		DEFINE_PROP(MoveWindowAndSwitchNewLeftDesktop);
		DEFINE_PROP(MoveWindowAndSwitchNewRightDesktop);
	};

}

#undef DEFINE_PROP
