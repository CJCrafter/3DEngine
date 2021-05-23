#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			PRESS, RELEASE, INVALID
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event() noexcept;
		Event(Type type, unsigned char code) noexcept;

		bool IsPress() const noexcept;
		bool IsRelease() const noexcept;
		bool isValid() const noexcept;
		unsigned char GetCode() const noexcept;
	};

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	// Key event handler
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void ClearKey() noexcept;

	// Char event handler
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void ClearChar() noexcept;

	// Clears both queues
	void Clear() noexcept;

	// Repeat control, for when people hold down a key on their keyboard
	bool IsAutorepeat();
	void SetAutorepeat(bool autorepeat);

private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(unsigned char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

	static constexpr unsigned int KEYS = 256u;
	static constexpr unsigned int BUFFER_SIZE = 16u;
	bool autorepeat = false;
	std::bitset<KEYS> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};


