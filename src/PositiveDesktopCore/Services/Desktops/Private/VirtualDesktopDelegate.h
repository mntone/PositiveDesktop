#pragma once
#include "Common/VirtualDesktop.h"
#include "../VirtualDesktopCache.h"
#include "../VirtualDesktopInterface.h"

namespace app::desktop {

	struct VirtualDesktopDelegate10240 final: public IVirtualDesktopDelegate {
		constexpr VirtualDesktopDelegate10240(IVirtualDesktop* iface, int index) noexcept: iface_(iface), index_(index) { }
		VirtualDesktopDelegate10240(VirtualDesktopDelegate10240 const&) = delete;
		VirtualDesktopDelegate10240& operator=(VirtualDesktopDelegate10240 const&) = delete;
		~VirtualDesktopDelegate10240() override {
			DeletePointer();
		}

		inline void DeletePointer() noexcept {
			IVirtualDesktop* iface = std::exchange(iface_, nullptr);
			if (iface) {
				iface->Release();
			}
		}

		constexpr int Index() const noexcept override { return index_; }
		constexpr void Index(int value) noexcept { index_ = value; }

		inline winrt::hstring Name() override { return winrt::hstring(); }

		constexpr IVirtualDesktop* iface() const noexcept { return iface_; }

	private:
		IVirtualDesktop* iface_;
		int index_;
	};

	struct VirtualDesktopDelegate19041 final: public IVirtualDesktopDelegate {
		constexpr VirtualDesktopDelegate19041(IVirtualDesktop2* iface, int index) noexcept: iface_(iface), index_(index), cachedName_(false) { }
		VirtualDesktopDelegate19041(VirtualDesktopDelegate19041 const&) = delete;
		VirtualDesktopDelegate19041& operator=(VirtualDesktopDelegate19041 const&) = delete;
		~VirtualDesktopDelegate19041() override {
			DeletePointer();
		}

		inline void DeletePointer() noexcept {
			IVirtualDesktop2* iface = std::exchange(iface_, nullptr);
			if (iface) {
				iface->Release();
			}
		}
		inline void ForceCache() noexcept {
			if (!iface_ || cachedName_) return;

			winrt::hstring name;
			{
				HSTRING abiName;
				winrt::check_hresult(iface_->GetName(&abiName));
				winrt::attach_abi(name, abiName);
			}
			name_ = std::move(name);
			cachedName_ = true;
		}

		constexpr int Index() const noexcept override { return index_; }
		constexpr void Index(int value) noexcept { index_ = value; }

		inline winrt::hstring Name() override {
			ForceCache();
			return name_;
		}
		inline void Name(HSTRING value) {
			winrt::copy_from_abi(name_, value);
		}
		inline void Name(HSTRING&& value) noexcept {
			winrt::attach_abi(name_, value);
		}

		constexpr IVirtualDesktop2* iface() const noexcept { return iface_; }

	private:
		IVirtualDesktop2* iface_;
		int index_;
		bool cachedName_;
		winrt::hstring name_;
	};

	struct VirtualDesktopDelegate20231 final: public IVirtualDesktopDelegate {
		constexpr VirtualDesktopDelegate20231(IVirtualDesktop20231* iface, int index) noexcept: iface_(iface), index_(index), cachedName_(false) { }
		VirtualDesktopDelegate20231(VirtualDesktopDelegate20231 const&) = delete;
		VirtualDesktopDelegate20231& operator=(VirtualDesktopDelegate20231 const&) = delete;
		~VirtualDesktopDelegate20231() override {
			DeletePointer();
		}

		inline void DeletePointer() noexcept {
			IVirtualDesktop20231* iface = std::exchange(iface_, nullptr);
			if (iface) {
				iface->Release();
			}
		}
		inline void ForceCache() {
			if (!iface_ || cachedName_) return;

			winrt::hstring name;
			{
				HSTRING abiName;
				winrt::check_hresult(iface_->GetName(&abiName));
				winrt::attach_abi(name, abiName);
			}
			name_ = std::move(name);
			cachedName_ = true;
		}

		constexpr int Index() const noexcept override { return index_; }
		constexpr void Index(int value) noexcept { index_ = value; }

		inline winrt::hstring Name() override {
			ForceCache();
			return name_;
		}
		inline void Name(HSTRING value) {
			winrt::copy_from_abi(name_, value);
		}
		inline void Name(HSTRING&& value) noexcept {
			winrt::attach_abi(name_, value);
		}

		constexpr IVirtualDesktop20231* iface() const noexcept { return iface_; }

	private:
		IVirtualDesktop20231* iface_;
		int index_;
		bool cachedName_;
		winrt::hstring name_;
	};

}
