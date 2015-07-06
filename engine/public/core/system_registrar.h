/*
SDLEngine
Matt Hoyle
*/

#pragma once

#include <string>

namespace Core
{
	class ISystem;

	// This class acts as an interface allowing external apps to register systems with the engine
	class ISystemRegistrar
	{
	public:
		virtual void RegisterSystem(const char* systemName, ISystem* theSystem) = 0;
	};
}