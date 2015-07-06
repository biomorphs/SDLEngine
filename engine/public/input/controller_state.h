/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/base_types.h"

namespace Input
{
	enum ControllerButtons
	{
		A = (1 << 0), B = (1 << 1), X = (1 << 2), Y = (1 << 3),
		Back = (1 << 4), Guide = (1 << 5), Start = (1 << 6),
		LeftStick = (1 << 7), RightStick = (1 << 8),
		LeftShoulder = (1 << 9), RightShoulder = (1 << 10),
		DPadUp = (1 << 11), DPadDown = (1 << 12), DPadLeft = (1 << 13), DPadRight = (1 << 14)
	};

	struct ControllerRawState
	{
		uint32_t m_buttonState;		// 1 bit/button, use Button enum to access
		float m_leftStickAxes[2];	// x, y, -1 to 1
		float m_rightStickAxes[2];	// x, y, -1 to 1
		float m_leftTrigger;		// 0 to 1
		float m_rightTrigger;		// 0 to 1
	};
}