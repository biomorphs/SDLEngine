/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include <glm.hpp>
#include "camera_controller.h"

namespace Engine
{
	struct ControllerRawState;
}

namespace SDE
{
	class DebugCameraController : public CameraController
	{
	public:
		DebugCameraController();
		virtual ~DebugCameraController();
		virtual void ApplyToCamera(Render::Camera& target);

		void Update(const Engine::ControllerRawState& controllerState, double timeDelta);

	private:
		glm::vec3 m_position;
		glm::vec3 m_lookAt;
		glm::vec3 m_up;
	};
}