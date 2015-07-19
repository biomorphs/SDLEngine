/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <vector>
#include "core/system.h"
#include "render/render_pass.h"

namespace Render
{
	class Device;
	class Window;
	class Camera;
}

namespace SDE
{
	// Main renderer. Runs on the main thread and handles
	// collection of render primitives, and submission to GPU
	class RenderSystem : public Core::ISystem
	{
	public:
		RenderSystem();
		virtual ~RenderSystem();

		uint32_t CreatePass(std::string passName);
		Render::RenderPass& GetPass(uint32_t passId);

		inline uint32_t GetViewportWidth() { return m_initParams.m_windowWidth; }
		inline uint32_t GetViewportHeight() { return m_initParams.m_windowHeight; }

		// Pass one of these to set custom params before Initialise
		struct InitialisationParams
		{
			InitialisationParams(uint32_t w = 1280, uint32_t h = 720, bool fs = false, std::string title = "SDE")
				: m_windowWidth(w), m_windowHeight(h), m_fullscreen(fs), m_windowTitle(title)	{ }
			uint32_t m_windowWidth;
			uint32_t m_windowHeight;
			std::string m_windowTitle;
			bool m_fullscreen;
		};
		void SetInitialiseParams(const InitialisationParams& p) { m_initParams = p; }

		bool Initialise();		// Window and device are created here
		bool PostInit();		// Window made visible
		bool Tick();			// All passes are drawn here
		void PostShutdown();	// Device + window shutdown here

		inline void SetClearColour(const glm::vec4& c) { m_clearColour = c; }

	private:
		InitialisationParams m_initParams;
		glm::vec4 m_clearColour;
		std::vector<Render::RenderPass> m_passes;
		std::unique_ptr<Render::Window> m_window;
		std::unique_ptr<Render::Device> m_device;
	};

	inline Render::RenderPass& RenderSystem::GetPass(uint32_t passId)
	{
		return m_passes[passId];
	}
}