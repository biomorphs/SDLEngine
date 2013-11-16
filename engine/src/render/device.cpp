/*
SDLEngine
Matt Hoyle
*/

#include "device.h"
#include "window.h"
#include "utils.h"
#include "SDL.h"

namespace Render
{
	Device::Device(Window& theWindow, int flags)
		: m_window( theWindow )
	{
		SDL_Window* windowHandle = theWindow.GetWindowHandle();
		SDE_RENDER_ASSERT(windowHandle);

		int renderFlags = SDL_RENDERER_ACCELERATED;
		if (flags & UseVSync)
			renderFlags |= SDL_RENDERER_PRESENTVSYNC;

		m_renderer = SDL_CreateRenderer(windowHandle, -1, renderFlags);
		SDE_RENDER_ASSERT(m_renderer);
	}

	Device::~Device()
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}

	void Device::Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
		SDL_RenderClear(m_renderer);
	}

	void Device::Present()
	{
		SDL_RenderPresent(m_renderer);
	}

	SDL_Renderer* Device::GetRenderer()
	{
		return m_renderer;
	}
}