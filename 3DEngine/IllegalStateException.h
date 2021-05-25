#pragma once
#include "EngineException.h"

class IllegalStateException : public EngineException
{
	const char* msg;

public:
	IllegalStateException(int line, const char* file, const char* msg) noexcept;

	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	std::string GetMessage() const noexcept;
};