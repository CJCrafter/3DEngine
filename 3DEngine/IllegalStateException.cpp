#include "IllegalStateException.h"

#include <sstream>

IllegalStateException::IllegalStateException(int line, const char* file, const char* msg) noexcept
	:
	EngineException(line, file),
	msg(msg)
{}

const char* IllegalStateException::what() const noexcept
{
	std::ostringstream stream;
	
	stream << GetType() << std::endl
		<< "[Message] " << GetMessage() << std::endl
		<< GetOriginString();

	whatBuffer = stream.str();
	return whatBuffer.c_str();
}

const char* IllegalStateException::GetType() const noexcept
{
	return "Illegal State Exception";
}

std::string IllegalStateException::GetMessage() const noexcept
{
	return msg;
}
