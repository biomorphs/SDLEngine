/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/base_types.h"

namespace Input
{
	enum MouseButtons
	{
		LeftButton = (1 << 0),
		MiddleButton = (1 << 1),
		RightButton = (1 << 2)
	};

	struct MouseRawState
	{
		int32_t m_cursorX;	// position relative to window/viewport in pixels
		int32_t m_cursorY;
		uint32_t m_buttonState;	// mask of buttons
	};
}