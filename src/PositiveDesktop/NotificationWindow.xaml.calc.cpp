#include "pch.h"

#include <utility>
#include <Common/Math.h>

namespace app {

	inline int32x2_t getCursorPosition() {
		POINT cursor { 0, 0 };
		winrt::check_bool(GetCursorPos(&cursor));
		return int32x2_t(cursor);
	}

}

using namespace app;

std::pair<int32x2_t, double4> getPositionAndThickness(
	unsigned int modeX,
	unsigned int modeY,
	int32x4_t workArea,
	int32x4_t outerBounds,
	int32x2_t size,
	bool isWindows11) {
	WINRT_ASSERT(modeX < 32); // The variable "modeX" should be less than 32.
	WINRT_ASSERT(modeY < 32); // The variable "modeY" should be less than 32.

	int32x2_t position;
	double4 border { 1, 1, 1, 1 };

	// Pointer mode both X and Y
	if (modeY <= 3 && modeX <= 3) {
		int32x2_t cursor = getCursorPosition();

		// the same mode
		if (modeY == modeX) {
			switch (modeY) {
			case 1:
				position = max(cursor - size, workArea.point());
				break;
			case 3:
				position = min(cursor, workArea.point2() - size);
				break;
			case 2:
			default:
				position = clamp(cursor - (size >> 1), workArea.point(), workArea.point2() - size);
				break;
			}
		}

		// different mode
		else {
			switch (modeY) {
			case 1:
				position.y = std::max(cursor.y - size.y, workArea.y);
				if (!isWindows11 && position.y == workArea.y) {
					border.y = 0;
				}
				break;
			case 3:
				position.y = std::min(cursor.y, workArea.y2() - size.y);
				if (!isWindows11 && position.y == workArea.y2() - size.y) {
					border.w = 0;
				}
				break;
			case 2:
			default:
				position.y = std::clamp(cursor.y - (size.y >> 1), workArea.y, workArea.y2() - size.y);
				if (!isWindows11) {
					if (position.y == workArea.y) {
						border.y = 0;
					} else if (position.y == workArea.y2() - size.y) {
						border.w = 0;
					}
				}
				break;
			}

			switch (modeX) {
			case 1:
				position.x = std::max(cursor.x - size.x, workArea.x);
				if (!isWindows11 && position.x == workArea.x) {
					border.x = 0;
				}
				break;
			case 3:
				position.x = std::min(cursor.x, workArea.x2() - size.x);
				if (!isWindows11 && position.x == workArea.x2() - size.x) {
					border.z = 0;
				}
				break;
			case 2:
			default:
				position.x = std::clamp(cursor.x - (size.x >> 1), workArea.x, workArea.x2() - size.x);
				if (!isWindows11) {
					if (position.x == workArea.x) {
						border.x = 0;
					} else if (position.x == workArea.x2() - size.x) {
						border.z = 0;
					}
				}
				break;
			}
		}
	}

	// Fixed mode both X and Y
	else if (modeY == modeX) {
		int32x2_t outerPosition = outerBounds.point();
		switch (modeY) {
		case 11:
			position = outerPosition;
			if (!isWindows11) {
				border = double4 { 0, 0, 1, 1 };
			}
			break;
		case 16:
			position = outerPosition + ((outerBounds.size() - size) >> 2);
			break;
		case 21:
			position = outerPosition + ((outerBounds.size() - size) >> 1);
			break;
		case 26:
			position = outerPosition + (((outerBounds.size() - size) * 3) >> 2);
			break;
		case 31:
			position = outerPosition + (outerBounds.size() - size);
			if (!isWindows11) {
				border = double4 { 1, 1, 0, 0 };
			}
			break;
		default:
			position = outerPosition + (static_cast<float2>(outerBounds.size() - size) * (0.05f * static_cast<float>(modeY - 11))).lfloor();
			break;
		}
	} else {
		switch (modeY) {
		case 1:
		{
			int32x2_t cursor = getCursorPosition();
			position.y = std::max(cursor.y - size.y, workArea.y);
			if (!isWindows11 && position.y == workArea.y) {
				border.y = 0;
			}
			break;
		}
		case 2:
		{
			int32x2_t cursor = getCursorPosition();
			position.y = std::clamp(cursor.y - (size.y >> 1), workArea.y, workArea.y2() - size.y);
			if (!isWindows11) {
				if (position.y == workArea.y) {
					border.y = 0;
				} else if (position.y == workArea.y2() - size.y) {
					border.w = 0;
				}
			}
			break;
		}
		case 3:
		{
			int32x2_t cursor = getCursorPosition();
			position.y = std::min(cursor.y, workArea.y2() - size.y);
			if (!isWindows11 && position.y == workArea.y2() - size.y) {
				border.w = 0;
			}
			break;
		}
		case 11:
			position.y = outerBounds.y;
			if (!isWindows11) {
				border.y = 0;
			}
			break;
		case 16:
			position.y = outerBounds.y + ((outerBounds.w - size.y) >> 2);
			break;
		case 21:
			position.y = outerBounds.y + ((outerBounds.w - size.y) >> 1);
			break;
		case 26:
			position.y = outerBounds.y + (((outerBounds.w - size.y) * 3) >> 2);
			break;
		case 31:
			position.y = outerBounds.y + (outerBounds.w - size.y);
			if (!isWindows11) {
				border.w = 0;
			}
			break;
		default:
			position.y = outerBounds.y + std::lround(static_cast<float>(outerBounds.y - size.y) * 0.05f * static_cast<float>(modeY - 11));
			break;
		}

		switch (modeX) {
		case 1:
		{
			int32x2_t cursor = getCursorPosition();
			position.x = std::max(cursor.x - size.x, workArea.x);
			if (!isWindows11 && position.x == workArea.x) {
				border.x = 0;
			}
			break;
		}
		case 2:
		{
			int32x2_t cursor = getCursorPosition();
			position.x = std::clamp(cursor.x - (size.x >> 1), workArea.x, workArea.x2() - size.x);
			if (!isWindows11) {
				if (position.x == workArea.x) {
					border.x = 0;
				} else if (position.x == workArea.x2() - size.x) {
					border.z = 0;
				}
			}
			break;
		}
		case 3:
		{
			int32x2_t cursor = getCursorPosition();
			position.x = std::min(cursor.x, workArea.x2() - size.x);
			if (!isWindows11 && position.x == workArea.x2() - size.x) {
				border.z = 0;
			}
			break;
		}
		case 11:
			position.x = outerBounds.x;
			if (!isWindows11) {
				border.x = 0;
			}
			break;
		case 16:
			position.x = outerBounds.x + ((outerBounds.z - size.x) >> 2);
			break;
		case 21:
			position.x = outerBounds.x + ((outerBounds.z - size.x) >> 1);
			break;
		case 26:
			position.x = outerBounds.x + (((outerBounds.z - size.x) * 3) >> 2);
			break;
		case 31:
			position.x = outerBounds.x + (outerBounds.z - size.x);
			if (!isWindows11) {
				border.z = 0;
			}
			break;
		default:
			position.x = outerBounds.x + std::lround(static_cast<float>(outerBounds.x - size.x) * 0.05f * static_cast<float>(modeX - 11));
			break;
		}
	}

	return std::make_pair(position, border);
}
