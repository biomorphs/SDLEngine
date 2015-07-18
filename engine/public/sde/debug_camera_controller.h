/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include <glm.hpp>
#include "camera_controller.h"

namespace Input
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
		void Update(const Input::ControllerRawState& controllerState, double timeDelta);
		inline void SetPosition(const glm::vec3& pos) { m_position = pos; }
		inline void SetYaw(float y) { m_yaw = y; }
		inline void SetPitch(float p) { m_pitch = p; }

	private:
		glm::vec3 m_position;
		glm::vec3 m_lookDirection;
		glm::vec3 m_right;
		float m_pitch;
		float m_yaw;
	};
}