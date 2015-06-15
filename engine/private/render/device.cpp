/*
SDLEngine
Matt Hoyle
*/

#include "device.h"
#include "window.h"
#include "utils.h"
#include <SDL.h>
#include <glew.h>

namespace Render
{
	Device::Device(Window& theWindow)
		: m_window( theWindow )
	{
		SDL_Window* windowHandle = theWindow.GetWindowHandle();
		SDE_RENDER_ASSERT(windowHandle);

		// Request opengl 3.0 context.
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		// Always doublebuffer + 24 bit depth
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		m_context = SDL_GL_CreateContext(windowHandle);
		SDE_RENDER_ASSERT(m_context);

		SDL_GL_SetSwapInterval(1);	// Synchronise swap with monitor

		// glew initialises GL function pointers
		glewExperimental = true;		// must be set for core profile and above
		auto glewError = glewInit();
		SDE_ASSERT(glewError == GLEW_OK, "Failed to initialise glew: %s", glewGetErrorString(glewError));

		// glewInit can push INVALID_ENUM to the error stack (due to how extensions are checked)
		// pop everything off here
		auto glErrorPop = glGetError();
		while (glErrorPop == GL_INVALID_ENUM)
		{
			glErrorPop = glGetError();
		}
		SDE_RENDER_PROCESS_GL_ERRORS("Device Initialise");
	}

	Device::~Device()
	{
		SDL_GL_DeleteContext(m_context);
		m_context = nullptr;
	}

	void Device::Present()
	{
		SDL_GL_SwapWindow(m_window.GetWindowHandle());
	}

	SDL_GLContext Device::GetGLContext()
	{
		return m_context;
	}
}