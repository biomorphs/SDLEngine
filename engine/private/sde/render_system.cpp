/*
SDLEngine
Matt Hoyle
*/
#include "render_system.h"
#include "kernel/assert.h"
#include "render/window.h"
#include "render/device.h"

namespace SDE
{
	RenderSystem::RenderSystem()
		: m_clearColour(0.0f, 0.0f, 0.0f, 1.0f)
	{
	}

	RenderSystem::~RenderSystem()
	{
	}

	uint32_t RenderSystem::CreatePass(std::string passName)
	{
		m_passes.push_back(Render::RenderPass());
		return (uint32_t)(m_passes.size() - 1);
	}

	bool RenderSystem::Initialise()
	{
		Render::Window::Properties winProps(m_initParams.m_windowTitle, m_initParams.m_windowWidth, m_initParams.m_windowHeight);
		winProps.m_flags = m_initParams.m_fullscreen ? Render::Window::CreateFullscreen : 0;
		m_window = std::make_unique<Render::Window>(winProps);
		if (!m_window)
		{
			SDE_LOGC(SDE, "Failed to create window");
			return false;
		}

		m_device = std::make_unique<Render::Device>(*m_window);
		if (!m_device)
		{
			SDE_LOGC(SDE, "Failed to create render device");
			return false;
		}

		return true;
	}

	bool RenderSystem::PostInit()
	{
		m_window->Show();
		return true;
	}

	bool RenderSystem::Tick()
	{
		m_device->ClearColourDepthTarget(m_clearColour);

		for (auto& renderPass : m_passes)
		{
			renderPass.RenderAll(*m_device);
			renderPass.Reset();
		}

		m_device->Present();

		return true;
	}

	void RenderSystem::PostShutdown()
	{
		m_passes.clear();
		m_device = nullptr;
		m_window = nullptr;
	}
}