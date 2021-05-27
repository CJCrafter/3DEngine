#pragma once
#include <vector>

#include "EngineException.h"

class InfoException : public EngineException
{
public:
	InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
	virtual const char* what() const noexcept;
	std::string GetErrorInfo() const noexcept;
private:
	std::string info;
};