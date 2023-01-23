#pragma once

#include "config_t.h"

#include <cmath>

namespace app::storage {

	constexpr float actualDuration(unsigned int packedDuration, float parentDuration = 3.0f) noexcept {
		if (packedDuration >= 3) {
			return 0.5f * static_cast<float>(packedDuration - 3);
		} else {
			switch (packedDuration) {
			case 1: return 0.25f;
			case 2: return 0.75f;
			default: return parentDuration;
			}
		}
	}

	constexpr unsigned int packedDuration(float actualDuration) noexcept {
		if (0.125 < actualDuration && actualDuration < 0.375) {
			return 1;
		} else if (0.625 < actualDuration && actualDuration < 0.875) {
			return 2;
		} else {
			return std::min(63u, static_cast<unsigned int>(std::lround(2.0f * actualDuration)) + 3u);
		}
	}

	constexpr float actualPosition(unsigned int packedPosition, float parentPosition = 0) noexcept {
		if (packedPosition >= 3) {
			return 0.5f * static_cast<float>(packedPosition - 3) - 1.f;
		} else {
			return parentPosition;
		}
	}

	constexpr unsigned int packedPosition(float actualPosition) noexcept {
		return std::min(7u, static_cast<unsigned int>(std::lround(2.f * (1.f + actualPosition))) + 3u);
	}

	constexpr float actualScale(unsigned int packedScale, float parentScale = 1.0) noexcept {
		if (packedScale != 0) {
			return 0.8f + 0.05f * static_cast<float>(packedScale - 1);
		} else {
			return parentScale;
		}
	}

	constexpr unsigned int packedScale(float actualScale) noexcept {
		return std::min(15u, static_cast<unsigned int>(std::lround(20.f * (actualScale - 0.8f))) + 1u);
	}

}
