#pragma once
#include <queue>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LEFT_PRESS, LEFT_RELEASE,
			RIGHT_PRESS, RIGHT_RELEASE,
			WHEEL_UP, WHEEL_DOWN,
			MOVE, INVALID
		};
	private:
		Type type;
		bool isLeft;
		bool isRight;
		int x, y;
	public:
		Event() noexcept;
		Event(Type type, const Mouse& parent) noexcept;

		bool IsValid() const noexcept;
		Type GetType() const noexcept;
		std::pair<int, int> GetPos() const noexcept;
		int GetX() const noexcept;
		int GetY() const noexcept;
		bool IsLeftPressed() const noexcept;
		bool IsRightPressed() const noexcept;
	};

	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetX() const noexcept;
	int GetY() const noexcept;
	bool IsLeftPressed() const noexcept;
	bool IsRightPressed() const noexcept;
	Event Read() noexcept;
	bool IsEmpty() const noexcept;
	void Clear() noexcept;

private:
	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
	void TrimBuffer() noexcept;

	static constexpr unsigned int BUFFER_SIZE = 16u;
	int x;
	int y;
	bool isLeft;
	bool isRight;
	int wheelDelta;
	std::queue<Event> buffer;
};