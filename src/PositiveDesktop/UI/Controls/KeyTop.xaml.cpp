#include "pch.h"
#include "KeyTop.xaml.h"

#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>

namespace resources {
	constexpr std::wstring_view PositiveDesktop_UI_Controls_KeyTop { L"PositiveDesktop.UI.Controls.KeyTop" };
	constexpr std::wstring_view PositiveDesktop_UI_Controls_KeyTop_Uri { L"ms-appx:///UI/Controls/KeyTop.xaml" };

	constexpr std::wstring_view CodeResources { L"CodeResources" };
}

namespace keyuri {
	constexpr std::wstring_view Tab { L"ms-appx:///Assets/KeyboardTab.png" };
	constexpr std::wstring_view Enter { L"ms-appx:///Assets/KeyboardEnter.png" };
	constexpr std::wstring_view ArrowLeft { L"ms-appx:///Assets/KeyboardArrowLeft.png" };
	constexpr std::wstring_view ArrowUp { L"ms-appx:///Assets/KeyboardArrowUp.png" };
	constexpr std::wstring_view ArrowRight { L"ms-appx:///Assets/KeyboardArrowRight.png" };
	constexpr std::wstring_view ArrowDown { L"ms-appx:///Assets/KeyboardArrowDown.png" };
	constexpr std::wstring_view Shift { L"ms-appx:///Assets/KeyboardShift.png" };
}

namespace keyres {
	constexpr std::wstring_view Convert { L"KeyTop_Convert" };
	constexpr std::wstring_view NonConvert { L"KeyTop_NonConvert" };
	constexpr std::wstring_view NumberPad { L"KeyTop_NumberPad" };
}

namespace key {
	constexpr std::wstring_view None { L"None" };
	constexpr std::wstring_view Click { L"Click" };
	constexpr std::wstring_view RightClick { L"Right Click" };
	constexpr std::wstring_view Cancel { L"Cancel" };
	constexpr std::wstring_view MiddleClick { L"Middle Click" };
	constexpr std::wstring_view XButton1 { L"XButton1" };
	constexpr std::wstring_view XButton2 { L"XButton2" };

	constexpr std::wstring_view Backspace { L"Backspace" };
	//constexpr std::wstring_view Tab { L"Tab" }; /* Use the image */
	constexpr std::wstring_view Clear { L"Clear" };
	//constexpr std::wstring_view Enter { L"Enter" }; /* Use the image */
	constexpr std::wstring_view Shift { L"Shift" };
	constexpr std::wstring_view Control { L"Ctrl" };
	constexpr std::wstring_view Alt { L"Alt" };
	constexpr std::wstring_view Pause { L"Pause" };
	constexpr std::wstring_view CapsLock { L"CapsLock" };
	constexpr std::wstring_view Escape { L"ESC" };
	constexpr std::wstring_view Accept { L"Accept" };
	constexpr std::wstring_view ModeChange { L"Mode Change" };
	constexpr std::wstring_view Space { L"Space" };
	constexpr std::wstring_view PageUp { L"Page Up" };
	constexpr std::wstring_view PageDown { L"Page Down" };
	constexpr std::wstring_view End { L"End" };
	constexpr std::wstring_view Home { L"Home" };
	//constexpr std::wstring_view Left { L"Left" }; /* Use the image */
	//constexpr std::wstring_view Up { L"Up" }; /* Use the image */
	//constexpr std::wstring_view Right { L"Right" }; /* Use the image */
	//constexpr std::wstring_view Down { L"Down" }; /* Use the image */
	constexpr std::wstring_view Select { L"Select" };
	constexpr std::wstring_view Print { L"Print" };
	constexpr std::wstring_view Execute { L"Execute" };
	constexpr std::wstring_view PrintScreen { L"Print Screen" };
	constexpr std::wstring_view Insert { L"Insert" };
	constexpr std::wstring_view Delete { L"Delete" };
	constexpr std::wstring_view Help { L"Help" };

	constexpr std::wstring_view Key0 { L"0" };
	constexpr std::wstring_view Key1 { L"1" };
	constexpr std::wstring_view Key2 { L"2" };
	constexpr std::wstring_view Key3 { L"3" };
	constexpr std::wstring_view Key4 { L"4" };
	constexpr std::wstring_view Key5 { L"5" };
	constexpr std::wstring_view Key6 { L"6" };
	constexpr std::wstring_view Key7 { L"7" };
	constexpr std::wstring_view Key8 { L"8" };
	constexpr std::wstring_view Key9 { L"9" };

	constexpr std::wstring_view KeyA { L"A" };
	constexpr std::wstring_view KeyB { L"B" };
	constexpr std::wstring_view KeyC { L"C" };
	constexpr std::wstring_view KeyD { L"D" };
	constexpr std::wstring_view KeyE { L"E" };
	constexpr std::wstring_view KeyF { L"F" };
	constexpr std::wstring_view KeyG { L"G" };
	constexpr std::wstring_view KeyH { L"H" };
	constexpr std::wstring_view KeyI { L"I" };
	constexpr std::wstring_view KeyJ { L"J" };
	constexpr std::wstring_view KeyK { L"K" };
	constexpr std::wstring_view KeyL { L"L" };
	constexpr std::wstring_view KeyM { L"M" };
	constexpr std::wstring_view KeyN { L"N" };
	constexpr std::wstring_view KeyO { L"O" };
	constexpr std::wstring_view KeyP { L"P" };
	constexpr std::wstring_view KeyQ { L"Q" };
	constexpr std::wstring_view KeyR { L"R" };
	constexpr std::wstring_view KeyS { L"S" };
	constexpr std::wstring_view KeyT { L"T" };
	constexpr std::wstring_view KeyU { L"U" };
	constexpr std::wstring_view KeyV { L"V" };
	constexpr std::wstring_view KeyW { L"W" };
	constexpr std::wstring_view KeyX { L"X" };
	constexpr std::wstring_view KeyY { L"Y" };
	constexpr std::wstring_view KeyZ { L"Z" };

	constexpr std::wstring_view Windows { L"Windows" };
	constexpr std::wstring_view Application { L"App" };
	constexpr std::wstring_view Sleep { L"Sleep" };

	constexpr std::wstring_view Multiply { L"*" };
	constexpr std::wstring_view Add { L"+" };
	constexpr std::wstring_view Subtract { L"-" };
	constexpr std::wstring_view Decimal { L"." };
	constexpr std::wstring_view Divide { L"/" };

	constexpr std::wstring_view F1 { L"F1" };
	constexpr std::wstring_view F2 { L"F2" };
	constexpr std::wstring_view F3 { L"F3" };
	constexpr std::wstring_view F4 { L"F4" };
	constexpr std::wstring_view F5 { L"F5" };
	constexpr std::wstring_view F6 { L"F6" };
	constexpr std::wstring_view F7 { L"F7" };
	constexpr std::wstring_view F8 { L"F8" };
	constexpr std::wstring_view F9 { L"F9" };
	constexpr std::wstring_view F10 { L"F10" };
	constexpr std::wstring_view F11 { L"F11" };
	constexpr std::wstring_view F12 { L"F12" };
	constexpr std::wstring_view F13 { L"F13" };
	constexpr std::wstring_view F14 { L"F14" };
	constexpr std::wstring_view F15 { L"F15" };
	constexpr std::wstring_view F16 { L"F16" };
	constexpr std::wstring_view F17 { L"F17" };
	constexpr std::wstring_view F18 { L"F18" };
	constexpr std::wstring_view F19 { L"F19" };
	constexpr std::wstring_view F20 { L"F20" };
	constexpr std::wstring_view F21 { L"F21" };
	constexpr std::wstring_view F22 { L"F22" };
	constexpr std::wstring_view F23 { L"F23" };
	constexpr std::wstring_view F24 { L"F24" };

	constexpr std::wstring_view NumLock { L"NumLock" };
	constexpr std::wstring_view ScrollLock { L"Scroll Lock" };
}

namespace controls {
	constexpr std::wstring_view Content { L"Content" };
	constexpr std::wstring_view PrefixContent { L"PrefixContent" };
}

namespace winrt {
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::System;

	using namespace ::winrt::Microsoft::UI::Xaml;
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;
	using namespace ::winrt::Microsoft::UI::Xaml::Media::Imaging;
	using namespace ::winrt::Microsoft::Windows::ApplicationModel::Resources;

	using namespace winrt::PositiveDesktop::UI::Controls;
}

using namespace winrt::PositiveDesktop::UI::Controls::implementation;

winrt::IInspectable LoadImageAsIInspectable(std::wstring_view uri) {
	winrt::BitmapIcon icon;
	icon.Height(20.0);
	icon.ShowAsMonochrome(true);
	icon.UriSource(winrt::Uri(uri));
	icon.Width(20.0);
	return icon;
}

winrt::IInspectable LoadStringAsIInspectable(winrt::ResourceMap const& resourceMap, std::wstring_view key) {
	winrt::hstring value { resourceMap.GetValue(key).ValueAsString() };
	return winrt::box_value(value);
}

std::array<winrt::IInspectable, 167> InitKeymap() {
	using namespace winrt;

	ResourceManager resourceManager;
	ResourceMap resourceMap { resourceManager.MainResourceMap().GetSubtree(resources::CodeResources) };
	std::array<winrt::IInspectable, 167> keymap {
		box_value(key::None),					// 0
		box_value(key::Click),					// 1
		box_value(key::RightClick),				// 2
		box_value(key::Cancel),					// 3
		box_value(key::MiddleClick),			// 4
		box_value(key::XButton1),				// 5
		box_value(key::XButton2),				// 6

		nullptr,								// 7

		box_value(key::Backspace),				// 8
		LoadImageAsIInspectable(keyuri::Tab),	// 9

		nullptr,								// 10
		nullptr,								// 11

		box_value(key::Clear),					// 12
		LoadImageAsIInspectable(keyuri::Enter),	// 13

		nullptr,								// 14
		nullptr,								// 15

		LoadImageAsIInspectable(keyuri::Shift),	// 16
		box_value(key::Control),				// 17
		box_value(key::Alt),					// 18
		box_value(key::Pause),					// 19
		box_value(key::CapsLock),				// 20

		nullptr,								// 21
		nullptr,								// 22
		nullptr,								// 23
		nullptr,								// 24
		nullptr,								// 25
		nullptr,								// 26

		box_value(key::Escape),					// 27
		LoadStringAsIInspectable(resourceMap, keyres::Convert), // 28
		LoadStringAsIInspectable(resourceMap, keyres::NonConvert), // 29
		box_value(key::Accept),					// 30
		box_value(key::ModeChange),				// 31
		box_value(key::Space),					// 32
		box_value(key::PageUp),					// 33
		box_value(key::PageDown),				// 34
		box_value(key::End),					// 35
		box_value(key::Home),					// 36
		LoadImageAsIInspectable(keyuri::ArrowLeft), // 37
		LoadImageAsIInspectable(keyuri::ArrowUp), // 38
		LoadImageAsIInspectable(keyuri::ArrowRight), // 39
		LoadImageAsIInspectable(keyuri::ArrowDown), // 40
		box_value(key::Select),					// 41
		box_value(key::Print),					// 42
		box_value(key::Execute),				// 43
		box_value(key::PrintScreen),			// 44
		box_value(key::Insert),					// 45
		box_value(key::Delete),					// 46
		box_value(key::Help),					// 47

		// 48 to 57
		box_value(key::Key0),
		box_value(key::Key1),
		box_value(key::Key2),
		box_value(key::Key3),
		box_value(key::Key4),
		box_value(key::Key5),
		box_value(key::Key6),
		box_value(key::Key7),
		box_value(key::Key8),
		box_value(key::Key9),

		nullptr,								// 58
		nullptr,								// 59
		nullptr,								// 60
		nullptr,								// 61
		nullptr,								// 62
		nullptr,								// 63
		nullptr,								// 64

		// 65 to 90
		box_value(key::KeyA),
		box_value(key::KeyB),
		box_value(key::KeyC),
		box_value(key::KeyD),
		box_value(key::KeyE),
		box_value(key::KeyF),
		box_value(key::KeyG),
		box_value(key::KeyH),
		box_value(key::KeyI),
		box_value(key::KeyJ),
		box_value(key::KeyK),
		box_value(key::KeyL),
		box_value(key::KeyM),
		box_value(key::KeyN),
		box_value(key::KeyO),
		box_value(key::KeyP),
		box_value(key::KeyQ),
		box_value(key::KeyR),
		box_value(key::KeyS),
		box_value(key::KeyT),
		box_value(key::KeyU),
		box_value(key::KeyV),
		box_value(key::KeyW),
		box_value(key::KeyX),
		box_value(key::KeyY),
		box_value(key::KeyZ),

		box_value(key::Windows),				// 91
		box_value(key::Windows),				// 92
		box_value(key::Application),			// 93
		nullptr,								// 94
		box_value(key::Sleep),					// 95

		// 96 to 105
		box_value(key::Key0),
		box_value(key::Key1),
		box_value(key::Key2),
		box_value(key::Key3),
		box_value(key::Key4),
		box_value(key::Key5),
		box_value(key::Key6),
		box_value(key::Key7),
		box_value(key::Key8),
		box_value(key::Key9),

		box_value(key::Multiply),				// 106
		box_value(key::Add),					// 107
		LoadImageAsIInspectable(keyuri::Enter),	// 108
		box_value(key::Subtract),				// 109
		box_value(key::Decimal),				// 110
		box_value(key::Divide),					// 111

		// 112 to 135
		box_value(key::F1),
		box_value(key::F2),
		box_value(key::F3),
		box_value(key::F4),
		box_value(key::F5),
		box_value(key::F6),
		box_value(key::F7),
		box_value(key::F8),
		box_value(key::F9),
		box_value(key::F10),
		box_value(key::F11),
		box_value(key::F12),
		box_value(key::F13),
		box_value(key::F14),
		box_value(key::F15),
		box_value(key::F16),
		box_value(key::F17),
		box_value(key::F18),
		box_value(key::F19),
		box_value(key::F20),
		box_value(key::F21),
		box_value(key::F22),
		box_value(key::F23),
		box_value(key::F24),

		nullptr,								// 136
		nullptr,								// 137
		nullptr,								// 138
		nullptr,								// 139
		nullptr,								// 140
		nullptr,								// 141
		nullptr,								// 142
		nullptr,								// 143

		box_value(key::NumLock),				// 144
		box_value(key::ScrollLock),				// 145

		nullptr,								// 146
		nullptr,								// 147
		nullptr,								// 148
		nullptr,								// 149
		nullptr,								// 150
		nullptr,								// 151
		nullptr,								// 152
		nullptr,								// 153
		nullptr,								// 154
		nullptr,								// 155
		nullptr,								// 156
		nullptr,								// 157
		nullptr,								// 158
		nullptr,								// 159

		LoadImageAsIInspectable(keyuri::Shift),	// 160
		LoadImageAsIInspectable(keyuri::Shift),	// 161
		box_value(key::Control),				// 162
		box_value(key::Control),				// 163
		box_value(key::Alt),					// 164
		box_value(key::Alt),					// 165
	};
	return std::move(keymap);
}

KeyTop::KeyTop(): keymap_(InitKeymap()) {
	props_.DelayInitIfNeeded();
	DefaultStyleKey(box_value(resources::PositiveDesktop_UI_Controls_KeyTop));
	DefaultStyleResourceUri(Uri(resources::PositiveDesktop_UI_Controls_KeyTop_Uri));
}

void KeyTop::OnApplyTemplate() {
	__super::OnApplyTemplate();

	//content_ = GetTemplateChild(controls::Content).as<TextBlock>();
	//content_.Text(L"Test");
	OnKeyTopChanged(Key());
}

void KeyTop::OnKeyTopChanged(VirtualKey newValue) const {
	TextBlock prefixContent { GetTemplateChild(controls::PrefixContent).try_as<TextBlock>() };
	if (!prefixContent) return;

	ContentPresenter content { GetTemplateChild(controls::Content).try_as<ContentPresenter>() };
	if (!content) return;

	size_t val { static_cast<size_t>(newValue) };
	if (val < keymap_.size()) {
		prefixContent.Visibility(Visibility::Collapsed);
		content.Content(keymap_[val]);
	} else {
		prefixContent.Visibility(Visibility::Collapsed);
		content.Content(box_value(L"Unknown"));
	}
	//if (VirtualKey::Number0 <= newValue && newValue <= VirtualKey::Z) {
	//	wchar_t str[] { static_cast<wchar_t>(newValue), L'\0' };
	//	prefixContent.Visibility(Visibility::Collapsed);
	//	content.Content(box_value(str));
	//} else if (VirtualKey::NumberPad0 <= newValue && newValue <= VirtualKey::NumberPad9) {
	//	wchar_t str[] { static_cast<wchar_t>(static_cast<int>(newValue) - (96 /* NumberPad 0 */ - 48 /* Number0 */)), L'\0' };
	//	prefixContent.Visibility(Visibility::Visible);
	//	prefixContent.Text(L"NumPad");
	//	content.Content(box_value(str));
	//} else {
	//	switch (newValue) {
	//	case VirtualKey::LeftShift:
	//		prefixContent.Visibility(Visibility::Visible);
	//		prefixContent.Text(L"Left");
	//		content.Content(box_value(L"Shift"));
	//		break;
	//	case VirtualKey::Tab:
	//	{
	//		prefixContent.Visibility(Visibility::Collapsed);
	//		BitmapImage bitmapImage;
	//		bitmapImage.UriSource(Uri(L"ms-appx:///Assets/KeyboardTab.png"));
	//		Image image;
	//		image.Source(bitmapImage);
	//		content.Content(image);
	//		break;
	//	}
	//	default:
	//		prefixContent.Visibility(Visibility::Collapsed);
	//		content.Content(box_value(L"Unknown"));
	//		break;
	//	}
	//}
}

void KeyTop::OnKeyTopChangedStatic(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args) {
	get_self<KeyTop>(sender.as<winrt::KeyTop>())->OnKeyTopChanged(args.NewValue().as<VirtualKey>());
}
