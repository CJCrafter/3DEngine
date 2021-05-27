#include "HRException.h"

#include "dxerr.h"

HRException::HRException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	EngineException(line, file, "HRESULT Exception"),
	hr(hr)
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

const char* HRException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;

	if (!info.empty())
	{
		oss << std::endl << "[Error Info]" << std::endl << info << std::endl << std::endl;
	}

	oss << GetOriginString();
	
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

HRESULT HRException::GetErrorCode() const noexcept
{
	return hr;
}

std::string HRException::GetErrorString() const noexcept
{
	return DXGetErrorString(hr);
}

std::string HRException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}