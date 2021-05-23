#include "Keyboard.h"

Keyboard::Event::Event() noexcept
	:
	type(Type::INVALID),
	code(0u)
{}

Keyboard::Event::Event(Type type, unsigned char code) noexcept
	:
	type(type),
	code(code)
{}

bool Keyboard::Event::IsPress() const noexcept
{
	return type == Type::PRESS;
}

bool Keyboard::Event::IsRelease() const noexcept
{
	return type == Type::RELEASE;
}

bool Keyboard::Event::isValid() const noexcept
{
	return type != Type::INVALID;
}

unsigned char Keyboard::Event::GetCode() const noexcept
{
	return code;
}

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return keyStates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (!keyBuffer.empty())
	{
		Event e = keyBuffer.front();
		keyBuffer.pop(); // pop in c++ does not return front
		return e;
	}
	return {};
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return keyBuffer.empty();
}

void Keyboard::ClearKey() noexcept
{
	keyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept
{
	if (!charBuffer.empty())
	{
		char code = charBuffer.front();
		charBuffer.pop();
		return code;
	}
	return 0;
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return charBuffer.empty();
}

void Keyboard::ClearChar() noexcept
{
	charBuffer = std::queue<char>();
}

void Keyboard::Clear() noexcept
{
	ClearKey();
	ClearChar();
}

bool Keyboard::IsAutorepeat()
{
	return autorepeat;
}

void Keyboard::SetAutorepeat(bool autorepeat)
{
	Keyboard::autorepeat = autorepeat;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	keyStates[keycode] = true;
	keyBuffer.push(Event(Event::Type::PRESS, keycode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	keyStates[keycode] = false;
	keyBuffer.push(Event(Event::Type::RELEASE, keycode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(unsigned char character) noexcept
{
	charBuffer.push(character);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept
{
	keyStates.reset();
}

template <typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > BUFFER_SIZE)
	{
		buffer.pop();
	}
}