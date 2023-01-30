#include "pch.h"
#include "KeyboardHelper.h"

#include <winrt/Windows.Foundation.h> // Fix buggy IntelliSense :(
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>

namespace resources {
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
	constexpr std::wstring_view Left { L"KeyVisual_Key_Left" };
	constexpr std::wstring_view Right { L"KeyVisual_Key_Right" };
	constexpr std::wstring_view NumberPad { L"KeyVisual_Key_NumberPad" };

	constexpr std::wstring_view Convert { L"KeyVisual_Key_Convert" };
	constexpr std::wstring_view NonConvert { L"KeyVisual_Key_NonConvert" };
}

namespace key {
	constexpr std::wstring_view Empty;

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

	constexpr std::wstring_view GoBack { L"Go Back" };
	constexpr std::wstring_view GoForward { L"Go Forward" };
	constexpr std::wstring_view Refresh { L"Refresh" };
	constexpr std::wstring_view Stop { L"Stop" };
	constexpr std::wstring_view Search { L"Search" };
	constexpr std::wstring_view Favorites { L"Favorites" };
	constexpr std::wstring_view GoHome { L"GoHome" };

	constexpr std::wstring_view VolumeMute { L"Volume Mute" };
	constexpr std::wstring_view VolumeDown { L"Volume Down" };
	constexpr std::wstring_view VolumeUp { L"Volume Up" };

	constexpr std::wstring_view MediaNextTrack { L"Next Track" };
	constexpr std::wstring_view MediaPreviousTrack { L"Previous Track" };
	constexpr std::wstring_view MediaStop { L"Stop" };
	constexpr std::wstring_view MediaPlayPause { L"Play/Pause" };

	constexpr std::wstring_view AppMail { L"Mail" };
	constexpr std::wstring_view AppMediaSelect { L"MediaSelect" };
	constexpr std::wstring_view App1 { L"App1" };
	constexpr std::wstring_view App2 { L"App2" };

	constexpr std::wstring_view SemiColon { L";" };
	constexpr std::wstring_view Comma { L"," };
	constexpr std::wstring_view Hyphen { L"-" };
	constexpr std::wstring_view Dot { L"." };
	constexpr std::wstring_view Slash { L"/" };

	constexpr std::wstring_view SquareBracketLeft { L"[" };
	constexpr std::wstring_view BackSlash { L"\\" };
	constexpr std::wstring_view YenSign { L"\u00A5" };
	constexpr std::wstring_view WonSign { L"\u20A9" };
	constexpr std::wstring_view SquareBracketRight { L"]" };
	constexpr std::wstring_view Apostrophe { L"'" };
	
}

enum class KeyboardType {
	US,
	JIS,
	KR,
};

namespace winrt {
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::System;

	using namespace ::winrt::Microsoft::UI::Xaml::Controls;
	using namespace ::winrt::Microsoft::UI::Xaml::Media::Imaging;
}

using namespace winrt::PositiveDesktop::UI::Controls::implementation;

winrt::hstring LoadString(std::wstring_view key) {
	using namespace ::winrt::Microsoft::Windows::ApplicationModel::Resources;
	static ResourceMap resourceMap { ResourceManager().MainResourceMap().GetSubtree(resources::CodeResources) };
	winrt::hstring value { resourceMap.GetValue(key).ValueAsString() };
	return value;
}

KeyboardHelper::keymap_type KeyboardHelper::GetKeymap(std::nullptr_t) {
	return std::make_tuple(nullptr, winrt::hstring(key::Empty));
}

KeyboardHelper::keymap_type KeyboardHelper::GetKeymap(std::wstring_view keymapText) {
	return std::make_tuple(winrt::box_value(keymapText), winrt::hstring(key::Empty));
}

KeyboardHelper::keymap_type KeyboardHelper::GetKeymap(std::wstring_view keymapText, winrt::hstring const& prefixText) {
	return std::make_tuple(winrt::box_value(keymapText), prefixText);
}

KeyboardHelper::keymap_type KeyboardHelper::GetKeymap(UINT code) {
	UINT codePoint { MapVirtualKeyExW(code, MAPVK_VK_TO_CHAR, keyboardLayout_) };
	wchar_t str[] { static_cast<wchar_t>(codePoint), L'\0' };
	return std::make_tuple(winrt::box_value(winrt::hstring(str)), winrt::hstring(key::Empty));
}

KeyboardHelper::keymap_type LoadStringToKeymap(std::wstring_view key) {
	winrt::hstring value { LoadString(key) };
	return std::make_tuple(winrt::box_value(value), winrt::hstring(key::Empty));
}

KeyboardHelper::keymap_type LoadImageToKeymap(std::wstring_view uri, winrt::hstring const& prefixText) {
	winrt::BitmapIconSource icon;
	icon.ShowAsMonochrome(true);
	icon.UriSource(winrt::Uri(uri));
	return std::make_tuple(icon, prefixText);
}

KeyboardHelper::keymap_type LoadImageToKeymap(std::wstring_view uri) {
	return LoadImageToKeymap(uri, winrt::hstring(key::Empty));
}

KeyboardHelper::keymap_array_type KeyboardHelper::InitKeymap() {
	wchar_t layoutName[9] { L'\0' };
	check_bool(GetKeyboardLayoutNameW(layoutName));
	KeyboardType type { wcsncmp(layoutName, L"00000411", 8) == 0 ? KeyboardType::JIS : KeyboardType::US };

	winrt::hstring left { LoadString(keyres::Left) };
	winrt::hstring right { LoadString(keyres::Right) };
	winrt::hstring numpad { LoadString(keyres::NumberPad) };
	KeyboardHelper::keymap_array_type keymap {
		GetKeymap(key::None),					// 0
		GetKeymap(key::Click),					// 1
		GetKeymap(key::RightClick),				// 2
		GetKeymap(key::Cancel),					// 3
		GetKeymap(key::MiddleClick),			// 4
		GetKeymap(key::XButton1),				// 5
		GetKeymap(key::XButton2),				// 6

		GetKeymap(nullptr),						// 7

		GetKeymap(key::Backspace),				// 8
		LoadImageToKeymap(keyuri::Tab),			// 9

		GetKeymap(nullptr),						// 10
		GetKeymap(nullptr),						// 11

		GetKeymap(key::Clear),					// 12
		LoadImageToKeymap(keyuri::Enter),		// 13

		GetKeymap(nullptr),						// 14
		GetKeymap(nullptr),						// 15

		LoadImageToKeymap(keyuri::Shift),		// 16
		GetKeymap(key::Control),				// 17
		GetKeymap(key::Alt),					// 18
		GetKeymap(key::Pause),					// 19
		GetKeymap(key::CapsLock),				// 20

		GetKeymap(nullptr),						// 21
		GetKeymap(nullptr),						// 22
		GetKeymap(nullptr),						// 23
		GetKeymap(nullptr),						// 24
		GetKeymap(nullptr),						// 25
		GetKeymap(nullptr),						// 26

		GetKeymap(key::Escape),					// 27
		LoadStringToKeymap(keyres::Convert),	// 28
		LoadStringToKeymap(keyres::NonConvert),	// 29
		GetKeymap(key::Accept),					// 30
		GetKeymap(key::ModeChange),				// 31
		GetKeymap(key::Space),					// 32
		GetKeymap(key::PageUp),					// 33
		GetKeymap(key::PageDown),				// 34
		GetKeymap(key::End),					// 35
		GetKeymap(key::Home),					// 36
		LoadImageToKeymap(keyuri::ArrowLeft),	// 37
		LoadImageToKeymap(keyuri::ArrowUp),		// 38
		LoadImageToKeymap(keyuri::ArrowRight),	// 39
		LoadImageToKeymap(keyuri::ArrowDown),	// 40
		GetKeymap(key::Select),					// 41
		GetKeymap(key::Print),					// 42
		GetKeymap(key::Execute),				// 43
		GetKeymap(key::PrintScreen),			// 44
		GetKeymap(key::Insert),					// 45
		GetKeymap(key::Delete),					// 46
		GetKeymap(key::Help),					// 47

		// 48 to 57
		GetKeymap(key::Key0),
		GetKeymap(key::Key1),
		GetKeymap(key::Key2),
		GetKeymap(key::Key3),
		GetKeymap(key::Key4),
		GetKeymap(key::Key5),
		GetKeymap(key::Key6),
		GetKeymap(key::Key7),
		GetKeymap(key::Key8),
		GetKeymap(key::Key9),

		GetKeymap(nullptr),						// 58
		GetKeymap(nullptr),						// 59
		GetKeymap(nullptr),						// 60
		GetKeymap(nullptr),						// 61
		GetKeymap(nullptr),						// 62
		GetKeymap(nullptr),						// 63
		GetKeymap(nullptr),						// 64

		// 65 to 90
		GetKeymap(key::KeyA),
		GetKeymap(key::KeyB),
		GetKeymap(key::KeyC),
		GetKeymap(key::KeyD),
		GetKeymap(key::KeyE),
		GetKeymap(key::KeyF),
		GetKeymap(key::KeyG),
		GetKeymap(key::KeyH),
		GetKeymap(key::KeyI),
		GetKeymap(key::KeyJ),
		GetKeymap(key::KeyK),
		GetKeymap(key::KeyL),
		GetKeymap(key::KeyM),
		GetKeymap(key::KeyN),
		GetKeymap(key::KeyO),
		GetKeymap(key::KeyP),
		GetKeymap(key::KeyQ),
		GetKeymap(key::KeyR),
		GetKeymap(key::KeyS),
		GetKeymap(key::KeyT),
		GetKeymap(key::KeyU),
		GetKeymap(key::KeyV),
		GetKeymap(key::KeyW),
		GetKeymap(key::KeyX),
		GetKeymap(key::KeyY),
		GetKeymap(key::KeyZ),

		GetKeymap(key::Windows, left),			// 91
		GetKeymap(key::Windows, right),			// 92
		GetKeymap(key::Application),			// 93
		GetKeymap(nullptr),						// 94
		GetKeymap(key::Sleep),					// 95

		// 96 to 105
		GetKeymap(key::Key0, numpad),
		GetKeymap(key::Key1, numpad),
		GetKeymap(key::Key2, numpad),
		GetKeymap(key::Key3, numpad),
		GetKeymap(key::Key4, numpad),
		GetKeymap(key::Key5, numpad),
		GetKeymap(key::Key6, numpad),
		GetKeymap(key::Key7, numpad),
		GetKeymap(key::Key8, numpad),
		GetKeymap(key::Key9, numpad),

		GetKeymap(key::Multiply, numpad),		// 106
		GetKeymap(key::Add, numpad),			// 107
		LoadImageToKeymap(keyuri::Enter, numpad),// 108
		GetKeymap(key::Subtract, numpad),		// 109
		GetKeymap(key::Decimal, numpad),		// 110
		GetKeymap(key::Divide, numpad),			// 111

		// 112 to 135
		GetKeymap(key::F1),
		GetKeymap(key::F2),
		GetKeymap(key::F3),
		GetKeymap(key::F4),
		GetKeymap(key::F5),
		GetKeymap(key::F6),
		GetKeymap(key::F7),
		GetKeymap(key::F8),
		GetKeymap(key::F9),
		GetKeymap(key::F10),
		GetKeymap(key::F11),
		GetKeymap(key::F12),
		GetKeymap(key::F13),
		GetKeymap(key::F14),
		GetKeymap(key::F15),
		GetKeymap(key::F16),
		GetKeymap(key::F17),
		GetKeymap(key::F18),
		GetKeymap(key::F19),
		GetKeymap(key::F20),
		GetKeymap(key::F21),
		GetKeymap(key::F22),
		GetKeymap(key::F23),
		GetKeymap(key::F24),

		GetKeymap(nullptr),						// 136
		GetKeymap(nullptr),						// 137
		GetKeymap(nullptr),						// 138
		GetKeymap(nullptr),						// 139
		GetKeymap(nullptr),						// 140
		GetKeymap(nullptr),						// 141
		GetKeymap(nullptr),						// 142
		GetKeymap(nullptr),						// 143

		GetKeymap(key::NumLock),				// 144
		GetKeymap(key::ScrollLock),				// 145

		GetKeymap(nullptr),						// 146
		GetKeymap(nullptr),						// 147
		GetKeymap(nullptr),						// 148
		GetKeymap(nullptr),						// 149
		GetKeymap(nullptr),						// 150
		GetKeymap(nullptr),						// 151
		GetKeymap(nullptr),						// 152
		GetKeymap(nullptr),						// 153
		GetKeymap(nullptr),						// 154
		GetKeymap(nullptr),						// 155
		GetKeymap(nullptr),						// 156
		GetKeymap(nullptr),						// 157
		GetKeymap(nullptr),						// 158
		GetKeymap(nullptr),						// 159

		LoadImageToKeymap(keyuri::Shift, left),	// 160
		LoadImageToKeymap(keyuri::Shift, right),// 161
		GetKeymap(key::Control, left),			// 162
		GetKeymap(key::Control, right),			// 163
		GetKeymap(key::Alt, left),				// 164
		GetKeymap(key::Alt, right),				// 165

		GetKeymap(key::GoBack),					// 166
		GetKeymap(key::GoForward),				// 167
		GetKeymap(key::Refresh),				// 168
		GetKeymap(key::Stop),					// 169
		GetKeymap(key::Search),					// 170
		GetKeymap(key::Favorites),				// 171
		GetKeymap(key::Home),					// 172

		GetKeymap(key::VolumeMute),				// 173
		GetKeymap(key::VolumeDown),				// 174
		GetKeymap(key::VolumeUp),				// 175

		GetKeymap(key::MediaNextTrack),			// 176
		GetKeymap(key::MediaPreviousTrack),		// 177
		GetKeymap(key::MediaStop),				// 178
		GetKeymap(key::MediaPlayPause),			// 179

		GetKeymap(key::AppMail),				// 180
		GetKeymap(key::AppMediaSelect),			// 181
		GetKeymap(key::App1),					// 182
		GetKeymap(key::App2),					// 183

		GetKeymap(nullptr),						// 184
		GetKeymap(nullptr),						// 185

		GetKeymap(186u),						// 186
		GetKeymap(187u),						// 187
		GetKeymap(key::Comma),					// 188
		GetKeymap(key::Hyphen),					// 189
		GetKeymap(key::Dot),					// 190
		GetKeymap(key::Slash),					// 191
		GetKeymap(192u),						// 192

		GetKeymap(nullptr),						// 193
		GetKeymap(nullptr),						// 194
		GetKeymap(nullptr),						// 195
		GetKeymap(nullptr),						// 196
		GetKeymap(nullptr),						// 197
		GetKeymap(nullptr),						// 198
		GetKeymap(nullptr),						// 199
		GetKeymap(nullptr),						// 200
		GetKeymap(nullptr),						// 201
		GetKeymap(nullptr),						// 202
		GetKeymap(nullptr),						// 203
		GetKeymap(nullptr),						// 204
		GetKeymap(nullptr),						// 205
		GetKeymap(nullptr),						// 206
		GetKeymap(nullptr),						// 207
		GetKeymap(nullptr),						// 208
		GetKeymap(nullptr),						// 209
		GetKeymap(nullptr),						// 210
		GetKeymap(nullptr),						// 211
		GetKeymap(nullptr),						// 212
		GetKeymap(nullptr),						// 213
		GetKeymap(nullptr),						// 214
		GetKeymap(nullptr),						// 215
		GetKeymap(nullptr),						// 216
		GetKeymap(nullptr),						// 217
		GetKeymap(nullptr),						// 218

		GetKeymap(key::SquareBracketLeft),		// 219
		GetKeymap(type == KeyboardType::JIS ? key::YenSign : key::BackSlash), // 220
		GetKeymap(key::SquareBracketRight),		// 221
		GetKeymap(222u),						// 222
		GetKeymap(223u),						// 223
		GetKeymap(nullptr),						// 224
		GetKeymap(nullptr),						// 225
		GetKeymap(226u),						// 226
	};
	return std::move(keymap);
}

HKL KeyboardHelper::keyboardLayout_ { nullptr };
KeyboardHelper::keymap_array_type KeyboardHelper::keymap_;

void KeyboardHelper::init() {
	static bool initialized { false };
	if (initialized) return;

	keyboardLayout_ = GetKeyboardLayout(0);
	keymap_ = InitKeymap();

	initialized = true;
}


