/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>
#include <string>

namespace Core
{
	// Shortname is a tiny (4 character) identifier, which can be used either as
	// a string, or as a uint32_t
	class Shortname
	{
	public:
		Shortname();
		explicit Shortname(const char* text);
		~Shortname();
	
		uint32_t Code() const;
		std::string AsString() const;
		static Shortname Unknown();

		bool operator==(const Shortname& other) const;

	private:
		uint32_t m_code;
	};
}

// Hashing function in std namespace so Shortname can be used in unordered_map
namespace std 
{
	template <>
	struct hash<Core::Shortname>
	{
		std::size_t operator()(const Core::Shortname& k) const
		{
			return k.Code();
		}
	};
}

#include "shortname.inl"