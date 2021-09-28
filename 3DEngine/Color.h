#pragma once

class Color
{
public:
	constexpr Color() noexcept;
	constexpr Color(const Color& color) noexcept;
	constexpr Color(unsigned int argb);
	constexpr Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b) noexcept;

	constexpr unsigned int RGB() const noexcept;

	constexpr unsigned char A() const noexcept;
	constexpr unsigned char R() const noexcept;
	constexpr unsigned char G() const noexcept;
	constexpr unsigned char B() const noexcept;
private:
	unsigned int argb;
};
