#pragma once
#include "VirtualDesktopDelegate.h"

namespace app::desktop {

	struct VirtualDesktopManagerInternalDelegate14238 final: public IVirtualDesktopManagerInternalDelegate {
		VirtualDesktopManagerInternalDelegate14238(std::shared_ptr<VirtualDesktopCache> cache, IServiceProvider* serviceProvider): cache_(cache) {
			winrt::check_hresult(serviceProvider->QueryService(
				clsidVirtualDesktopManagerInternal,
				__uuidof(IVirtualDesktopManagerInternal14328),
				iface_.put_void()));
		}

		inline HRESULT MoveViewToDesktop(IUnknown* pView, IVirtualDesktopDelegate* pDesktop) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->MoveViewToDesktop(pView, reinterpret_cast<VirtualDesktopDelegate10240*>(pDesktop)->iface());
		}

		inline HRESULT GetCurrentDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop* pDesktop { nullptr };
			HRESULT hr = iface_->GetCurrentDesktop(&pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		inline HRESULT GetDesktops(IObjectArray** ppArray) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->GetDesktops(ppArray);
		}

		inline HRESULT GetAdjacentDesktop(IVirtualDesktopDelegate* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop* pDesktop { nullptr };
			HRESULT hr = iface_->GetAdjacentDesktop(reinterpret_cast<VirtualDesktopDelegate10240*>(pDesktopOrigin)->iface(), nDirection, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		inline HRESULT SwitchDesktop(IVirtualDesktopDelegate* pDesktop) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->SwitchDesktop(reinterpret_cast<VirtualDesktopDelegate10240*>(pDesktop)->iface());
		}

		inline HRESULT CreateDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop* pDesktop { nullptr };
			HRESULT hr = iface_->CreateDesktopW(&pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		HRESULT LoadDesktops() noexcept override;

	private:
		std::shared_ptr<VirtualDesktopCache> cache_;
		winrt::com_ptr<IVirtualDesktopManagerInternal14328> iface_;
	};

	struct VirtualDesktopManagerInternalDelegate18963 final: public IVirtualDesktopManagerInternalDelegate {
		VirtualDesktopManagerInternalDelegate18963(std::shared_ptr<VirtualDesktopCache> cache, IServiceProvider* serviceProvider): cache_(cache) {
			winrt::check_hresult(serviceProvider->QueryService(
				clsidVirtualDesktopManagerInternal,
				__uuidof(IVirtualDesktopManagerInternal14328_2),
				iface_.put_void()));
		}

		inline HRESULT MoveViewToDesktop(IUnknown* pView, IVirtualDesktopDelegate* pDesktop) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->MoveViewToDesktop(pView, reinterpret_cast<VirtualDesktopDelegate18963*>(pDesktop)->iface());
		}

		inline HRESULT GetCurrentDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop* pDesktop { nullptr };
			HRESULT hr = iface_->GetCurrentDesktop(&pDesktop);
			if (SUCCEEDED(hr)) {
				IVirtualDesktop2* pDesktop2 { nullptr };
				hr = pDesktop->QueryInterface(&pDesktop2);
				pDesktop->Release();
				if (SUCCEEDED(hr)) {
					hr = cache_->FromInterface(pDesktop2, ppDesktop);
				}
			}
			return hr;
		}

		inline HRESULT GetDesktops(IObjectArray** ppArray) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->GetDesktops(ppArray);
		}

		inline HRESULT GetAdjacentDesktop(IVirtualDesktopDelegate* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop* pDesktop { nullptr };
			HRESULT hr = iface_->GetAdjacentDesktop(reinterpret_cast<VirtualDesktopDelegate18963*>(pDesktopOrigin)->iface(), nDirection, &pDesktop);
			if (SUCCEEDED(hr)) {
				IVirtualDesktop2* pDesktop2 { nullptr };
				hr = pDesktop->QueryInterface(&pDesktop2);
				pDesktop->Release();
				if (SUCCEEDED(hr)) {
					hr = cache_->FromInterface(pDesktop, ppDesktop);
				}
			}
			return hr;
		}

		inline HRESULT SwitchDesktop(IVirtualDesktopDelegate* pDesktop) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->SwitchDesktop(reinterpret_cast<VirtualDesktopDelegate18963*>(pDesktop)->iface());
		}

		inline HRESULT CreateDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop* pDesktop { nullptr };
			HRESULT hr = iface_->CreateDesktopW(&pDesktop);
			if (SUCCEEDED(hr)) {
				IVirtualDesktop2* pDesktop2 { nullptr };
				hr = pDesktop->QueryInterface(&pDesktop2);
				pDesktop->Release();
				if (SUCCEEDED(hr)) {
					hr = cache_->FromInterface(pDesktop2, ppDesktop);
				}
			}
			return hr;
		}

		HRESULT LoadDesktops() noexcept override;

	private:
		std::shared_ptr<VirtualDesktopCache> cache_;
		winrt::com_ptr<IVirtualDesktopManagerInternal14328_2> iface_;
	};

	struct VirtualDesktopManagerInternalDelegate20231 final: public IVirtualDesktopManagerInternalDelegate {
		VirtualDesktopManagerInternalDelegate20231(DWORD version, std::shared_ptr<VirtualDesktopCache> cache, IServiceProvider* serviceProvider): cache_(cache) {
			if (version >= 202410000) {
				winrt::check_hresult(serviceProvider->QueryService(
					clsidVirtualDesktopManagerInternal,
					__uuidof(IVirtualDesktopManagerInternal20241),
					iface_.put_void()));
			} else {
				winrt::check_hresult(serviceProvider->QueryService(
					clsidVirtualDesktopManagerInternal,
					__uuidof(IVirtualDesktopManagerInternal20231),
					iface_.put_void()));
			}
		}

		inline HRESULT MoveViewToDesktop(IUnknown* pView, IVirtualDesktopDelegate* pDesktop) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->MoveViewToDesktop(pView, reinterpret_cast<VirtualDesktopDelegate20231*>(pDesktop)->iface());
		}

		inline HRESULT GetCurrentDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop20231* pDesktop { nullptr };
			HRESULT hr = iface_->GetCurrentDesktop(nullptr, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		inline HRESULT GetDesktops(IObjectArray** ppArray) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->GetDesktops(nullptr, ppArray);
		}

		inline HRESULT GetAdjacentDesktop(IVirtualDesktopDelegate* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop20231* pDesktop { nullptr };
			HRESULT hr = iface_->GetAdjacentDesktop(reinterpret_cast<VirtualDesktopDelegate20231*>(pDesktopOrigin)->iface(), nDirection, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		inline HRESULT SwitchDesktop(IVirtualDesktopDelegate* pDesktop) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->SwitchDesktop(nullptr, reinterpret_cast<VirtualDesktopDelegate20231*>(pDesktop)->iface());
		}

		inline HRESULT CreateDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop20231* pDesktop { nullptr };
			HRESULT hr = iface_->CreateDesktopW(nullptr, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		HRESULT LoadDesktops() noexcept override;

	private:
		std::shared_ptr<VirtualDesktopCache> cache_;
		winrt::com_ptr<IVirtualDesktopManagerInternal21313> iface_;
	};

	struct VirtualDesktopManagerInternalDelegate21313 final: public IVirtualDesktopManagerInternalDelegate {
		VirtualDesktopManagerInternalDelegate21313(DWORD version, std::shared_ptr<VirtualDesktopCache> cache, IServiceProvider* serviceProvider): cache_(cache) {
			if (version >= 213590000) {
				winrt::check_hresult(serviceProvider->QueryService(
					clsidVirtualDesktopManagerInternal,
					__uuidof(IVirtualDesktopManagerInternal21359),
					iface_.put_void()));
			} else {
				winrt::check_hresult(serviceProvider->QueryService(
					clsidVirtualDesktopManagerInternal,
					__uuidof(IVirtualDesktopManagerInternal21313),
					iface_.put_void()));
			}
		}

		inline HRESULT MoveViewToDesktop(IUnknown* pView, IVirtualDesktopDelegate* pDesktop) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->MoveViewToDesktop(pView, reinterpret_cast<VirtualDesktopDelegate20231*>(pDesktop)->iface());
		}

		inline HRESULT GetCurrentDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop20231* pDesktop { nullptr };
			HRESULT hr = iface_->GetCurrentDesktop(nullptr, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		inline HRESULT GetDesktops(IObjectArray** ppArray) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->GetDesktops(nullptr, ppArray);
		}

		inline HRESULT GetAdjacentDesktop(IVirtualDesktopDelegate* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop20231* pDesktop { nullptr };
			HRESULT hr = iface_->GetAdjacentDesktop(reinterpret_cast<VirtualDesktopDelegate20231*>(pDesktopOrigin)->iface(), nDirection, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		inline HRESULT SwitchDesktop(IVirtualDesktopDelegate* pDesktop) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->SwitchDesktop(nullptr, reinterpret_cast<VirtualDesktopDelegate20231*>(pDesktop)->iface());
		}

		inline HRESULT CreateDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop20231* pDesktop { nullptr };
			HRESULT hr = iface_->CreateDesktopW(nullptr, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		HRESULT LoadDesktops() noexcept override;

	private:
		std::shared_ptr<VirtualDesktopCache> cache_;
		winrt::com_ptr<IVirtualDesktopManagerInternal21313> iface_;
	};

	struct VirtualDesktopManagerInternalDelegate22449 final: public IVirtualDesktopManagerInternalDelegate {
		VirtualDesktopManagerInternalDelegate22449(DWORD version, std::shared_ptr<VirtualDesktopCache> cache, IServiceProvider* serviceProvider): cache_(cache) {
			if (version >= 251580000) {
				winrt::check_hresult(serviceProvider->QueryService(
					clsidVirtualDesktopManagerInternal,
					__uuidof(IVirtualDesktopManagerInternal25158),
					iface_.put_void()));
			} else {
				winrt::check_hresult(serviceProvider->QueryService(
					clsidVirtualDesktopManagerInternal,
					__uuidof(IVirtualDesktopManagerInternal22449),
					iface_.put_void()));
			}
		}

		inline HRESULT MoveViewToDesktop(IUnknown* pView, IVirtualDesktopDelegate* pDesktop) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->MoveViewToDesktop(pView, reinterpret_cast<VirtualDesktopDelegate20231*>(pDesktop)->iface());
		}

		inline HRESULT GetCurrentDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop20231* pDesktop { nullptr };
			HRESULT hr = iface_->GetCurrentDesktop(nullptr, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		inline HRESULT GetDesktops(IObjectArray** ppArray) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->GetDesktops(nullptr, ppArray);
		}

		inline HRESULT GetAdjacentDesktop(IVirtualDesktopDelegate* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop20231* pDesktop { nullptr };
			HRESULT hr = iface_->GetAdjacentDesktop(reinterpret_cast<VirtualDesktopDelegate20231*>(pDesktopOrigin)->iface(), nDirection, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		inline HRESULT SwitchDesktop(IVirtualDesktopDelegate* pDesktop) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->SwitchDesktop(nullptr, reinterpret_cast<VirtualDesktopDelegate20231*>(pDesktop)->iface());
		}

		inline HRESULT CreateDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop20231* pDesktop { nullptr };
			HRESULT hr = iface_->CreateDesktopW(nullptr, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		HRESULT LoadDesktops() noexcept override;

	private:
		std::shared_ptr<VirtualDesktopCache> cache_;
		winrt::com_ptr<IVirtualDesktopManagerInternal22449> iface_;
	};

	struct VirtualDesktopManagerInternalDelegate22621_2215 final: public IVirtualDesktopManagerInternalDelegate {
		VirtualDesktopManagerInternalDelegate22621_2215(std::shared_ptr<VirtualDesktopCache> cache, IServiceProvider* serviceProvider): cache_(cache) {
			winrt::check_hresult(serviceProvider->QueryService(
				clsidVirtualDesktopManagerInternal,
				__uuidof(IVirtualDesktopManagerInternal22621_2215),
				iface_.put_void()));
		}

		inline HRESULT MoveViewToDesktop(IUnknown* pView, IVirtualDesktopDelegate* pDesktop) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->MoveViewToDesktop(pView, reinterpret_cast<VirtualDesktopDelegate22621_2215*>(pDesktop)->iface());
		}

		inline HRESULT GetCurrentDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop22621_2215* pDesktop { nullptr };
			HRESULT hr = iface_->GetCurrentDesktop(nullptr, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		inline HRESULT GetDesktops(IObjectArray** ppArray) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->GetDesktops(nullptr, ppArray);
		}

		inline HRESULT GetAdjacentDesktop(IVirtualDesktopDelegate* pDesktopOrigin, AdjacentDesktopDirection nDirection, IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop22621_2215* pDesktop { nullptr };
			HRESULT hr = iface_->GetAdjacentDesktop(reinterpret_cast<VirtualDesktopDelegate22621_2215*>(pDesktopOrigin)->iface(), nDirection, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		inline HRESULT SwitchDesktop(IVirtualDesktopDelegate* pDesktop) const noexcept override {
			WINRT_ASSERT(iface_);
			return iface_->SwitchDesktop(nullptr, reinterpret_cast<VirtualDesktopDelegate22621_2215*>(pDesktop)->iface());
		}

		inline HRESULT CreateDesktop(IVirtualDesktopDelegate** ppDesktop) const noexcept override {
			WINRT_ASSERT(iface_);

			IVirtualDesktop22621_2215* pDesktop { nullptr };
			HRESULT hr = iface_->CreateDesktopW(nullptr, &pDesktop);
			if (SUCCEEDED(hr)) {
				hr = cache_->FromInterface(pDesktop, ppDesktop);
			}
			return hr;
		}

		HRESULT LoadDesktops() noexcept override;

	private:
		std::shared_ptr<VirtualDesktopCache> cache_;
		winrt::com_ptr<IVirtualDesktopManagerInternal22621_2215> iface_;
	};

}
