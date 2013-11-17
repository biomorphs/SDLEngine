/*
SDLEngine
Matt Hoyle
*/

#pragma once

namespace Core
{
	class ISystem;

	class ISystemRegistrar
	{
	public:
		virtual void RegisterSystem(const char* systemName, ISystem* theSystem) = 0;
	};
}