/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <string>

namespace Core
{
	class ISystem;

	// This class acts as an interface to find systems
	class ISystemEnumerator
	{
	public:
		virtual ISystem* GetSystem(const std::string& systemName) = 0;
	};
}