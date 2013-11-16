/*
SDLEngine
	Matt Hoyle
*/
#pragma once

struct SDL_Window;

// This is a wrapper for a render window. A device must be paired with the window
namespace Render
{
	class Window
	{
	public:
		enum CreationFlags
		{
			CreateFullscreen = (1 << 1),
			CreateFullscreenDesktop = (1 << 2),
			CreateResizable = (1 << 3),
			CreateMinimized = (1 << 4),
			CreateMaximized = (1 << 5)
		};
		class Properties
		{
			friend class Window;
		public:
			Properties(const char* title, int sizeX = - 1, int sizeY = -1, int flags = 0)
				: m_title( title )
				, m_sizeX( sizeX )
				, m_sizeY( sizeY )
				, m_flags( flags )
			{

			}
		private:
			const char* m_title;
			int m_sizeX;
			int m_sizeY;
			int m_flags;
		};

		Window(const Properties& properties);
		~Window();

		void Show();
		void Hide();

		SDL_Window* GetWindowHandle();

	private:
		SDL_Window* m_windowHandle;
	};
}