/*
SDLEngine
Matt Hoyle
*/
#include "debug_camera_controller.h"
#include "input/controller_state.h"
#include "render/camera.h"
#include <gtx\rotate_vector.hpp>

#define PI 3.14159265358f

namespace SDE
{
	DebugCameraController::DebugCameraController()
		: m_position(0.0f, 20.0f, 0.0f)
		, m_pitch(-0.9f)
		, m_yaw(3.85f)
	{

	}

	DebugCameraController::~DebugCameraController()
	{

	}

	void DebugCameraController::ApplyToCamera(Render::Camera& target)
	{
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		target.LookAt(m_position, m_position + m_lookDirection, up);
	}

	void DebugCameraController::Update(const Input::ControllerRawState& controllerState, double timeDelta)
	{
		static float s_yawRotSpeed = 2.0f;
		static float s_pitchRotSpeed = 2.0f;
		static float s_forwardSpeed = 1.0f;
		static float s_strafeSpeed = 1.0f;
		static float s_speedMultiplier = 10.0f;
		static float s_highSpeedMultiplier = 50.0f;

		const float timeDeltaF = (float)timeDelta;
		const float xAxisRight = controllerState.m_rightStickAxes[0];
		const float yAxisRight = controllerState.m_rightStickAxes[1];
		const float xAxisLeft = controllerState.m_leftStickAxes[0];
		const float yAxisLeft = controllerState.m_leftStickAxes[1];

		float moveSpeedMulti = 1.0f + (controllerState.m_rightTrigger * s_speedMultiplier) + ((controllerState.m_leftTrigger * s_highSpeedMultiplier));

		const float yawRotation = -xAxisRight * s_yawRotSpeed * timeDeltaF;
		m_yaw += yawRotation;

		const float pitchRotation = yAxisRight * s_pitchRotSpeed * timeDeltaF;
		m_pitch += pitchRotation;

		// build direction from pitch, yaw
		glm::vec3 downZ(0.0f, 0.0f, -1.0f);
		m_lookDirection = glm::normalize(glm::rotateX(downZ, m_pitch));		
		m_lookDirection = glm::normalize(glm::rotateY(m_lookDirection, m_yaw));

		// build right + up vectors
		const glm::vec3 upY(0.0f, 1.0f, 0.0f);
		m_right = glm::cross(m_lookDirection, upY);

		// move forward
		const float forward = yAxisLeft * s_forwardSpeed  * moveSpeedMulti * timeDeltaF;
		m_position += m_lookDirection * forward;

		// strafe
		const float strafe = xAxisLeft * s_strafeSpeed * moveSpeedMulti * timeDeltaF;
		m_position += m_right * strafe;
	}
}