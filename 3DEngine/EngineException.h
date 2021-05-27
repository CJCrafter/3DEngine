#pragma once
#include <exception>
#include <string>
#include <sstream>

class EngineException : public std::exception
{
public:
	EngineException(int line, const char* file, const char* type, const char* msg = "No Message Provided") noexcept;
	const char* what() const noexcept override;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	const char* GetType() const noexcept;
	const char* GetMessage() const noexcept;
	std::string GetOriginString() const noexcept;

private:
	int line;
	std::string file;
	const char* type;
	const char* message;

protected:
	mutable std::string whatBuffer;
};
