#pragma once

#include <exception>
#include <string>

class BaseException : public std::exception
{
public:
	BaseException(int line, const char *fileName) noexcept;
	int GetLine() const noexcept;
	const std::string &GetFile() const noexcept;
	virtual const wchar_t *GetType() const noexcept;
	std::string GetOriginString() const noexcept;

	virtual const wchar_t *What() const noexcept;
private:
	int line;
	std::string fileName;
protected:
	mutable std::wstring whatBuffer;
};

