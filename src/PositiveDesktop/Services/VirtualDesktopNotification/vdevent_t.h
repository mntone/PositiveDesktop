#pragma once

enum vdevent_type_t {
	vde_changed,
	vde_renamed,
};

struct vdevent_t {
	vdevent_type_t type;
	int index;
	winrt::hstring name;
};
