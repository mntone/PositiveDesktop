#pragma once

namespace app {

	[[nodiscard]] _Check_return_ inline long lceil(float f) noexcept {
		return static_cast<int>(std::ceil(f));
	}

	[[nodiscard]] _Check_return_ inline long lfloor(float f) noexcept {
		return static_cast<int>(std::floor(f));
	}

	[[nodiscard]] _Check_return_ inline long ltrunc(float f) noexcept {
		return static_cast<int>(std::trunc(f));
	}

}


#ifdef WINRT_Windows_Graphics_2_H

namespace app {

	[[nodiscard]] _Check_return_ inline winrt::Windows::Graphics::SizeInt32 asSize(winrt::Windows::Graphics::PointInt32 i) noexcept {
		return winrt::Windows::Graphics::SizeInt32 {
			i.X,
			i.Y,
		};
	}

	[[nodiscard]] _Check_return_ inline winrt::Windows::Graphics::PointInt32 asPoint(winrt::Windows::Graphics::SizeInt32 i) noexcept {
		return winrt::Windows::Graphics::PointInt32 {
			i.Width,
			i.Height,
		};
	}

	[[nodiscard]] _Check_return_ inline winrt::Windows::Graphics::PointInt32 point(winrt::Windows::Graphics::RectInt32 i) noexcept {
		return winrt::Windows::Graphics::PointInt32 {
			i.X,
			i.Y,
		};
	}


	[[nodiscard]] _Check_return_ inline winrt::Windows::Graphics::SizeInt32 size(winrt::Windows::Graphics::RectInt32 i) noexcept {
		return winrt::Windows::Graphics::SizeInt32 {
			i.Width,
			i.Height,
		};
	}

	[[nodiscard]] _Check_return_ inline winrt::Windows::Foundation::Point pointAsFloat(winrt::Windows::Graphics::RectInt32 i) noexcept {
		return winrt::Windows::Foundation::Point {
			static_cast<float>(i.Width),
			static_cast<float>(i.Height),
		};
	}

	[[nodiscard]] _Check_return_ inline winrt::Windows::Foundation::Size sizeAsFloat(winrt::Windows::Graphics::RectInt32 i) noexcept {
		return winrt::Windows::Foundation::Size {
			static_cast<float>(i.Width),
			static_cast<float>(i.Height),
		};
	}

}


[[nodiscard]] _Check_return_ inline winrt::Windows::Graphics::PointInt32 operator +(winrt::Windows::Graphics::PointInt32 a, winrt::Windows::Graphics::PointInt32 b) {
	return winrt::Windows::Graphics::PointInt32 {
		a.X + b.X,
		a.Y + b.Y,
	};
}

[[nodiscard]] _Check_return_ inline winrt::Windows::Graphics::PointInt32 operator -(winrt::Windows::Graphics::PointInt32 a, winrt::Windows::Graphics::PointInt32 b) {
	return winrt::Windows::Graphics::PointInt32 {
		a.X - b.X,
		a.Y - b.Y,
	};
}

[[nodiscard]] _Check_return_ inline winrt::Windows::Graphics::PointInt32 operator >>(winrt::Windows::Graphics::PointInt32 i, int count) {
	return winrt::Windows::Graphics::PointInt32 {
		i.X >> count,
		i.Y >> count,
	};
}

[[nodiscard]] _Check_return_ inline winrt::Windows::Graphics::PointInt32 operator +(winrt::Windows::Graphics::SizeInt32 a, winrt::Windows::Graphics::SizeInt32 b) {
	return winrt::Windows::Graphics::PointInt32 {
		a.Width + b.Width,
		a.Height + b.Height,
	};
}

[[nodiscard]] _Check_return_ inline winrt::Windows::Graphics::PointInt32 operator -(winrt::Windows::Graphics::SizeInt32 a, winrt::Windows::Graphics::SizeInt32 b) {
	return winrt::Windows::Graphics::PointInt32 {
		a.Width - b.Width,
		a.Height - b.Height,
	};
}

[[nodiscard]] _Check_return_ inline winrt::Windows::Graphics::SizeInt32 operator >>(winrt::Windows::Graphics::SizeInt32 i, int count) {
	return winrt::Windows::Graphics::SizeInt32 {
		i.Width >> count,
		i.Height >> count,
	};
}

#endif
