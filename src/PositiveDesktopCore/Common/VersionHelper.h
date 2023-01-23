#pragma once

namespace app {

	struct VersionHelper final {
		static void init();

		inline static constexpr OSVERSIONINFOW version() noexcept {
			return osver_;
		}

		inline static constexpr DWORD build() noexcept {
			return osver_.dwBuildNumber;
		}

		inline static constexpr bool isWindows11() noexcept {
			return osver_.dwBuildNumber >= 22000;
		}

	private:
		static OSVERSIONINFOW osver_;
	};

}
