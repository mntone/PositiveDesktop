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
#include <vector>

// Windows base
#include <Windows.h>

// COM / WinRT (base)
#include <unknwn.h>
#include <inspectable.h>
#include <restrictederrorinfo.h>
#include <hstring.h>

// WinRT
#include <winrt/base.h>

// App
#include "Services/Loggers/log_t.h"
