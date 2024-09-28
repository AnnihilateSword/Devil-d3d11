#include "EnginePCH.h"

#include "DevilException.h"

namespace Devil
{
	DevilException::DevilException(const char* file, int line)
		: m_File{ file }, m_Line{ line }
	{
	}

	const char* DevilException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< GetOriginString();
		m_Whatbuffer = oss.str();
		return m_Whatbuffer.c_str();
	}

	const char* DevilException::GetType() const noexcept
	{
		return "Devil Exception";
	}

	const int DevilException::GetLine() const noexcept
	{
		return m_Line;
	}

	const std::string& DevilException::GetFile() const noexcept
	{
		return m_File;
	}

	std::string DevilException::GetOriginString() const noexcept
	{
		std::ostringstream oss;
		oss << "[File]: " << m_File << std::endl
			<< "[Line]: " << m_Line;
		return oss.str();
	}
}