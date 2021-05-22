#include "EngineException.h"
#include <sstream>

EngineException::EngineException(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

const char* EngineException::what() const noexcept
{
	std::ostringstream stream;
	stream << GetType() << std::endl << GetOriginString();
	whatBuffer = stream.str();
	return whatBuffer.c_str();
}

const char* EngineException::GetType() const noexcept
{
	return "Engine Exception";
}

int EngineException::GetLine() const noexcept
{
	return line;
}

const std::string& EngineException::GetFile() const noexcept
{
	return file;
}

std::string EngineException::GetOriginString() const noexcept
{
	std::ostringstream stream;
	stream << "[File] " << file << std::endl << "[Line] " << line;
	return stream.str();
}

