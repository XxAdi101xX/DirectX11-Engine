#include "BaseException.h"

#include <sstream>

#include "Window.h"


BaseException::BaseException(int line, const char *fileName) noexcept : line(line), fileName(fileName)
{}

const wchar_t *BaseException::What() const noexcept
{
	std::wstringstream oss;
	oss << GetType() << std::endl
		<< ConvertUtf8ToWide(GetOriginString());
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const wchar_t *BaseException::GetType() const noexcept
{
	return L"BaseException";
}

int BaseException::GetLine() const noexcept
{
	return line;
}

const std::string &BaseException::GetFile() const noexcept
{
	return fileName;
}

std::string BaseException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << fileName << "\n"
		<< "[Line] " << line;
	return oss.str();
}