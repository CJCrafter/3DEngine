#pragma once

class Color
{
public:
	Color() = default;
	Color(const Color& color) noexcept;
	Color(unsigned int argb);
	Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b) noexcept;

	unsigned int ARGB() const noexcept;

	unsigned char A() const noexcept;
	unsigned char R() const noexcept;
	unsigned char G() const noexcept;
	unsigned char B() const noexcept;
private:
	unsigned int argb;
};
