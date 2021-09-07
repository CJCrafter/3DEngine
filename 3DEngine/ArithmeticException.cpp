#include "ArithmeticException.h"
#include <sstream>

ArithmeticException::ArithmeticException(int line, const char* file, const char* arithmetic, const char* msg) noexcept
	:
	EngineException(line, file, "ArithmeticException", msg),
	arithmetic(arithmetic)
{}

const char* ArithmeticException::what() const noexcept
{
	std::ostringstream stream;
	stream << GetType() << std::endl
		<< "[Arithmetic] " << arithmetic << std::endl
		<< "[Message] " << GetMessage() << std::endl;

	stream << GetOriginString();

	whatBuffer = stream.str();
	return whatBuffer.c_str();
}
