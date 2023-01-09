#pragma once
#include "shared.h"

#define WIN32_LEAN_AND_MEAN
#define WINRT_LEAN_AND_MEAN

// Unused definition
#define NOSERVICE
#define NOMCX

// windef.h
// - minwindef.h
#define NOMINMAX

// winbase.h
// - interlockedapi.h
#define NOWINBASEINTERLOCK

// wingdi.h
#define NORASTEROPS
#define NOMETAFILE
#define NOTEXTMETRIC
#define NOGDICAPMASKS

// WinUser.h
#define NOAPISET
#define NORESOURCE
#define NOSCROLL
#define NODESKTOP
#define NOWINDOWSTATION
#define NOSECURITY
#define NOTRACKMOUSEEVENT
#define NOCLIPBOARD
#define NOSYSMETRICS
#define NOMENUS
#define NODRAWTEXT
#define NOMB
#define NOCOLOR
#define NOICONS
#define NOMDI
#define NOHELP
#define NOSYSPARAMSINFO
#define NO_STATE_FLAGS

// WinNls.h
#define NONLS

// C++ STL
#include <iterator>
#include <map>
#include <unordered_map>
#include <vector>

// Windows base
#include <windows.h>
#include <dwmapi.h>

// COM / WinRT (base)
#include <unknwn.h>
#include <inspectable.h>
#include <restrictederrorinfo.h>
#include <hstring.h>

#include <DispatcherQueue.h>

// Undefine GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#undef GetCurrentTime

// WinRT
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Globalization.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

// App
#include "Services/Loggers/log_t.h"

// Windows App SDK (WinUI 3)
#include <WindowsAppSDK-VersionInfo.h>
#include <winrt/Microsoft.Graphics.Display.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Interop.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
