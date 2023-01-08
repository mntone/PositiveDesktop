#pragma once
#include "shared.h"

#define WIN32_LEAN_AND_MEAN
#define WINRT_LEAN_AND_MEAN

// Unused definition
#define NOGDICAPMASKS
#define NODESKTOP
#define NOWINDOWSTATION
#define NOMDI
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NORASTEROPS
#define NOATOM
#define NOCLIPBOARD
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMB
#define NOCOLOR
#define NOMEMMGR
#define NOMETAFILE
#define NOTEXTMETRIC
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOCOMM
#define NOHELP
#define NOPROFILER
#define NOMCX
#define NOCRYPT

#include <windows.h>
#include <dwmapi.h>

#include <unknwn.h>
#include <inspectable.h>
#include <restrictederrorinfo.h>
#include <hstring.h>

// Undefine GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#undef GetCurrentTime

#include <WindowsAppSDK-VersionInfo.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <DispatcherQueue.h>

#include "Services/Loggers/log_t.h"
#include "Services/Storages/config_t.h"

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
