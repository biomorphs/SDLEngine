/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>
#include "math/rect.h"

struct SDL_Renderer;

namespace Render
{
	class Window;
	class Texture;

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
		void DrawSprite(Texture& texture, const Math::Rect& source, const Math::Rect& dest, double angle, const Math::Vector2& rotOrigin);
		SDL_Renderer* GetRenderer();
	private:
		Window& m_window;
		SDL_Renderer* m_renderer;
	};
}