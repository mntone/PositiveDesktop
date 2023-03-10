#pragma once
#include "Common/VirtualDesktop.h"

#include "vdevent_t.h"
#include "VirtualDesktopCache.h"
#include "VirtualDesktopInterface.h"

#include "Common/Reps.h"

namespace app::desktop {

	class DesktopService final: public IVirtualDesktopNotificationCallback, public reps::single_subject_t<vdevent_t> {
	public:
		DesktopService();
		~DesktopService();

		void initialize();

		// - Desktop actions
		void switchDesktop(int target) noexcept;
		void moveForegroundWindow(int target) noexcept;
		void moveForegroundWindowAndSwitch(int target) noexcept;

		// - Window actions
		void setTopmostToForegroundWindow() noexcept;
		void unsetTopmostToForegroundWindow() noexcept;
		void toggleTopmostToForegroundWindow() noexcept;

	private:
		HRESULT GetTargetDesktop(int target, IVirtualDesktopDelegate** ppDesktop) const noexcept;

		HRESULT __stdcall VirtualDesktopCreated(IVirtualDesktopDelegate* pDesktop) noexcept override;
		HRESULT __stdcall VirtualDesktopDestroyed(IVirtualDesktopDelegate* pDesktopDestroyed) noexcept override;
		HRESULT __stdcall VirtualDesktopMoved(IVirtualDesktopDelegate* pDesktop, int nFromIndex) noexcept override;
		HRESULT __stdcall VirtualDesktopNameChanged(IVirtualDesktopDelegate* pDesktop) noexcept override;
		HRESULT __stdcall CurrentVirtualDesktopChanged(IVirtualDesktopDelegate* pDesktopNew) noexcept override;

		void unsetTopmostAll() noexcept;

		//void FASTCALL on(reps::bag_t const& value) noexcept override;

	private:
		winrt::com_ptr<IServiceProvider> serviceProvider_;
		winrt::com_ptr<IApplicationViewCollection> applicationViewCollection_;
		std::unique_ptr<IVirtualDesktopManagerInternalDelegate> virtualDesktopManagerDelegate_;
		winrt::com_ptr<IVirtualDesktopNotificationService> virtualDesktopNotificationService_;
		DWORD cookie_;

		std::shared_ptr<VirtualDesktopCache> virtualDesktopCache_;
	};

}
