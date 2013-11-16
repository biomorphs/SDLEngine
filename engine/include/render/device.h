/*
SDLEngine
Matt Hoyle
*/
#pragma once

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
	private:
		Window& m_window;
		SDL_Renderer* m_renderer;
	};
}