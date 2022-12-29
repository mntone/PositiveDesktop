#pragma once

#define TEST_EXPORT

#ifdef _M_IX86
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "gtest/gtest.h"
