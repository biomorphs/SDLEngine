/*
SDLEngine
	Matt Hoyle
*/
#include "window.h"
#include "utils.h"
#include <SDL.h>

namespace Render
{
	Window::Window(const Properties& props)
	{
		int windowPosX = SDL_WINDOWPOS_UNDEFINED;
		int windowPosY = SDL_WINDOWPOS_UNDEFINED;
		int windowFlags = SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL;	// Always start hidden, using opengl
		
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

		m_windowHandle = SDL_CreateWindow(props.m_title.c_str(), windowPosX, windowPosY, props.m_sizeX, props.m_sizeY, windowFlags);
		SDE_RENDER_ASSERT(m_windowHandle);
	}

	Window::~Window()
	{
		SDL_DestroyWindow(m_windowHandle);
		m_windowHandle = nullptr;
	}

	void Window::Show()
	{
		SDE_RENDER_ASSERT(m_windowHandle);
		SDL_ShowWindow(m_windowHandle);
	}

	void Window::Hide()
	{
		SDE_RENDER_ASSERT(m_windowHandle);
		SDL_HideWindow(m_windowHandle);
	}

	SDL_Window* Window::GetWindowHandle() 
	{
		return m_windowHandle;
	}
}