/*
SDLEngine
Matt Hoyle
*/
#pragma once

namespace Core
{
	class ISystemEnumerator;

	class ISystem
	{
	public:
		virtual bool PreInit(ISystemEnumerator& systemEnumerator) { return true; }
		virtual bool Initialise() { return true; }
		virtual bool PostInit() { return true; }

		virtual bool Tick() { return true; }

		virtual void PreShutdown() { }
		virtual void Shutdown() { }
		virtual void PostShutdown() { }
	};
}