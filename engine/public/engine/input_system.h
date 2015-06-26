/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "controller_state.h"
#include "core/system.h"
#include <vector>

namespace Engine
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

	private:
		void UpdateControllerState();
		struct ControllerDesc
		{
			void* m_sdlController;
			ControllerRawState m_padState;
		};
		void EnumerateControllers();
		float ApplyDeadZone(float input, float deadZone) const;

		std::vector<ControllerDesc> m_controllers;
		float m_controllerAxisDeadZone;
	};
}