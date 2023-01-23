#include "pch.h"
#include "VersionHelper.h"

using namespace app;

OSVERSIONINFOW VersionHelper::osver_ = { sizeof(OSVERSIONINFOW), 0, 0, 0, 0 };

void VersionHelper::init() {
#pragma warning(push)
#pragma warning(disable: 4996)
	winrt::check_bool(GetVersionExW(&osver_));
#pragma warning(pop)
	if (osver_.dwMajorVersion != 10 || osver_.dwMinorVersion != 0) {
		winrt::throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	}
}
