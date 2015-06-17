/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <glm.hpp>

namespace Render
{
	// Contains view and projection parameters
	class Camera
	{
	public:
		Camera();
		~Camera();
		void SetProjection(float fov, float aspect, float nearPlane, float farPlane);
		void SetFOVAndAspectRatio(float fov, float aspect);
		void SetClipPlanes(float nearPlane, float farPlane);
		void SetPosition(glm::vec3 pos);
		void SetUp(glm::vec3 up);
		void SetTarget(glm::vec3 target);
		void LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
		void LookAt(glm::vec3 target, glm::vec3 up);
		void LookAt(glm::vec3 target);

		inline glm::vec3 Position() const			{ return m_position; }
		inline glm::vec3 Up() const					{ return m_up; }
		inline glm::vec3 Target() const				{ return m_target; }
		inline glm::mat4 ViewMatrix() const			{ return m_viewMatrix; }
		inline glm::mat4 ProjectionMatrix() const	{ return m_projectionMatrix; }
		inline float NearPlane() const				{ return m_nearPlane; }
		inline float FarPlane() const				{ return m_farPlane; }

	private:
		void RebuildViewMatrix();
		void RebuildProjectionMatrix();

		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::vec3 m_position;
		glm::vec3 m_up;
		glm::vec3 m_target;
		float m_nearPlane;
		float m_farPlane;
		float m_fov;
		float m_aspect;
	};
}

#include "camera.inl"