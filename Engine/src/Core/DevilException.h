#pragma once

#include <exception>
#include <string>

namespace Devil
{
	class DevilException : public std::exception
	{
	public:
		DevilException(const char* file, int line);

	public:
		virtual const char* what() const noexcept;
		virtual const char* GetType() const noexcept;
		const int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
		std::string GetOriginString() const noexcept;

	private:
		int m_Line{};
		std::string m_File{};

	protected:
		mutable std::string m_Whatbuffer{};
	};
}