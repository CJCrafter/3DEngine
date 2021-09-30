#include "Color.h"

Color::Color(const Color& color) noexcept
	: argb(color.argb)
{  
}

Color::Color(const unsigned int argb)
	: argb(argb)
{
}

Color::Color(const unsigned char a, const unsigned char r, const unsigned char g, const unsigned char b) noexcept
	: argb(a << 24u | r << 16u | g << 8u | b)
{
}

unsigned int Color::ARGB() const noexcept
{
	return argb;
}

unsigned char Color::A() const noexcept
{
	return argb >> 24u;
}

unsigned char Color::R() const noexcept
{
	return 0xFFu & argb >> 16;
}

unsigned char Color::G() const noexcept
{
	return 0xFFu & argb >> 8;
}

unsigned char Color::B() const noexcept
{
	return 0xFFu & argb;
}
