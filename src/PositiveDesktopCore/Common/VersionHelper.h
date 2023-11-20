#pragma once

namespace app {

	struct VersionHelper final {
		static void init();

		inline static constexpr DWORD version() noexcept {
			return 10000 * build_ + revision_;
		}

		inline static constexpr DWORD build() noexcept {
			return build_;
		}

		inline static constexpr DWORD revision() noexcept {
			return revision_;
		}

		inline static constexpr bool isWindows11() noexcept {
			return build_ >= 22000;
		}

	private:
		static bool initialized_;
		static DWORD build_, revision_;
	};

}
