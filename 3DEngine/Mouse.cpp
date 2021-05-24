#include "Mouse.h"

#include "ModifiedWindows.h"

Mouse::Event::Event() noexcept
	:
    type(Type::INVALID),
	isLeft(false),
	isRight(false),
	x(0),
    y(0)
{}

Mouse::Event::Event(Type type, const Mouse& parent) noexcept
	:
	type(type),
	isLeft(parent.IsLeftPressed()),
	isRight(parent.IsRightPressed()),
	x(parent.GetX()),
	y(parent.GetY())
{}

bool Mouse::Event::IsValid() const noexcept
{
	return type != Type::INVALID;
}

Mouse::Event::Type Mouse::Event::GetType() const noexcept
{
	return type;
}

std::pair<int, int> Mouse::Event::GetPos() const noexcept
{
	return { x, y };
}

int Mouse::Event::GetX() const noexcept
{
	return x;
}

int Mouse::Event::GetY() const noexcept
{
	return y;
}

bool Mouse::Event::IsLeftPressed() const noexcept
{
	return isLeft;
}

bool Mouse::Event::IsRightPressed() const noexcept
{
	return isRight;
}

std::pair<int, int> Mouse::GetPos() const noexcept
{
	return { x, y };
}

int Mouse::GetX() const noexcept
{
	return x;
}

int Mouse::GetY() const noexcept
{
	return y;
}

bool Mouse::IsLeftPressed() const noexcept
{
	return isLeft;
}

bool Mouse::IsRightPressed() const noexcept
{
	return isRight;
}

Mouse::Event Mouse::Read() noexcept
{
	if (!buffer.empty()) {
		Event temp = buffer.front();
		buffer.pop();
		return temp;
	}
	return {};
}

bool Mouse::IsEmpty() const noexcept
{
	return buffer.empty();
}

void Mouse::Clear() noexcept
{
	buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int x, int y) noexcept
{
	Mouse::x = x;
	Mouse::y = y;

	buffer.push(Event(Event::Type::MOVE, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	isLeft = true;

	buffer.push(Event(Event::Type::LEFT_PRESS, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	isLeft = false;

	buffer.push(Event(Event::Type::LEFT_RELEASE, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	isRight = true;

	buffer.push(Event(Event::Type::RIGHT_PRESS, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	isRight = false;

	buffer.push(Event(Event::Type::RIGHT_RELEASE, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	buffer.push(Event(Event::Type::WHEEL_UP, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	buffer.push(Event(Event::Type::WHEEL_DOWN, *this));
	TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	// Wheel data is a not a local variable to account for a "fine resolution" mouse
	wheelDelta += delta;
	while (wheelDelta >= WHEEL_DELTA)
	{
		wheelDelta -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDelta <= WHEEL_DELTA)
	{
		wheelDelta += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > BUFFER_SIZE)
	{
		buffer.pop();
	}
}