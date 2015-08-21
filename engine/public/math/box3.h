/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <glm/glm.hpp>

namespace Math
{
	// A bounding box in 3 dimensions
	class Box3
	{
	public:
		Box3();
		~Box3();
		explicit Box3(const glm::vec3& min, const glm::vec3& max);
		Box3(const Box3& other);
		Box3& operator=(const Box3& other);

		glm::vec3& Min();
		glm::vec3& Max();
		const glm::vec3& Min() const;
		const glm::vec3& Max() const;

		glm::vec3 Size() const;

		bool Intersects(const Box3& other) const;

	private:
		void Validate();
		glm::vec3 m_min;
		glm::vec3 m_max;
	};
}

#include "box3.inl"