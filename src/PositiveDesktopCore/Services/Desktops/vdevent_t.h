#pragma once

namespace app::desktop {

	enum vdevent_type_t: int {
		vde_created,
		vde_changed,
		vde_moved,
		vde_removed,
		vde_renamed,
	};

	struct vdevent_t {
		vdevent_type_t type : 3;
		int oldIndex : 29;
		int index;
		winrt::hstring name;
	};

}
