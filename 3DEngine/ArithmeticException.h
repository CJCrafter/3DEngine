#pragma once

#include "EngineException.h"

class ArithmeticException : public EngineException 
{
public:
	ArithmeticException(int line, const char* file, const char* arithmetic = "No Arithmetic Provided", const char* msg = "No Message Provided") noexcept;
	const char* what() const noexcept override;
private:
	const char* arithmetic;
};
