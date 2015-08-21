/*
SDLEngine
Matt Hoyle
*/

#include <glm/gtx/component_wise.hpp>

namespace Math
{
	inline bool RayIntersectsAAB(const glm::vec3& rayStart, const glm::vec3& rayEnd, const Math::Box3& aabb, float& tNearOut, float& tFarOut)
	{
		glm::vec3 t1, t2; // vectors to hold the T-values for every direction
		double tNear = -DBL_MAX;
		double tFar = DBL_MAX;

		glm::vec3 rayDirection = glm::normalize(rayEnd - rayStart);
		for (int i = 0; i < 3; i++)
		{
			//we test slabs in every direction
			if (rayDirection[i] == 0.0f)
			{
				// ray parallel to planes in this direction
				if ((rayStart[i] < aabb.Min()[i]) || (rayStart[i] > aabb.Max()[i]))
				{
					return false; // parallel AND outside box : no intersection possible
				}
			}
			else
			{
				// ray not parallel to planes in this direction
				t1[i] = (aabb.Min()[i] - rayStart[i]) / rayDirection[i];
				t2[i] = (aabb.Max()[i] - rayStart[i]) / rayDirection[i];
				if (t1[i] > t2[i]) 
				{
					glm::vec3 tmp = t1;
					t1 = t2;
					t2 = tmp;
				}
				if (t1[i] > tNear) 
				{
					tNear = t1[i];
				}
				if (t2[i] < tFar) 
				{
					tFar = t2[i];
				}
				if ((tNear > tFar) || (tFar < 0)) 
				{
					return false;
				}
			}
		}
		const float rayLengthDiv = 1.0f / glm::length(rayEnd - rayStart);
		tNearOut = (float)(rayLengthDiv * tNear);
		tFarOut = (float)(rayLengthDiv * tFar);
		if (tNearOut < 0.0f)
		{
			tNearOut = 0.0f;
		}
		else if (tNearOut > 1.0f)
		{
			return false;
		}
		if (tFarOut > 1.0f)
		{
			tFarOut = 1.0f;
		}
		else if(tFarOut < 0.0f)
		{
			return false;
		}

		return true;
	}
}