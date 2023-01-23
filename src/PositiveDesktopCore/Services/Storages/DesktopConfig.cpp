#include "pch.h"
#include "DesktopConfig.h"

#include "config_interop.h"

using namespace app::storage;

DesktopConfig::DesktopConfig(IConfigManager* repository)
	: repository_(repository)
	, raw_(repository_->LoadDefaultDesktop()) {
}

void DesktopConfig::store() const {
	repository_->store(raw_);
}

bool DesktopConfig::theme(theme_t value) noexcept {
	if (raw_.desktop.theme == value) {
		return false;
	}
	raw_.desktop.theme = value;

	desktop_update_t update {
		prp_theme,
		{ value },
	};
	next(update);
	return true;
}

bool DesktopConfig::backdrop(backdrop_t value) noexcept {
	if (raw_.desktop.backdrop == value) {
		return false;
	}
	raw_.desktop.backdrop = value;

	desktop_update_t update { prp_backdrop };
	update.newValue.backdrop = value;
	next(update);
	return true;
}

bool DesktopConfig::inactiveBackdrop(inactive_backdrop_t value) noexcept {
	if (raw_.desktop.inactiveBackdrop == value) {
		return false;
	}
	raw_.desktop.inactiveBackdrop = value;

	desktop_update_t update { prp_inactiveBackdrop };
	update.newValue.inactiveBackdrop = value;
	next(update);
	return true;
}

bool DesktopConfig::corner(corner_t value) noexcept {
	if (raw_.desktop.corner == value) {
		return false;
	}
	raw_.desktop.corner = value;

	desktop_update_t update { prp_corner };
	update.newValue.corner = value;
	next(update);
	return true;
}

float DesktopConfig::duration() const noexcept {
	return actualDuration(raw_.desktop.duration, kDurationDefaultFloat);
}

bool DesktopConfig::duration(float value) noexcept {
	position_t value2 { packedDuration(value) };
	if (raw_.desktop.duration == value2) {
		return false;
	}
	raw_.desktop.duration = value2;

	desktop_update_t update { prp_duration };
	update.newValue.duration = value2;
	next(update);
	return true;
}

bool DesktopConfig::positionOrigin(position_origin_t value) noexcept {
	if (raw_.desktop.positionOrigin == value) {
		return false;
	}
	raw_.desktop.positionOrigin = value;

	desktop_update_t update { prp_positionMode };
	update.newValue.positionOrigin = value;
	next(update);
	return true;
}

float DesktopConfig::positionX() const noexcept {
	return actualPosition(raw_.desktop.positionX, kPositionXDefaultFloat);
}

bool DesktopConfig::positionX(float value) noexcept {
	position_t value2 { packedPosition(value) };
	if (raw_.desktop.positionX == value2) {
		return false;
	}
	raw_.desktop.positionX = value2;

	desktop_update_t update { prp_positionX };
	update.newValue.positionX = value2;
	next(update);
	return true;
}

float DesktopConfig::positionY() const noexcept {
	return actualPosition(raw_.desktop.positionY, kPositionYDefaultFloat);
}

bool DesktopConfig::positionY(float value) noexcept {
	position_t value2 { packedPosition(value) };
	if (raw_.desktop.positionY == value2) {
		return false;
	}
	raw_.desktop.positionY = value2;

	desktop_update_t update { prp_positionY };
	update.newValue.positionY = value2;
	next(update);
	return true;
}

bool DesktopConfig::scale(scale_t value) noexcept {
	if (raw_.desktop.scale == value) {
		return false;
	}
	raw_.desktop.scale = value;

	desktop_update_t update { prp_scale };
	update.newValue.scale = value;
	next(update);
	return true;
}
