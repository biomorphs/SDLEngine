/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include <glm.hpp>

// Implementation of the 3d-DDA algorithm described in 
// "Efficient implementation of the 3D-DDA ray traversal algorithm
// on GPU and its application in radiation dose calculation." - Kai Xiao, Jun 03, 2014 

namespace Math
{
	// Takes a structure/class/whatever with a single function, 
	// bool OnDDAIntersection(const glm::ivec3& p)
	// OnDDAIntersection is called for all intersecting voxels
	// If the iterator returns false, the function does not continue
	// and returns false itself
	template<class Intersector>
	bool DDAIntersect(const glm::vec3& rayStart, const glm::vec3& rayEnd, const glm::vec3& voxelSize, Intersector& intersecter);
}

#include "dda.inl"