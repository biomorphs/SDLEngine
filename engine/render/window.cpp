/*
SDLEngine
	Matt Hoyle
*/
#include "window.h"
#include "kernel/assert.h"
#include "SDL.h"

namespace Render
{
	Window::Window(const Properties& props)
	{
		int windowPosX = SDL_WINDOWPOS_UNDEFINED;
		int windowPosY = SDL_WINDOWPOS_UNDEFINED;
		int windowFlags = SDL_WINDOW_HIDDEN;	// Always start hidden
		
		if (props.m_flags & CreateFullscreen)
			windowFlags |= SDL_WINDOW_FULLSCREEN;
		else if (props.m_flags & CreateFullscreenDesktop)
			windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		else
		{
			if (props.m_flags & CreateResizable)
				windowFlags |= SDL_WINDOW_RESIZABLE;

			if (props.m_flags & CreateMinimized)
				windowFlags |= SDL_WINDOW_MINIMIZED;
			else if (props.m_flags & CreateResizable)
				windowFlags |= SDL_WINDOW_MAXIMIZED;
		}

		m_windowHandle = SDL_CreateWindow(props.m_title, windowPosX, windowPosY, props.m_sizeX, props.m_sizeY, windowFlags);
		SDE_ASSERT(m_windowHandle);
	}

	Window::~Window()
	{
		SDL_DestroyWindow(m_windowHandle);
	}

	void Window::Show()
	{
		SDE_ASSERT(m_windowHandle);
		SDL_ShowWindow(m_windowHandle);
	}
	void Window::Hide()
	{
		SDE_ASSERT(m_windowHandle);
		SDL_HideWindow(m_windowHandle);
	}
}