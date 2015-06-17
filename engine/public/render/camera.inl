/*
SDLEngine
Matt Hoyle
*/

#include <gtc\matrix_transform.hpp>

namespace Render
{
	// CTor sets some sane values
	inline Camera::Camera()
		: m_position(0.0f)
		, m_up(0.0f, 1.0f, 0.0f)
		, m_target(0.0f, 0.0f, 1.0f)
		, m_nearPlane(0.1f)
		, m_farPlane(100.0f)
		, m_fov(45.0f)
		, m_aspect(4.0f / 3.0f)
	{
		RebuildViewMatrix();
		RebuildProjectionMatrix();
	}

	inline Camera::~Camera()
	{
	}

	inline void Camera::SetProjection(float fov, float aspect, float nearPlane, float farPlane)
	{
		m_fov = fov;
		m_aspect = aspect;
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;
		RebuildProjectionMatrix();
	}

	inline void Camera::SetFOVAndAspectRatio(float fov, float aspect)
	{
		m_fov = fov;
		m_aspect = aspect;
		RebuildProjectionMatrix();
	}

	inline void Camera::SetClipPlanes(float nearPlane, float farPlane)
	{
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;
		RebuildProjectionMatrix();
	}

	inline void Camera::SetPosition(glm::vec3 pos)
	{
		m_position = pos;
		RebuildViewMatrix();
	}

	inline void Camera::SetUp(glm::vec3 up)
	{
		m_up = up;
		RebuildViewMatrix();
	}

	inline void Camera::SetTarget(glm::vec3 target)
	{
		m_target = target;
		RebuildViewMatrix();
	}

	inline void Camera::LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
	{
		m_position = pos;
		m_up = up;
		m_target = target;
		RebuildViewMatrix();
	}

	inline void Camera::LookAt(glm::vec3 target, glm::vec3 up)
	{
		m_up = up;
		m_target = target;
		RebuildViewMatrix();
	}

	inline void Camera::LookAt(glm::vec3 target)
	{
		m_target = target;
		RebuildViewMatrix();
	}

	inline void Camera::RebuildViewMatrix()
	{
		m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
	}

	inline void Camera::RebuildProjectionMatrix()
	{
		m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane);
	}
}