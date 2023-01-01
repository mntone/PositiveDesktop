#pragma once

#include <numeric>
#include <string>

inline void OutputDebugStringFVW(const wchar_t* format, va_list args) {
	int len = _vscwprintf(format, args) + 1;
	std::wstring buffer;
	buffer.resize(len);
	vswprintf_s(buffer.data(), len, format, args);
	OutputDebugStringW(buffer.c_str());
}

inline void OutputDebugStringFW(const wchar_t* format, ...) {
	va_list args;
	va_start(args, format);
	OutputDebugStringFVW(format, args);
	va_end(args);
}

inline void OutputCurrentStatus() {
	ULONG_PTR lowLimit, highLimit;
	GetCurrentThreadStackLimits(&lowLimit, &highLimit);
	auto stackSize = highLimit - lowLimit;
	OutputDebugStringFW(L"# Current Stack Size: %d\n", stackSize);

	//auto heaps = GetProcessHeaps(0, nullptr);
	//if (0 == heaps) {
	//	// err
	//}
	//std::vector<HANDLE> hHeaps;
	//hHeaps.resize(heaps);
	//heaps = GetProcessHeaps(heaps, hHeaps.data());
	//if (0 == heaps) {
	//	// err
	//}
	//if (hHeaps.size() < heaps) {
	//	// err
	//}
	//HeapSize()
	//auto heapSize = std::accumulate(std::begin(heapHandles), std::end(heapHandles), 0);

	//OutputDebugStringFW(L"# Current Stack Size: %d\n# Current Heap Size: %d\n", stackSize, heapSize);
}
