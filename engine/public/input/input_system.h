/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "controller_state.h"
#include "mouse_state.h"
#include "core/system.h"
#include <vector>

namespace Input
{
	class InputSystem : public Core::ISystem
	{
	public:
		InputSystem();
		virtual ~InputSystem();
		virtual bool Initialise();
		virtual bool Tick();

		inline uint32_t ControllerCount() const { return (uint32_t)m_controllers.size(); }
		const ControllerRawState* ControllerState(uint32_t padIndex) const;
		const MouseRawState& MouseState() const;

	private:
		void UpdateControllerState();
		void EnumerateControllers();
		float ApplyDeadZone(float input, float deadZone) const;
		void UpdateMouseState();

		struct ControllerDesc
		{
			void* m_sdlController;
			ControllerRawState m_padState;
		};
		std::vector<ControllerDesc> m_controllers;
		float m_controllerAxisDeadZone;

		MouseRawState m_mouseState;
	};
}