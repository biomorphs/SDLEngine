/*
SDLEngine
Matt Hoyle
*/

#include "kernel/assert.h"
#include <stdlib.h>

namespace Core
{
	inline Shortname::Shortname()
		: m_code(0)
	{

	}

	inline Shortname::Shortname(const char* text)
	{
		SDE_ASSERT(strlen(text) == 4);
		union Converter
		{
			char m_data[4];
			uint32_t m_code;
		} convert;
		convert.m_data[0] = text[0];
		convert.m_data[1] = text[1];
		convert.m_data[2] = text[2];
		convert.m_data[3] = text[3];
		m_code = convert.m_code;
	}

	inline Shortname::~Shortname()
	{
	}

	inline bool Shortname::operator==(const Shortname& other) const
	{
		return m_code == other.m_code;
	}

	inline Shortname Shortname::Unknown()
	{
		return Shortname();
	}

	inline uint32_t Shortname::Code() const
	{
		return m_code;
	}

	inline std::string Shortname::AsString() const
	{
		union Converter
		{
			char m_data[4];
			uint32_t m_code;
		} convert;
		convert.m_code = m_code;

		std::string theString;
		theString.reserve(5);
		theString.push_back(convert.m_data[0]);
		theString.push_back(convert.m_data[1]);
		theString.push_back(convert.m_data[2]);
		theString.push_back(convert.m_data[3]);
		theString.push_back('\0');
		return theString;
	}
}