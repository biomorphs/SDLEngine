/*
SDLEngine
Matt Hoyle
*/

#include "kernel/assert.h"

namespace Math
{
	inline Box3::Box3()
		: m_min(0.0f)
		, m_max(0.0f)
	{
	}

	inline Box3::~Box3()
	{
	}

	inline Box3::Box3(const glm::vec3& min, const glm::vec3& max)
	{
		m_min = min;
		m_max = max;
	}

	inline Box3::Box3(const Box3& other)
	{
		*this = other;
	}

	inline Box3& Box3::operator=(const Box3& other)
	{
		if (this != &other)
		{
			m_min = other.m_min;
			m_max = other.m_max;
		}
		return *this;
	}

	inline glm::vec3& Box3::Min()
	{
		return m_min;
	}

	inline glm::vec3& Box3::Max()
	{
		return m_max;
	}

	inline const glm::vec3& Box3::Min() const
	{
		return m_min;
	}

	inline const glm::vec3& Box3::Max() const
	{
		return m_max;
	}

	inline void Box3::Validate()
	{
		SDE_ASSERT(m_min.x <= m_max.x && m_min.y <= m_max.y && m_min.z <= m_max.z);
	}

	inline glm::vec3 Box3::Size() const
	{
		return m_max - m_min;
	}

	inline bool Box3::Intersects(const Box3& other) const
	{
		return !(glm::any(glm::greaterThan(m_min, other.m_max)) || glm::any(glm::greaterThan(other.m_min, m_max)));
	}
}