/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "core/system.h"
#include "core/timer.h"
#include "kernel/base_types.h"
#include <memory>

namespace Input
{
	class InputSystem;
}

namespace SDE
{
	class RenderSystem;
}

namespace DebugGui
{
	class DebugGuiRender;
	class DebugGuiSystem : public Core::ISystem
	{
	public:
		DebugGuiSystem();
		virtual ~DebugGuiSystem();

		// Pass one of these BEFORE PostInitialise is called
		struct InitialisationParams
		{
			InitialisationParams();
			InitialisationParams(SDE::RenderSystem* renderSystem, Input::InputSystem* inputSystem, uint32_t passId);
			Input::InputSystem* m_inputSystem;
			SDE::RenderSystem* m_renderSystem;
			uint32_t m_renderPassId;
		};
		void SetInitialiseParams(const InitialisationParams& p) { m_initParams = p; }

		virtual bool Initialise() override;
		virtual bool PostInit() override;
		virtual bool Tick() override;
		virtual void Shutdown() override;
		void BeginWindow(bool& windowOpen, const char* windowName);
		void EndWindow();
		void Text(const char* txt);
		void Separator();

	private:
		void UpdateImgGuiInputState();

		Core::Timer m_timer;
		InitialisationParams m_initParams;
		std::unique_ptr<DebugGuiRender> m_renderer;
	};
}