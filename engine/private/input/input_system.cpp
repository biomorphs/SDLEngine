/*
SDLEngine
Matt Hoyle
*/
#include "input_system.h"
#include "kernel/assert.h"
#include <SDL_joystick.h>
#include <SDL_gamecontroller.h>

namespace Input
{
	InputSystem::InputSystem()
		: m_controllerAxisDeadZone( 0.5f )
	{
	}

	InputSystem::~InputSystem()
	{
	}

	bool InputSystem::Tick()
	{
		UpdateControllerState();
		return true;
	}

	const ControllerRawState* InputSystem::ControllerState(uint32_t padIndex) const
	{
		SDE_ASSERT(padIndex < m_controllers.size());
		return &m_controllers[padIndex].m_padState;
	}

	void InputSystem::UpdateControllerState()
	{
		for (auto& padDesc : m_controllers)
		{
			SDL_GameController* controller = (SDL_GameController*)padDesc.m_sdlController;

			// helper macros for button / axis stuff
			#define GET_SDL_BUTTON_STATE(sdlBtn, sdeBtn)	\
				SDL_GameControllerGetButton(controller, sdlBtn) ? sdeBtn : 0
			#define GET_SDL_AXIS_STATE(sdlAxis)				\
				((float)SDL_GameControllerGetAxis(controller, sdlAxis) / 32768.0f)

			uint32_t buttonState = 0;
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_A, ControllerButtons::A);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_B, ControllerButtons::B);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_X, ControllerButtons::X);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_Y, ControllerButtons::Y);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_BACK, ControllerButtons::Back);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_GUIDE, ControllerButtons::Guide);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_START, ControllerButtons::Start);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_LEFTSTICK, ControllerButtons::LeftStick);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_RIGHTSTICK, ControllerButtons::RightStick);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_LEFTSHOULDER, ControllerButtons::LeftShoulder);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, ControllerButtons::RightShoulder);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_DPAD_UP, ControllerButtons::DPadUp);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_DPAD_DOWN, ControllerButtons::DPadDown);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_DPAD_LEFT, ControllerButtons::DPadLeft);
			buttonState |= GET_SDL_BUTTON_STATE( SDL_CONTROLLER_BUTTON_DPAD_RIGHT, ControllerButtons::DPadRight);
			padDesc.m_padState.m_buttonState = buttonState;

			// Triggers are 0 - 32k, sticks are -32k to 32k, normalised
			padDesc.m_padState.m_leftTrigger = GET_SDL_AXIS_STATE(SDL_CONTROLLER_AXIS_TRIGGERLEFT);
			padDesc.m_padState.m_rightTrigger = GET_SDL_AXIS_STATE(SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
			padDesc.m_padState.m_leftStickAxes[0] = ApplyDeadZone(GET_SDL_AXIS_STATE(SDL_CONTROLLER_AXIS_LEFTX), m_controllerAxisDeadZone);
			padDesc.m_padState.m_leftStickAxes[1] = -ApplyDeadZone(GET_SDL_AXIS_STATE(SDL_CONTROLLER_AXIS_LEFTY), m_controllerAxisDeadZone);
			padDesc.m_padState.m_rightStickAxes[0] = ApplyDeadZone(GET_SDL_AXIS_STATE(SDL_CONTROLLER_AXIS_RIGHTX), m_controllerAxisDeadZone);
			padDesc.m_padState.m_rightStickAxes[1] = -ApplyDeadZone(GET_SDL_AXIS_STATE(SDL_CONTROLLER_AXIS_RIGHTY), m_controllerAxisDeadZone);
		}
	}

	inline float InputSystem::ApplyDeadZone(float input, float deadZone) const
	{
		if (input < deadZone && input > -deadZone)
		{
			return 0.0f;
		}
		else
		{
			return input;
		}
	}

	bool InputSystem::Initialise()
	{
		EnumerateControllers();
		return true;
	}

	void InputSystem::EnumerateControllers()
	{
		for (int i = 0; i < SDL_NumJoysticks(); ++i) 
		{
			if (SDL_IsGameController(i)) 
			{
				SDL_GameController *controller = SDL_GameControllerOpen(i);
				if (controller) 
				{
					ControllerDesc newController;
					newController.m_sdlController = controller;
					memset(&newController.m_padState, 0, sizeof(newController.m_padState));
					m_controllers.push_back(newController);
				}
			}
		}
	}
}