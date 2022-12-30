#pragma once
#include "DesktopServiceImpl.h"

#include "Common/RepsBase.h"
#include "UI/NotificationPresenter.h"

namespace app::desktop {

	class DesktopService final: public reps::observer_t {
	public:
		DesktopService(app::ui::INotificationPresenter* presenter);
		~DesktopService();

		void initialize(uint32_t build);

		// - Desktop actions
		void moveWindowLeft() noexcept;
		void moveWindowRight() noexcept;

	private:
		void FASTCALL on(reps::bag_t const& value) noexcept override;

	private:
		app::ui::INotificationPresenter* presenter_;
		IDesktopServiceImpl* impl_;
		void (*deleter_)(IDesktopServiceImpl*);
	};

}
