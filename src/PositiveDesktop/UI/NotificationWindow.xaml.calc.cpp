#include "pch.h"

#include <utility>

#include <dwmapi.h>

#include <Common/Math.h>
#include <Services/Storages/config_t.h>

namespace app {

	inline int32x4_t getActiveWindowRect() {
		HWND hWnd { GetForegroundWindow() };
		winrt::check_pointer(hWnd);

		RECT rect { 0, 0, 0, 0 };
		HRESULT hr = DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(RECT));
		if (FAILED(hr)) {
			winrt::check_bool(GetWindowRect(hWnd, &rect));
		}
		return int32x4_t(rect);
	}

	inline int32x2_t getCursorPosition() {
		POINT cursor { 0, 0 };
		winrt::check_bool(GetCursorPos(&cursor));
		return int32x2_t(cursor);
	}

}

#pragma warning(push)
#pragma warning(disable: 4324)
std::pair<app::int32x2_t, app::double4> getPositionAndThickness(
	app::storage::desktop_t const& config,
	app::int32x4_t workArea,
	app::int32x4_t outerBounds,
	app::int32x2_t size,
	bool isSquareCorner) {
	app::int32x2_t position;
	app::double4 border { 1, 1, 1, 1 };

	unsigned int const modeX = config.positionX;
	unsigned int const modeY = config.positionY;

	switch (config.positionMode) {
	case app::storage::psn_window:
		app::int32x4_t windowBounds = app::getActiveWindowRect();
		if (modeY == modeX) {
			app::int32x2_t windowPosition = windowBounds.point();
			switch (modeY) {
			case 3:
				position = windowPosition;
				break;
			case 4:
				position = windowPosition + ((windowBounds.size() - size) >> 2);
				break;
			case 6:
				position = windowPosition + (((windowBounds.size() - size) * 3) >> 2);
				break;
			case 7:
				position = windowPosition + (windowBounds.size() - size);
				break;
			case 5:
			default:
				position = windowPosition + ((windowBounds.size() - size) >> 1);
				break;
			}
		} else {
			switch (modeY) {
			case 3:
				position.y = windowBounds.y;
				break;
			case 4:
				position.y = windowBounds.y + ((windowBounds.w - size.y) >> 2);
				break;
			case 6:
				position.y = windowBounds.y + (((windowBounds.w - size.y) * 3) >> 2);
				break;
			case 7:
				position.y = windowBounds.y + (windowBounds.w - size.y);
				break;
			case 5:
			default:
				position.y = windowBounds.y + ((windowBounds.w - size.y) >> 1);
				break;
			}

			switch (modeX) {
			case 3:
				position.x = windowBounds.x;
				break;
			case 4:
				position.x = windowBounds.x + ((windowBounds.z - size.x) >> 2);
				break;
			case 6:
				position.x = windowBounds.x + (((windowBounds.z - size.x) * 3) >> 2);
				break;
			case 7:
				position.x = windowBounds.x + (windowBounds.z - size.x);
				break;
			case 5:
			default:
				position.x = windowBounds.x + ((windowBounds.z - size.x) >> 1);
				break;
			}
		}
		position = clamp(position, workArea.point(), workArea.point2() - size);
		break;
	case app::storage::psn_cursor:
		app::int32x2_t cursor = app::getCursorPosition();
		if (modeY == modeX) {
			switch (modeY) {
			case 3:
				position = max(cursor - size, workArea.point());
				break;
			case 4:
				position = clamp(cursor - (size >> 2), workArea.point(), workArea.point2() - size);
				break;
			case 6:
				position = clamp(cursor - ((size * 3) >> 2), workArea.point(), workArea.point2() - size);
				break;
			case 7:
				position = min(cursor, workArea.point2() - size);
				break;
			case 5:
			default:
				position = clamp(cursor - (size >> 1), workArea.point(), workArea.point2() - size);
				break;
			}
		} else {
			switch (modeY) {
			case 3:
				position.y = std::max(cursor.y - size.y, workArea.y);
				if (isSquareCorner && position.y == workArea.y) {
					border.y = 0;
				}
				break;
			case 4:
				position.y = std::clamp(cursor.y - (size.y >> 2), workArea.y, workArea.y2() - size.y);
				if (isSquareCorner) {
					if (position.y == workArea.y) {
						border.y = 0;
					} else if (position.y == workArea.y2() - size.y) {
						border.w = 0;
					}
				}
				break;
			case 6:
				position.y = std::clamp(cursor.y - ((size.y * 3) >> 2), workArea.y, workArea.y2() - size.y);
				if (isSquareCorner) {
					if (position.y == workArea.y) {
						border.y = 0;
					} else if (position.y == workArea.y2() - size.y) {
						border.w = 0;
					}
				}
				break;
			case 7:
				position.y = std::min(cursor.y, workArea.y2() - size.y);
				if (isSquareCorner && position.y == workArea.y2() - size.y) {
					border.w = 0;
				}
				break;
			case 5:
			default:
				position.y = std::clamp(cursor.y - (size.y >> 1), workArea.y, workArea.y2() - size.y);
				if (isSquareCorner) {
					if (position.y == workArea.y) {
						border.y = 0;
					} else if (position.y == workArea.y2() - size.y) {
						border.w = 0;
					}
				}
				break;
			}

			switch (modeX) {
			case 3:
				position.x = std::max(cursor.x - size.x, workArea.x);
				if (isSquareCorner && position.x == workArea.x) {
					border.x = 0;
				}
				break;
			case 4:
				position.x = std::clamp(cursor.x - (size.x >> 2), workArea.x, workArea.x2() - size.x);
				if (isSquareCorner) {
					if (position.x == workArea.x) {
						border.x = 0;
					} else if (position.x == workArea.x2() - size.x) {
						border.z = 0;
					}
				}
				break;
			case 6:
				position.x = std::clamp(cursor.x - ((size.x * 3) >> 2), workArea.x, workArea.x2() - size.x);
				if (isSquareCorner) {
					if (position.x == workArea.x) {
						border.x = 0;
					} else if (position.x == workArea.x2() - size.x) {
						border.z = 0;
					}
				}
				break;
			case 7:
				position.x = std::min(cursor.x, workArea.x2() - size.x);
				if (isSquareCorner && position.x == workArea.x2() - size.x) {
					border.z = 0;
				}
				break;
			case 5:
			default:
				position.x = std::clamp(cursor.x - (size.x >> 1), workArea.x, workArea.x2() - size.x);
				if (isSquareCorner) {
					if (position.x == workArea.x) {
						border.x = 0;
					} else if (position.x == workArea.x2() - size.x) {
						border.z = 0;
					}
				}
				break;
			}
		}
		break;

	// WorkArea mode
	case app::storage::psn_workarea:
	default:
		if (modeY == modeX) {
			app::int32x2_t outerPosition = outerBounds.point();
			switch (modeY) {
			case 3:
				position = outerPosition;
				if (isSquareCorner) {
					border = app::double4 { 0, 0, 1, 1 };
				}
				break;
			case 4:
				position = outerPosition + ((outerBounds.size() - size) >> 2);
				break;
			case 6:
				position = outerPosition + (((outerBounds.size() - size) * 3) >> 2);
				break;
			case 7:
				position = outerPosition + (outerBounds.size() - size);
				if (isSquareCorner) {
					border = app::double4 { 1, 1, 0, 0 };
				}
				break;
			case 5:
			default:
				position = outerPosition + ((outerBounds.size() - size) >> 1);
				break;
			}
		} else {
			switch (modeY) {
			case 3:
				position.y = outerBounds.y;
				if (isSquareCorner) {
					border.y = 0;
				}
				break;
			case 4:
				position.y = outerBounds.y + ((outerBounds.w - size.y) >> 2);
				break;
			case 6:
				position.y = outerBounds.y + (((outerBounds.w - size.y) * 3) >> 2);
				break;
			case 7:
				position.y = outerBounds.y + (outerBounds.w - size.y);
				if (isSquareCorner) {
					border.w = 0;
				}
				break;
			case 5:
			default:
				position.y = outerBounds.y + ((outerBounds.w - size.y) >> 1);
				break;
			}

			switch (modeX) {
			case 3:
				position.x = outerBounds.x;
				if (isSquareCorner) {
					border.x = 0;
				}
				break;
			case 4:
				position.x = outerBounds.x + ((outerBounds.z - size.x) >> 2);
				break;
			case 6:
				position.x = outerBounds.x + (((outerBounds.z - size.x) * 3) >> 2);
				break;
			case 7:
				position.x = outerBounds.x + (outerBounds.z - size.x);
				if (isSquareCorner) {
					border.z = 0;
				}
				break;
			case 5:
			default:
				position.x = outerBounds.x + ((outerBounds.z - size.x) >> 1);
				break;
			}
		}
		
		break;
	}

	return std::make_pair(position, border);
}
#pragma warning(pop)
