/*
SDLEngine
Matt Hoyle
*/
#pragma once

namespace Math
{
	template<class Intersector>
	inline void DDAIntersect(const glm::vec3& rayStart, const glm::vec3& rayEnd, const glm::vec3& voxelSize, Intersector& intersecter)
	{
		glm::vec3 scaledRayStart = rayStart / voxelSize;
		glm::vec3 scaledRayEnd = rayEnd / voxelSize;

		glm::ivec3 currentV;
		glm::ivec3 vStep;

		glm::vec3 rayTIncrement;
		glm::vec3 rayT;

		const float threshold = 1.0e-6f;
		const glm::vec3 rayDirection = scaledRayEnd - scaledRayStart;
		for (int i = 0; i < 3; i++)
		{
			// inverse scaling parameters
			bool bAligned = (fabs(rayDirection[i]) < threshold) ? true : false;
			float fInvD = (bAligned) ? 0.0f : 1.0f / rayDirection[i];
			float signD = (rayDirection[i] < 0.0f ? -1 : 1.0f);

			// which side of the cell we need to test
			float fPlane = (floor(scaledRayStart[i]) + 0.5f + signD * 0.5f);

			// first cell to trasverse
			currentV[i] = floor(scaledRayStart[i]);
			vStep[i] = signD;

			// the DDA increment for that axis
			rayTIncrement[i] = (bAligned) ? 0.0f : fabs(fInvD);

			// the first time of intersection
			rayT[i] = (bAligned) ? 1.01f : fabs(fPlane - scaledRayStart[i]) * rayTIncrement[i];
		}

		while (1)
		{
			if (intersecter.OnDDAIntersection(currentV))
			{
				break;
			}

			bool ok = false;

			// find the dimension with the closest intersection
			int iMinAxis = -1;
			for (int i = 0; i < 3; i++)
			{
				// have we reached end of ray
				ok |= (rayT[i] <= 1.0f);

				// it's the minimum (or the first)
				if (iMinAxis == -1 || rayT[i] < rayT[iMinAxis])
				{
					// that will be our dimension to use
					// for incrementing.
					iMinAxis = i;
				}
			}

			if (!ok)
				break;

			// move to next cell along the dimension of minimum intersection
			currentV[iMinAxis] += vStep[iMinAxis];
			rayT[iMinAxis] += rayTIncrement[iMinAxis];
		}
	}
}