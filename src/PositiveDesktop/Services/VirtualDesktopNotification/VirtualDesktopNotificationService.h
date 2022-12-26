#pragma once
#include "Common/RepsBase.h"
#include "VirtualDesktopNotificationServiceImpl.h"
#include "UI/NotificationPresenter.h"

namespace app::listener {

	class VirtualDesktopNotificationService final: public reps::observer_t {
	public:
		VirtualDesktopNotificationService(app::ui::INotificationPresenter* presenter);
		~VirtualDesktopNotificationService();

		void initialize(uint32_t build);

	private:
		void FASTCALL on(reps::bag_t const& value) noexcept override;

	private:
		app::ui::INotificationPresenter* presenter_;
		IVirtualDesktopNotificationServiceImpl* impl_;
		void (*deleter_)(IVirtualDesktopNotificationServiceImpl*);
	};

}
