#include "Color.h"

constexpr Color::Color() noexcept
	: argb()
{
}

constexpr Color::Color(const Color& color) noexcept
	: argb(color.argb)
{  
}

constexpr Color::Color(const unsigned int argb)
	: argb(argb)
{
}

constexpr Color::Color(const unsigned char a, const unsigned char r, const unsigned char g, const unsigned char b) noexcept
	: argb(a << 24u | r << 16u | g << 8u | b)
{
}

constexpr unsigned int Color::RGB() const noexcept
{
	return argb;
}

constexpr unsigned char Color::A() const noexcept
{
	return argb >> 24u;
}

constexpr unsigned char Color::R() const noexcept
{
	return 0xFFu & argb >> 16;
}

constexpr unsigned char Color::G() const noexcept
{
	return 0xFFu & argb >> 8;
}

constexpr unsigned char Color::B() const noexcept
{
	return 0xFFu & argb;
}
