#pragma once

#ifdef _M_IX86
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif

#if defined(_M_IX86) || defined(_M_X64)
#define VECTORCALL __vectorcall
#else
#define VECTORCALL
#endif

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

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.System.h>
#include <DispatcherQueue.h>

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
