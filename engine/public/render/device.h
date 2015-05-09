/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>
#include "SDL.h"

struct SDL_Renderer;

namespace Render
{
	class Window;
	class Texture;

	// This represents the GL context for a window
	class Device
	{
	public:
		Device(Window& theWindow);
		~Device();
		void Present();
		SDL_GLContext GetGLContext();
	private:
		Window& m_window;
		SDL_GLContext m_context;
	};
}