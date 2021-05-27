#pragma once
#include <string>
#include <vector>

#include "EngineException.h"
#include "ModifiedWindows.h"

class HRException : public EngineException
{
public:
	HRException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
	const char* what() const noexcept override;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorString() const noexcept;
	std::string GetErrorDescription() const noexcept;
private:
	HRESULT hr;
	std::string info;
};