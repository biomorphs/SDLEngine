/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "box3.h"
#include <glm.hpp>

namespace Math
{
	bool RayIntersectsAAB(const glm::vec3& rayStart, const glm::vec3& rayEnd, const Math::Box3& aabb, float& tNearOut);
}

#include "intersections.inl"