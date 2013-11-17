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

	// This represents the renderer for a window
	class Device
	{
	public:
		enum CreationFlags
		{
			UseVSync = (1<<1)
		};
		Device(Window& theWindow, int flags = 0);
		~Device();
		void Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void Present();
		SDL_Renderer* GetRenderer();
	private:
		Window& m_window;
		SDL_Renderer* m_renderer;
	};
}