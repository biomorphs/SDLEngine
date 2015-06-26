/*
SDLEngine
Matt Hoyle
*/
#include "debug_camera_controller.h"
#include "engine/controller_state.h"
#include "render/camera.h"

namespace SDE
{
	DebugCameraController::DebugCameraController()
		: m_position(0.0f)
		, m_lookAt(0.0f, 0.0f, 1.0f)
		, m_up(0.0f, 1.0f, 0.0f)
	{

	}

	DebugCameraController::~DebugCameraController()
	{

	}

	void DebugCameraController::ApplyToCamera(Render::Camera& target)
	{
		target.LookAt(m_position, m_position + m_lookAt, m_up);
	}

	void DebugCameraController::Update(const Engine::ControllerRawState& controllerState, double timeDelta)
	{
		static float s_yAxisRotSpeed = 2.0f;
		static float s_xAxisRotSpeed = 2.0f;
		static float s_xAxisMoveSpeed = 1.0f;
		static float s_zAxisMoveSpeed = 1.0f;

		const float timeDeltaF = (float)timeDelta;
		const float yAxisRight = controllerState.m_rightStickAxes[0];
		const float xAxisRight = controllerState.m_rightStickAxes[1];
		const float xAxisLeft = controllerState.m_leftStickAxes[0];
		const float zAxisLeft = controllerState.m_leftStickAxes[1];

		const float yAxisRotation = -yAxisRight * s_yAxisRotSpeed * timeDeltaF;
		const float xAxisRotation = -xAxisRight * s_xAxisRotSpeed * timeDeltaF;

		glm::mat4 rotateY = glm::rotate(glm::mat4(), yAxisRotation, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotateX = glm::rotate(glm::mat4(), xAxisRotation, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotator = rotateY * rotateX;

		m_lookAt = glm::vec3(rotator * glm::vec4(m_lookAt, 0.0f));
		
		m_lookAt = glm::normalize(m_lookAt);
		m_up = glm::normalize(m_up);

		m_position += m_lookAt * zAxisLeft * s_zAxisMoveSpeed * timeDeltaF;
	}
}