/*
SDLEngine
Matt Hoyle
*/
#pragma once

namespace Core
{
	class ISystem;

	// This class acts as an interface to find systems
	class ISystemEnumerator
	{
	public:
		virtual ISystem* GetSystem(const char* systemName) = 0;
	};
}