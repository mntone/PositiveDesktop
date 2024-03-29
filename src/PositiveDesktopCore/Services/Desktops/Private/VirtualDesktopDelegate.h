#pragma once
#include "Common/VirtualDesktop.h"
#include "../VirtualDesktopCache.h"
#include "../VirtualDesktopInterface.h"

namespace app::desktop {

	struct VirtualDesktopDelegate10240 final: public IVirtualDesktopDelegate {
		inline VirtualDesktopDelegate10240(IVirtualDesktop* iface, int index) noexcept: iface_(iface), index_(index) {
			iface_->AddRef();
		}
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

		inline bool IsRemote() const override { return false; }

		constexpr IVirtualDesktop* iface() const noexcept { return iface_; }

	private:
		IVirtualDesktop* iface_;
		int index_;
	};

	struct VirtualDesktopDelegate18963 final: public IVirtualDesktopDelegate {
		inline VirtualDesktopDelegate18963(IVirtualDesktop2* iface, int index) noexcept: iface_(iface), index_(index), cachedName_(false) {
			iface_->AddRef();
		}
		VirtualDesktopDelegate18963(VirtualDesktopDelegate18963 const&) = delete;
		VirtualDesktopDelegate18963& operator=(VirtualDesktopDelegate18963 const&) = delete;
		~VirtualDesktopDelegate18963() override {
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
			winrt::check_hresult(iface_->GetName(reinterpret_cast<HSTRING*>(put_abi(name))));
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

		inline bool IsRemote() const override { return false; }

		constexpr IVirtualDesktop2* iface() const noexcept { return iface_; }

	private:
		IVirtualDesktop2* iface_;
		int index_;
		bool cachedName_;
		winrt::hstring name_;
	};

	struct VirtualDesktopDelegate20231 final: public IVirtualDesktopDelegate {
		inline VirtualDesktopDelegate20231(IVirtualDesktop20231* iface, int index) noexcept: iface_(iface), index_(index), cachedName_(false) {
			iface_->AddRef();
		}
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
			winrt::check_hresult(iface_->GetName(reinterpret_cast<HSTRING*>(put_abi(name))));
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

		inline bool IsRemote() const override { return false; }

		constexpr IVirtualDesktop20231* iface() const noexcept { return iface_; }

	private:
		IVirtualDesktop20231* iface_;
		int index_;
		bool cachedName_;
		winrt::hstring name_;
	};

	struct VirtualDesktopDelegate22621_2215 final: public IVirtualDesktopDelegate {
		inline VirtualDesktopDelegate22621_2215(IVirtualDesktop22621_2215* iface, int index) noexcept: iface_(iface), index_(index), cachedName_(false) {
			iface_->AddRef();
		}
		VirtualDesktopDelegate22621_2215(VirtualDesktopDelegate22621_2215 const&) = delete;
		VirtualDesktopDelegate22621_2215& operator=(VirtualDesktopDelegate22621_2215 const&) = delete;
		~VirtualDesktopDelegate22621_2215() override {
			DeletePointer();
		}

		inline void DeletePointer() noexcept {
			IVirtualDesktop22621_2215* iface = std::exchange(iface_, nullptr);
			if (iface) {
				iface->Release();
			}
		}
		inline void ForceCache() {
			if (!iface_ || cachedName_) return;

			winrt::hstring name;
			winrt::check_hresult(iface_->GetName(reinterpret_cast<HSTRING*>(put_abi(name))));
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

		inline bool IsRemote() const override {
			BOOL remote { FALSE };
			winrt::check_hresult(iface_->IsRemote(&remote));
			return remote != FALSE;
		}

		constexpr IVirtualDesktop22621_2215* iface() const noexcept { return iface_; }

	private:
		IVirtualDesktop22621_2215* iface_;
		int index_;
		bool cachedName_;
		winrt::hstring name_;
	};

}
