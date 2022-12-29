#pragma once

#define TEST_EXPORT

#ifdef _M_IX86
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif

#include <Windows.h>

#include "gtest/gtest.h"
