/*
SDLEngine
Matt Hoyle
*/
#pragma once

namespace Render
{
	class Camera;
}

namespace SDE
{
	class CameraController
	{
	public:
		CameraController() { }
		virtual ~CameraController() { }
		virtual void ApplyToCamera(Render::Camera& target) = 0;
	};
}