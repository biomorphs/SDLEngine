/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>

struct SDL_Renderer;

namespace Render
{
	class Window;
	class Texture;

	// This represents the GL context for a window
	class Device
	{
	public:
		enum CreationFlags
		{
			UseVSync = (1<<1)
		};
		Device(Window& theWindow, int flags = 0);
		~Device();
		void Present();
		SDL_Renderer* GetRenderer();
	private:
		Window& m_window;
		SDL_Renderer* m_renderer;
	};
}