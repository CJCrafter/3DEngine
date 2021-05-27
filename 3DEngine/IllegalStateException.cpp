#include "IllegalStateException.h"

IllegalStateException::IllegalStateException(int line, const char* file, const char* msg) noexcept
	:
	EngineException(line, file, "Illegal State Exception", msg)
{}
