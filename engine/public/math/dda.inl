/*
SDLEngine
Matt Hoyle
*/
#pragma once

namespace Math
{
	template<class Intersector>
	inline bool DDAIntersect(const glm::vec3& rayStart, const glm::vec3& rayEnd, const glm::vec3& voxelSize, Intersector& intersecter)
	{
		// Rescale ray as if we were acting on a grid with voxel size = 1
		const glm::vec3 scaledRayStart = rayStart / voxelSize;
		const glm::vec3 scaledRayEnd = rayEnd / voxelSize;
		const glm::vec3 rayDirection = scaledRayEnd - scaledRayStart;

		glm::ivec3 currentV = glm::floor(scaledRayStart);		// current voxel indices
		glm::ivec3 vStep = glm::sign(rayDirection);				// step direction in voxels

		// Vectorised DDA parameters
		const glm::vec3 c_threshold(1.0e-6f);
		const auto dirAxisAligned = glm::lessThan(glm::abs(rayDirection), c_threshold);
		const auto dirAxisAlignedInverted = glm::equal(glm::tvec3<bool>(false), dirAxisAligned);
		const glm::vec3 planeDiff = glm::abs((glm::floor(scaledRayStart) + 0.5f) + (glm::vec3(vStep) * 0.5f) - scaledRayStart);

		// Calculate inverse direction (sucks, but I can't see a way to vectorise)
		glm::vec3 inverseDirection;
		inverseDirection.x = dirAxisAligned.x ? 0.0f : (1.0f / rayDirection.x);
		inverseDirection.y = dirAxisAligned.y ? 0.0f : (1.0f / rayDirection.y);
		inverseDirection.z = dirAxisAligned.z ? 0.0f : (1.0f / rayDirection.z);

		// Ray increment per step
		const glm::vec3 rayIncrement = glm::vec3(dirAxisAlignedInverted) * glm::abs(inverseDirection);

		// Ray initial T value
		glm::vec3 rayT;											// current T value for ray in 3 axes
		rayT.x = dirAxisAligned.x ? 1.01f : planeDiff.x * rayIncrement.x;
		rayT.y = dirAxisAligned.y ? 1.01f : planeDiff.y * rayIncrement.y;
		rayT.z = dirAxisAligned.z ? 1.01f : planeDiff.z * rayIncrement.z;

		while (glm::any(glm::lessThanEqual(rayT, glm::vec3(1.0f))))
		{
			if (!intersecter.OnDDAIntersection(currentV))
			{
				return false;
			}

			// find the dimension with the closest intersection
			int iMinAxis = -1;
			for (int i = 0; i < 3; i++)
			{
				// it's the minimum (or the first)
				if (iMinAxis == -1 || rayT[i] < rayT[iMinAxis])
				{
					iMinAxis = i;
				}
			}

			// move to next cell along the dimension of minimum intersection
			currentV[iMinAxis] += vStep[iMinAxis];
			rayT[iMinAxis] += rayIncrement[iMinAxis];
		}

		return true;
	}
}