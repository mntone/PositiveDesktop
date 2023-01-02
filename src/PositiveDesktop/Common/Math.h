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

	[[nodiscard]] _Check_return_ inline int lceil(double f) noexcept {
		return static_cast<int>(std::ceil(f));
	}

	[[nodiscard]] _Check_return_ inline int lfloor(double f) noexcept {
		return static_cast<int>(std::floor(f));
	}

	[[nodiscard]] _Check_return_ inline int ltrunc(double f) noexcept {
		return static_cast<int>(std::trunc(f));
	}

}

namespace app {

	struct int32x2_t final {
		int32_t x;
		int32_t y;

		int32x2_t() = default;
		int32x2_t(int32x2_t const&) = default;
		int32x2_t(int32x2_t&&) = default;

		int32x2_t& operator=(int32x2_t const&) = default;
		int32x2_t& operator=(int32x2_t&&) = default;

		constexpr int32x2_t(int32_t s): x(s), y(s) { }
		constexpr int32x2_t(int32_t x, int32_t y) : x(x), y(y) { }

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
		constexpr int32x2_t(POINT p) : x(p.x), y(p.y) { }
		constexpr int32x2_t(POINTL p) : x(p.x), y(p.y) { }
		constexpr int32x2_t(SIZE s) : x(s.cx), y(s.cy) { }
#endif

#ifdef WINRT_Windows_Graphics_2_H
		constexpr int32x2_t(winrt::Windows::Graphics::PointInt32 v): x(v.X), y(v.Y) { }
		constexpr int32x2_t(winrt::Windows::Graphics::SizeInt32 v) : x(v.Width), y(v.Height) { }
#endif

		constexpr int32x2_t& operator+=(int32_t other) noexcept {
			x += other;
			y += other;
			return *this;
		}

		constexpr int32x2_t& operator-=(int32_t other) noexcept {
			x -= other;
			y -= other;
			return *this;
		}

		constexpr int32x2_t& operator*=(int32_t other) noexcept {
			x *= other;
			y *= other;
			return *this;
		}

		constexpr int32x2_t& operator>>=(int32_t other) noexcept {
			x >>= other;
			y >>= other;
			return *this;
		}

		constexpr int32x2_t& operator<<=(int32_t other) noexcept {
			x <<= other;
			y <<= other;
			return *this;
		}

		constexpr int32x2_t& operator&=(int32_t other) noexcept {
			x &= other;
			y &= other;
			return *this;
		}

		constexpr int32x2_t& operator|=(int32_t other) noexcept {
			x |= other;
			y |= other;
			return *this;
		}

		constexpr int32x2_t& operator^=(int32_t other) noexcept {
			x ^= other;
			y ^= other;
			return *this;
		}

		constexpr int32x2_t& operator+=(int32x2_t const& other) noexcept {
			x += other.x;
			y += other.y;
			return *this;
		}

		constexpr int32x2_t& operator-=(int32x2_t const& other) noexcept {
			x -= other.x;
			y -= other.y;
			return *this;
		}

		constexpr int32x2_t& operator*=(int32x2_t const& other) noexcept {
			x *= other.x;
			y *= other.y;
			return *this;
		}

		[[nodiscard]] constexpr int32x2_t const operator+(int32_t other) const noexcept {
			int32x2_t copy(*this);
			copy += other;
			return copy;
		}

		[[nodiscard]] constexpr int32x2_t const operator-(int32_t other) const noexcept {
			int32x2_t copy(*this);
			copy -= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x2_t const operator*(int32_t other) const noexcept {
			int32x2_t copy(*this);
			copy *= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x2_t const operator>>(int32_t other) const noexcept {
			int32x2_t copy(*this);
			copy >>= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x2_t const operator<<(int32_t other) const noexcept {
			int32x2_t copy(*this);
			copy <<= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x2_t const operator&(int32_t other) const noexcept {
			int32x2_t copy(*this);
			copy &= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x2_t const operator|(int32_t other) const noexcept {
			int32x2_t copy(*this);
			copy |= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x2_t const operator^(int32_t other) const noexcept {
			int32x2_t copy(*this);
			copy ^= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x2_t const operator+(int32x2_t const& other) const noexcept {
			int32x2_t copy(*this);
			copy += other;
			return copy;
		}

		[[nodiscard]] constexpr int32x2_t const operator-(int32x2_t const& other) const noexcept {
			int32x2_t copy(*this);
			copy -= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x2_t const operator*(int32x2_t const& other) const noexcept {
			int32x2_t copy(*this);
			copy *= other;
			return copy;
		}

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
		template<typename T, typename std::enable_if_t<std::is_same_v<T, POINT>, int> = 0> constexpr POINT* get() const noexcept { return reinterpret_cast<POINT*>(this); }

		constexpr operator POINT() const noexcept { return POINT { x, y }; }
		constexpr operator POINTL() const noexcept { return POINTL { x, y }; }
		constexpr operator SIZE() const noexcept { return SIZE { x, y }; }
#endif

#ifdef WINRT_Windows_Graphics_2_H
		constexpr operator winrt::Windows::Graphics::PointInt32() const noexcept { return winrt::Windows::Graphics::PointInt32 { x, y }; }
		constexpr operator winrt::Windows::Graphics::SizeInt32() const noexcept { return winrt::Windows::Graphics::SizeInt32 { x, y }; }
#endif
	};

	[[nodiscard]] _Check_return_ constexpr int32x2_t max(int32x2_t const& value1, int32x2_t const& value2) {
		return int32x2_t((value1.x > value2.x) ? value1.x : value2.x,
						 (value1.y > value2.y) ? value1.y : value2.y);
	}

	[[nodiscard]] _Check_return_ constexpr int32x2_t min(int32x2_t const& value1, int32x2_t const& value2) {
		return int32x2_t((value1.x < value2.x) ? value1.x : value2.x,
						 (value1.y < value2.y) ? value1.y : value2.y);
	}

	[[nodiscard]] _Check_return_ constexpr int32x2_t clamp(int32x2_t const& value, int32x2_t const& minValue, int32x2_t const& maxValue) {
		return (max)((min)(value, maxValue), minValue);
	}

	struct alignas(16) int32x4_t final {
		int32_t x;
		int32_t y;
		int32_t z;
		int32_t w;

		int32x4_t() = default;
		int32x4_t(int32x4_t const&) = default;
		int32x4_t(int32x4_t&&) = default;

		int32x4_t& operator=(int32x4_t const&) = default;
		int32x4_t& operator=(int32x4_t&&) = default;

		constexpr int32x4_t(int32_t x, int32_t y, int32_t z, int32_t w): x(x), y(y), z(z), w(w) { }

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
		constexpr explicit int32x4_t(RECT r): x(r.left), y(r.top), z(r.right - r.left), w(r.bottom - r.top) { }
		constexpr explicit int32x4_t(RECTL r): x(r.left), y(r.top), z(r.right - r.left), w(r.bottom - r.top) { }
#endif

#ifdef WINRT_Windows_Graphics_2_H
		constexpr int32x4_t(winrt::Windows::Graphics::RectInt32 v): x(v.X), y(v.Y), z(v.Width), w(v.Height) { }
#endif

		inline int32x4_t addPadding(int32_t other) noexcept {
			x += other;
			y += other;
			z -= other << 1;
			w -= other << 1;
			return *this;
		}

		inline int32x4_t addingPadding(int32_t other) const noexcept {
			int32x4_t copy(*this);
			copy.addPadding(other);
			return copy;
		}

		constexpr int32_t x2() const noexcept { return x + z; }
		constexpr int32_t y2() const noexcept { return y + w; }
		constexpr int32x2_t point() const noexcept { return int32x2_t(x, y); }
		constexpr int32x2_t point2() const noexcept { return int32x2_t(x + z, y + w); }
		constexpr int32x2_t size() const noexcept { return int32x2_t(z, w); }

		constexpr int32x4_t& operator+=(int32_t other) noexcept {
			x += other;
			y += other;
			z += other;
			w += other;
			return *this;
		}

		constexpr int32x4_t& operator-=(int32_t other) noexcept {
			x -= other;
			y -= other;
			z -= other;
			w -= other;
			return *this;
		}

		constexpr int32x4_t& operator*=(int32_t other) noexcept {
			x *= other;
			y *= other;
			z *= other;
			w *= other;
			return *this;
		}

		constexpr int32x4_t& operator>>=(int32_t other) noexcept {
			x >>= other;
			y >>= other;
			z >>= other;
			w >>= other;
			return *this;
		}

		constexpr int32x4_t& operator<<=(int32_t other) noexcept {
			x <<= other;
			y <<= other;
			z <<= other;
			w <<= other;
			return *this;
		}

		constexpr int32x4_t& operator&=(int32_t other) noexcept {
			x &= other;
			y &= other;
			z &= other;
			w &= other;
			return *this;
		}

		constexpr int32x4_t& operator|=(int32_t other) noexcept {
			x |= other;
			y |= other;
			z |= other;
			w |= other;
			return *this;
		}

		constexpr int32x4_t& operator^=(int32_t other) noexcept {
			x ^= other;
			y ^= other;
			z ^= other;
			w ^= other;
			return *this;
		}

		constexpr int32x4_t& operator+=(int32x4_t const& other) noexcept {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}

		constexpr int32x4_t& operator-=(int32x4_t const& other) noexcept {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}

		constexpr int32x4_t& operator*=(int32x4_t const& other) noexcept {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}

		constexpr int32x4_t& operator>>=(int32x4_t other) noexcept {
			x >>= other.x;
			y >>= other.y;
			z >>= other.z;
			w >>= other.w;
			return *this;
		}

		constexpr int32x4_t& operator<<=(int32x4_t other) noexcept {
			x <<= other.x;
			y <<= other.y;
			z <<= other.z;
			w <<= other.w;
			return *this;
		}

		constexpr int32x4_t& operator&=(int32x4_t other) noexcept {
			x &= other.x;
			y &= other.y;
			z &= other.z;
			w &= other.w;
			return *this;
		}

		constexpr int32x4_t& operator|=(int32x4_t other) noexcept {
			x |= other.x;
			y |= other.y;
			z |= other.z;
			w |= other.w;
			return *this;
		}

		constexpr int32x4_t& operator^=(int32x4_t other) noexcept {
			x ^= other.x;
			y ^= other.y;
			z ^= other.z;
			w ^= other.w;
			return *this;
		}

		[[nodiscard]] constexpr int32x4_t const operator+(int32x4_t other) const noexcept {
			int32x4_t copy(*this);
			copy += other;
			return copy;
		}

		[[nodiscard]] constexpr int32x4_t const operator-(int32x4_t other) const noexcept {
			int32x4_t copy(*this);
			copy -= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x4_t const operator*(int32x4_t other) const noexcept {
			int32x4_t copy(*this);
			copy *= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x4_t const operator>>(int32_t other) const noexcept {
			int32x4_t copy(*this);
			copy >>= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x4_t const operator<<(int32_t other) const noexcept {
			int32x4_t copy(*this);
			copy <<= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x4_t const operator&(int32_t other) const noexcept {
			int32x4_t copy(*this);
			copy &= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x4_t const operator|(int32_t other) const noexcept {
			int32x4_t copy(*this);
			copy |= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x4_t const operator^(int32_t other) const noexcept {
			int32x4_t copy(*this);
			copy ^= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x4_t const operator+(int32x4_t const& other) const noexcept {
			int32x4_t copy(*this);
			copy += other;
			return copy;
		}

		[[nodiscard]] constexpr int32x4_t const operator-(int32x4_t const& other) const noexcept {
			int32x4_t copy(*this);
			copy -= other;
			return copy;
		}

		[[nodiscard]] constexpr int32x4_t const operator*(int32x4_t const& other) const noexcept {
			int32x4_t copy(*this);
			copy *= other;
			return copy;
		}

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
		constexpr explicit operator RECT() const noexcept { return RECT { x, y, x + z, y + w }; }
		constexpr explicit operator RECTL() const noexcept { return RECTL { x, y, x + z, y + w }; }
#endif

#ifdef WINRT_Windows_Graphics_2_H
		constexpr operator winrt::Windows::Graphics::RectInt32() const noexcept { return winrt::Windows::Graphics::RectInt32 { x, y, z, w }; }
#endif
	};

	struct float2 final {
		float x;
		float y;

		float2() = default;
		float2(float2 const&) = default;
		float2(float2&&) = default;

		float2& operator=(float2 const&) = default;
		float2& operator=(float2&&) = default;

		constexpr float2(float s): x(s), y(s) { }
		constexpr float2(float x, float y) : x(x), y(y) { }
		explicit float2(int32x2_t v): x(static_cast<float>(v.x)), y(static_cast<float>(v.y)) { }

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
		constexpr explicit float2(POINT p): x(static_cast<float>(p.x)), y(static_cast<float>(p.y)) { }
		constexpr explicit float2(POINTL p): x(static_cast<float>(p.x)), y(static_cast<float>(p.y)) { }
		constexpr explicit float2(SIZE s): x(static_cast<float>(s.cx)), y(static_cast<float>(s.cy)) { }
#endif

#ifdef WINRT_BASE_H
		constexpr float2(winrt::Windows::Foundation::Point v): x(v.X), y(v.Y) { }
		constexpr float2(winrt::Windows::Foundation::Size v) : x(v.Width), y(v.Height) { }
#endif

#ifdef WINRT_Windows_Graphics_2_H
		constexpr explicit float2(winrt::Windows::Graphics::PointInt32 v): x(static_cast<float>(v.X)), y(static_cast<float>(v.Y)) { }
		constexpr explicit float2(winrt::Windows::Graphics::SizeInt32 v): x(static_cast<float>(v.Width)), y(static_cast<float>(v.Height)) { }
#endif

		constexpr float2 floor() const noexcept { return float2(std::floor(x), std::floor(y)); }
		constexpr float2 ceil() const noexcept { return float2(std::ceil(x), std::ceil(y)); }
		inline int32x2_t lfloor() const noexcept { return int32x2_t(app::lfloor(x), app::lfloor(y)); }
		inline int32x2_t lceil() const noexcept { return int32x2_t(app::lceil(x), app::lceil(y)); }

		constexpr float2& operator+=(float other) noexcept {
			x += other;
			y += other;
			return *this;
		}

		constexpr float2& operator-=(float other) noexcept {
			x -= other;
			y -= other;
			return *this;
		}

		constexpr float2& operator*=(float other) noexcept {
			x *= other;
			y *= other;
			return *this;
		}

		constexpr float2& operator/=(float other) noexcept {
			x /= other;
			y /= other;
			return *this;
		}

		constexpr float2& operator+=(float2 const& other) noexcept {
			x += other.x;
			y += other.y;
			return *this;
		}

		constexpr float2& operator-=(float2 const& other) noexcept {
			x -= other.x;
			y -= other.y;
			return *this;
		}

		constexpr float2& operator*=(float2 const& other) noexcept {
			x *= other.x;
			y *= other.y;
			return *this;
		}

		constexpr float2& operator/=(float2 const& other) noexcept {
			x /= other.x;
			y /= other.y;
			return *this;
		}

		[[nodiscard]] constexpr float2 const operator+(float other) const noexcept {
			float2 copy(*this);
			copy += other;
			return copy;
		}

		[[nodiscard]] constexpr float2 const operator-(float other) const noexcept {
			float2 copy(*this);
			copy -= other;
			return copy;
		}

		[[nodiscard]] constexpr float2 const operator*(float other) const noexcept {
			float2 copy(*this);
			copy *= other;
			return copy;
		}

		[[nodiscard]] constexpr float2 const operator/(float other) const noexcept {
			float2 copy(*this);
			copy /= other;
			return copy;
		}

		[[nodiscard]] constexpr float2 const operator+(float2 const& other) const noexcept {
			float2 copy(*this);
			copy += other;
			return copy;
		}

		[[nodiscard]] constexpr float2 const operator-(float2 const& other) const noexcept {
			float2 copy(*this);
			copy -= other;
			return copy;
		}

		[[nodiscard]] constexpr float2 const operator*(float2 const& other) const noexcept {
			float2 copy(*this);
			copy *= other;
			return copy;
		}

		[[nodiscard]] constexpr float2 const operator/(float2 const& other) const noexcept {
			float2 copy(*this);
			copy /= other;
			return copy;
		}

		explicit operator int32x2_t() const noexcept { return int32x2_t { std::lround(x), std::lround(y) }; }

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
		explicit operator POINT() const noexcept { return POINT { std::lround(x), std::lround(y) }; }
		explicit operator POINTL() const noexcept { return POINTL { std::lround(x), std::lround(y) }; }
		explicit operator SIZE() const noexcept { return SIZE { std::lround(x), std::lround(y) }; }
#endif

#ifdef WINRT_BASE_H
		constexpr operator winrt::Windows::Foundation::Point() const noexcept { return winrt::Windows::Foundation::Point { x, y }; }
		constexpr operator winrt::Windows::Foundation::Size() const noexcept { return winrt::Windows::Foundation::Size { x, y }; }
#endif

#ifdef WINRT_Windows_Graphics_2_H
		explicit operator winrt::Windows::Graphics::PointInt32() const noexcept { return winrt::Windows::Graphics::PointInt32 { std::lround(x), std::lround(y) }; }
		explicit operator winrt::Windows::Graphics::SizeInt32() const noexcept { return winrt::Windows::Graphics::SizeInt32 { std::lround(x), std::lround(y) }; }
#endif
	};

	[[nodiscard]] _Check_return_ constexpr float2 max(float2 const& value1, float2 const& value2) {
		return float2((value1.x > value2.x) ? value1.x : value2.x,
					  (value1.y > value2.y) ? value1.y : value2.y);
	}

	[[nodiscard]] _Check_return_ constexpr float2 min(float2 const& value1, float2 const& value2) {
		return float2((value1.x < value2.x) ? value1.x : value2.x,
					  (value1.y < value2.y) ? value1.y : value2.y);
	}

	[[nodiscard]] _Check_return_ constexpr float2 clamp(float2 const& value, float2 const& minValue, float2 const& maxValue) {
		return (max)((min)(value, maxValue), minValue);
	}

	struct alignas(16) double2 final {
		double x;
		double y;

		double2() = default;
		double2(double2 const&) = default;
		double2(double2&&) = default;

		double2& operator=(double2 const&) = default;
		double2& operator=(double2&&) = default;

		constexpr double2(double s): x(s), y(s) { }
		constexpr double2(double x, double y) : x(x), y(y) { }
		explicit double2(int32x2_t v): x(v.x), y(v.y) { }

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
		constexpr explicit double2(POINT p): x(p.x), y(p.y) { }
		constexpr explicit double2(POINTL p): x(p.x), y(p.y) { }
		constexpr explicit double2(SIZE s): x(s.cx), y(s.cy) { }
#endif

#ifdef WINRT_BASE_H
		constexpr double2(winrt::Windows::Foundation::Point v): x(v.X), y(v.Y) { }
		constexpr double2(winrt::Windows::Foundation::Size v) : x(v.Width), y(v.Height) { }
#endif

#ifdef WINRT_Windows_Graphics_2_H
		constexpr explicit double2(winrt::Windows::Graphics::PointInt32 v): x(v.X), y(v.Y) { }
		constexpr explicit double2(winrt::Windows::Graphics::SizeInt32 v): x(v.Width), y(v.Height) { }
#endif

		constexpr double2& operator+=(double other) noexcept {
			x += other;
			y += other;
			return *this;
		}

		constexpr double2& operator-=(double other) noexcept {
			x -= other;
			y -= other;
			return *this;
		}

		constexpr double2& operator*=(double other) noexcept {
			x *= other;
			y *= other;
			return *this;
		}

		constexpr double2& operator/=(double other) noexcept {
			x /= other;
			y /= other;
			return *this;
		}

		constexpr double2& operator+=(double2 const& other) noexcept {
			x += other.x;
			y += other.y;
			return *this;
		}

		constexpr double2& operator-=(double2 const& other) noexcept {
			x -= other.x;
			y -= other.y;
			return *this;
		}

		constexpr double2& operator*=(double2 const& other) noexcept {
			x *= other.x;
			y *= other.y;
			return *this;
		}

		constexpr double2& operator/=(double2 const& other) noexcept {
			x /= other.x;
			y /= other.y;
			return *this;
		}

		[[nodiscard]] constexpr double2 const operator+(double other) const noexcept {
			double2 copy(*this);
			copy += other;
			return copy;
		}

		[[nodiscard]] constexpr double2 const operator-(double other) const noexcept {
			double2 copy(*this);
			copy -= other;
			return copy;
		}

		[[nodiscard]] constexpr double2 const operator*(double other) const noexcept {
			double2 copy(*this);
			copy *= other;
			return copy;
		}

		[[nodiscard]] constexpr double2 const operator/(double other) const noexcept {
			double2 copy(*this);
			copy /= other;
			return copy;
		}

		[[nodiscard]] constexpr double2 const operator+(double2 const& other) const noexcept {
			double2 copy(*this);
			copy += other;
			return copy;
		}

		[[nodiscard]] constexpr double2 const operator-(double2 const& other) const noexcept {
			double2 copy(*this);
			copy -= other;
			return copy;
		}

		[[nodiscard]] constexpr double2 const operator*(double2 const& other) const noexcept {
			double2 copy(*this);
			copy *= other;
			return copy;
		}

		[[nodiscard]] constexpr double2 const operator/(double2 const& other) const noexcept {
			double2 copy(*this);
			copy /= other;
			return copy;
		}

		explicit operator int32x2_t() const noexcept { return int32x2_t { std::lround(x), std::lround(y) }; }

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
		explicit operator POINT() const noexcept { return POINT { std::lround(x), std::lround(y) }; }
		explicit operator POINTL() const noexcept { return POINTL { std::lround(x), std::lround(y) }; }
		explicit operator SIZE() const noexcept { return SIZE { std::lround(x), std::lround(y) }; }
#endif

#ifdef WINRT_BASE_H
		constexpr operator winrt::Windows::Foundation::Point() const noexcept { return winrt::Windows::Foundation::Point { static_cast<float>(x), static_cast<float>(y) }; }
		constexpr operator winrt::Windows::Foundation::Size() const noexcept { return winrt::Windows::Foundation::Size { static_cast<float>(x), static_cast<float>(y) }; }
#endif

#ifdef WINRT_Windows_Graphics_2_H
		explicit operator winrt::Windows::Graphics::PointInt32() const noexcept { return winrt::Windows::Graphics::PointInt32 { std::lround(x), std::lround(y) }; }
		explicit operator winrt::Windows::Graphics::SizeInt32() const noexcept { return winrt::Windows::Graphics::SizeInt32 { std::lround(x), std::lround(y) }; }
#endif
	};

	[[nodiscard]] _Check_return_ constexpr double2 max(double2 const& value1, double2 const& value2) {
		return double2((value1.x > value2.x) ? value1.x : value2.x,
					   (value1.y > value2.y) ? value1.y : value2.y);
	}

	[[nodiscard]] _Check_return_ constexpr double2 min(double2 const& value1, double2 const& value2) {
		return double2((value1.x < value2.x) ? value1.x : value2.x,
					   (value1.y < value2.y) ? value1.y : value2.y);
	}

	[[nodiscard]] _Check_return_ constexpr double2 clamp(double2 const& value, double2 const& minValue, double2 const& maxValue) {
		return (max)((min)(value, maxValue), minValue);
	}

	struct alignas(32) double4 final {
		double x; // left
		double y; // top
		double z; // width | right for thickness
		double w; // height | bottom for thickness

		double4() = default;
		double4(double4 const&) = default;
		double4(double4&&) = default;

		double4& operator=(double4 const&) = default;
		double4& operator=(double4&&) = default;

		constexpr double4(double x, double y, double z, double w): x(x), y(y), z(z), w(w) { }

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
		constexpr explicit double4(RECT r): x(r.left), y(r.top), z(r.right - r.left), w(r.bottom - r.top) { }
		constexpr explicit double4(RECTL r): x(r.left), y(r.top), z(r.right - r.left), w(r.bottom - r.top) { }
#endif

#ifdef WINRT_BASE_H
		constexpr double4(winrt::Windows::Foundation::Rect v): x(v.X), y(v.Y), z(v.Width), w(v.Height) { }
#endif

#ifdef WINRT_Windows_Graphics_2_H
		constexpr double4(winrt::Windows::Graphics::RectInt32 v): x(v.X), y(v.Y), z(v.Width), w(v.Height) { }
#endif

#ifdef WINRT_Microsoft_UI_Xaml_2_H
		constexpr double4(winrt::Microsoft::UI::Xaml::Thickness v): x(v.Left), y(v.Top), z(v.Right), w(v.Bottom) { }
#endif

		constexpr double x2() const noexcept { return x + z; }
		constexpr double y2() const noexcept { return y + w; }
		constexpr double2 point() const noexcept { return double2(x, y); }
		constexpr double2 point2() const noexcept { return double2(x + z, y + w); }
		constexpr double2 size() const noexcept { return double2(z, w); }

		constexpr double4& operator+=(double other) noexcept {
			x += other;
			y += other;
			z += other;
			w += other;
			return *this;
		}

		constexpr double4& operator-=(double other) noexcept {
			x -= other;
			y -= other;
			z -= other;
			w -= other;
			return *this;
		}

		constexpr double4& operator*=(double other) noexcept {
			x *= other;
			y *= other;
			z *= other;
			w *= other;
			return *this;
		}

		constexpr double4& operator/=(double other) noexcept {
			x /= other;
			y /= other;
			z /= other;
			w /= other;
			return *this;
		}

		constexpr double4& operator+=(double4 const& other) noexcept {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}

		constexpr double4& operator-=(double4 const& other) noexcept {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}

		constexpr double4& operator*=(double4 const& other) noexcept {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}

		constexpr double4& operator/=(double4 const& other) noexcept {
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
			return *this;
		}

		[[nodiscard]] constexpr double4 const operator+(double4 other) const noexcept {
			double4 copy(*this);
			copy += other;
			return copy;
		}

		[[nodiscard]] constexpr double4 const operator-(double4 other) const noexcept {
			double4 copy(*this);
			copy -= other;
			return copy;
		}

		[[nodiscard]] constexpr double4 const operator*(double4 other) const noexcept {
			double4 copy(*this);
			copy *= other;
			return copy;
		}

		[[nodiscard]] constexpr double4 const operator/(double4 other) const noexcept {
			double4 copy(*this);
			copy /= other;
			return copy;
		}

		[[nodiscard]] constexpr double4 const operator+(double4 const& other) const noexcept {
			double4 copy(*this);
			copy += other;
			return copy;
		}

		[[nodiscard]] constexpr double4 const operator-(double4 const& other) const noexcept {
			double4 copy(*this);
			copy -= other;
			return copy;
		}

		[[nodiscard]] constexpr double4 const operator*(double4 const& other) const noexcept {
			double4 copy(*this);
			copy *= other;
			return copy;
		}

		[[nodiscard]] constexpr double4 const operator/(double4 const& other) const noexcept {
			double4 copy(*this);
			copy /= other;
			return copy;
		}

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)
		inline explicit operator RECT() const noexcept { return RECT { lfloor(x), lfloor(y), lceil(x + z), lceil(y + w) }; }
		inline explicit operator RECTL() const noexcept { return RECTL { lfloor(x), lfloor(y), lceil(x + z), lceil(y + w) }; }
#endif

#ifdef WINRT_BASE_H
		constexpr operator winrt::Windows::Foundation::Rect() const noexcept { return winrt::Windows::Foundation::Rect { static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w) }; }
#endif

#ifdef WINRT_Windows_Graphics_2_H
		inline operator winrt::Windows::Graphics::RectInt32() const noexcept { return winrt::Windows::Graphics::RectInt32 { lfloor(x), lfloor(y), lceil(z), lceil(w) }; }
#endif

#ifdef WINRT_Microsoft_UI_Xaml_2_H
		constexpr operator winrt::Microsoft::UI::Xaml::Thickness() const noexcept { return winrt::Microsoft::UI::Xaml::Thickness { x, y, z, w }; }
#endif
	};

}
