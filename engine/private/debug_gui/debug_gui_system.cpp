/*
SDLEngine
Matt Hoyle
*/
#include "debug_gui_system.h"
#include "debug_gui_render.h"
#include "graph_data_buffer.h"
#include "sde/render_system.h"
#include "render/mesh.h"
#include "render/shader_binary.h"
#include "render/shader_program.h"
#include "render/material.h"
#include "render/texture_source.h"
#include "render/texture.h"
#include "input/input_system.h"
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

		// We push one early frame so we can offset rendering with 1 frame delay
		ImGui::NewFrame();

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
		const auto& mouseState = m_initParams.m_inputSystem->MouseState();
		io.MousePos = ImVec2((float)mouseState.m_cursorX, (float)mouseState.m_cursorY);
		io.MouseDown[0] = (mouseState.m_buttonState & Input::MouseButtons::LeftButton) != 0;
		io.MouseDown[1] = (mouseState.m_buttonState & Input::MouseButtons::MiddleButton) != 0;
		io.MouseDown[2] = (mouseState.m_buttonState & Input::MouseButtons::RightButton) != 0;
	}

	void DebugGuiSystem::BeginWindow(bool& windowOpen, const char* windowName)
	{
		ImGui::Begin(windowName, &windowOpen, 0);
	}

	void DebugGuiSystem::EndWindow()
	{
		ImGui::End();
	}

	void DebugGuiSystem::Text(const char* txt)
	{
		ImGui::Text(txt);
	}

	void DebugGuiSystem::Separator()
	{
		ImGui::Separator();
	}

	void DebugGuiSystem::GraphLines(const char* label, glm::vec2 size, GraphDataBuffer& buffer)
	{
		ImVec2 graphSize(size.x, size.y);
		ImGui::PlotLines("", buffer.GetValues(), buffer.ValueCount(), 0, label, FLT_MAX, FLT_MAX, graphSize);
	}

	void DebugGuiSystem::GraphHistogram(const char* label, glm::vec2 size, GraphDataBuffer& buffer)
	{
		ImVec2 graphSize(size.x, size.y);
		ImGui::PlotHistogram("", buffer.GetValues(), buffer.ValueCount(), 0, label, FLT_MAX, FLT_MAX, graphSize);
	}

	bool DebugGuiSystem::Tick()
	{
		m_renderer->RebuildMesh();		// Update UI mesh for last frame

		// Submit last frame to renderer
		auto& renderPass = m_initParams.m_renderSystem->GetPass(m_initParams.m_renderPassId);
		m_renderer->SubmitToPass(renderPass);

		// Start next frame
		UpdateImgGuiInputState();
		ImGui::NewFrame();

		ImGui::ShowMetricsWindow();

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