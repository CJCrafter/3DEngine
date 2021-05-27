#include "InfoException.h"

InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	EngineException(line, file, "Info Exception")
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::string InfoException::GetErrorInfo() const noexcept
{
	return info;
}