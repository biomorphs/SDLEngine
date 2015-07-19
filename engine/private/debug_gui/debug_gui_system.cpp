/*
SDLEngine
Matt Hoyle
*/
#include "debug_gui_system.h"
#include "debug_gui_render.h"
#include "render/mesh.h"
#include "render/shader_binary.h"
#include "render/shader_program.h"
#include "render/material.h"
#include "render/texture_source.h"
#include "render/texture.h"
#include "sde/render_system.h"
#include "kernel/log.h"
#include <imggui\imgui.h>

namespace DebugGui
{
	DebugGuiSystem::DebugGuiSystem()
	{
	}

	DebugGuiSystem::~DebugGuiSystem()
	{
	}

	bool DebugGuiSystem::Initialise()
	{
		return true;
	}

	bool DebugGuiSystem::PostInit()
	{
		float viewportWidth = (float)m_initParams.m_renderSystem->GetViewportWidth();
		float viewportHeight = (float)m_initParams.m_renderSystem->GetViewportHeight();

		// Create renderer
		m_renderer = std::make_unique<DebugGuiRender>();
		if (!m_renderer->Create(viewportWidth, viewportHeight))
		{
			return false;
		}

		return true;
	}

	void DebugGuiSystem::UpdateImgGuiInputState()
	{
		ImGuiIO& io = ImGui::GetIO();

		// Tick 
		static double s_lastTickTime = m_timer.GetSeconds();
		double thisTime = m_timer.GetSeconds();
		float thisTickTime = (float)(thisTime - s_lastTickTime);
		s_lastTickTime = thisTime;
		io.DeltaTime = thisTickTime > 0.0 ? thisTickTime : (float)(1.0f / 60.0f);

		// Setup inputs
		// TODO - pass mouse position in PIXELS
		float mouse_x = 0.0, mouse_y = 0.0;
		io.MousePos = ImVec2(mouse_x, mouse_y);   // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)

		// TODO - pass mouse buttons
		for (int i = 0; i < 3; i++)
		{
			//io.MouseDown[i] = g_MousePressed[i] || glfwGetMouseButton(g_Window, i) != 0;   
		}

		// TODO - pass mouse wheel
		io.MouseWheel = 0.0f;
	}

	bool DebugGuiSystem::Tick()
	{
		UpdateImgGuiInputState();		// Update inputs
		ImGui::NewFrame();				// Start next frame
	
		float f, clear_color[3];
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color);

		m_renderer->RebuildMesh();		// Update UI mesh for last frame

		// Submit to renderer
		auto& renderPass = m_initParams.m_renderSystem->GetPass(m_initParams.m_renderPassId);
		m_renderer->SubmitToPass(renderPass);

		return true;
	}

	void DebugGuiSystem::Shutdown()
	{
		ImGui::Shutdown(); 
		m_renderer = nullptr;
	}

	DebugGuiSystem::InitialisationParams::InitialisationParams()
		: m_inputSystem(nullptr)
		, m_renderSystem(nullptr)
	{
	}

	DebugGuiSystem::InitialisationParams::InitialisationParams(SDE::RenderSystem* renderSystem, Input::InputSystem* inputSystem, uint32_t passId)
		: m_inputSystem(inputSystem)
		, m_renderSystem(renderSystem)
		, m_renderPassId(passId)
	{
	}
}