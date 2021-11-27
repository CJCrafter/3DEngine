#pragma once

class Color
{
public:
	Color() = default;
	Color(const Color& color) noexcept;
	Color(unsigned int argb);
	Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b) noexcept;

	[[nodiscard]] unsigned int ARGB() const noexcept;

	[[nodiscard]] unsigned char A() const noexcept;
	[[nodiscard]] unsigned char R() const noexcept;
	[[nodiscard]] unsigned char G() const noexcept;
	[[nodiscard]] unsigned char B() const noexcept;
private:
	unsigned int argb;
};
