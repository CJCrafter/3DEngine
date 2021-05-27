#pragma once
#include "EngineException.h"

class IllegalStateException : public EngineException
{
public:
	IllegalStateException(int line, const char* file, const char* msg) noexcept;

	const char* what() const noexcept override;
};