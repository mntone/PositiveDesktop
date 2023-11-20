#include "pch.h"
#include "VersionHelper.h"

#include <wil/registry.h>

namespace subkeys {

	constexpr std::wstring_view CurrentVersion = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";

}

using namespace app;

bool VersionHelper::initialized_ = false;
DWORD VersionHelper::build_ = 0;
DWORD VersionHelper::revision_ = 0;

void VersionHelper::init() {
	if (initialized_) return;

	// Get OS version from Windows API.
#pragma warning(push)
#pragma warning(disable: 4996)
	OSVERSIONINFOW osver { sizeof(OSVERSIONINFOW), 0, 0, 0, 0 };
	winrt::check_bool(GetVersionExW(&osver));
#pragma warning(pop)
	if (osver.dwMajorVersion != 10 || osver.dwMinorVersion != 0) {
		winrt::throw_hresult(0x80131515 /*COR_E_NOTSUPPORTED*/);
	}
	build_ = osver.dwBuildNumber;

	// Read UBR from regisitry.
	DWORD rev { ::wil::reg::get_value_dword(HKEY_LOCAL_MACHINE, subkeys::CurrentVersion.data(), L"UBR") };
	revision_ = rev;

	initialized_ = true;
}
