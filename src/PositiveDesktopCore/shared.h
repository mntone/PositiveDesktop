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
