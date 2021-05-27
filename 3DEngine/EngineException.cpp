#include "EngineException.h"
#include <sstream>

EngineException::EngineException(int line, const char* file, const char* type, const char* msg) noexcept
	:
	line(line),
	file(file),
	type(type),
	message(msg)
{}

const char* EngineException::what() const noexcept
{
	std::ostringstream stream;
	stream << GetType() << std::endl
		<< GetOriginString();
	
	whatBuffer = stream.str();
	return whatBuffer.c_str();
}

int EngineException::GetLine() const noexcept
{
	return line;
}

const std::string& EngineException::GetFile() const noexcept
{
	return file;
}

const char* EngineException::GetType() const noexcept
{
	return typeid(this).name();
}

const char* EngineException::GetMessage() const noexcept
{
	return message;
}

std::string EngineException::GetOriginString() const noexcept
{
	std::ostringstream stream;
	stream << "[File] " << file << std::endl << "[Line] " << line;
	return stream.str();
}

