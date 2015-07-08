/*
SDLEngine
Matt Hoyle
*/

#include "math/dda.h"
#include "math/intersections.h"

namespace Vox
{
	namespace Internal
	{
		template< class ModelType>
		class ModelClumpRaymarcher
		{
		public:
			ModelClumpRaymarcher(ModelType& target, typename ModelType::ClumpDataAccessor& accessor, const glm::ivec3& blockIndex,
								 typename ModelRaymarcher<ModelType>::ClumpIterator& iterator)
				: m_accessor(accessor)
				, m_blockIndex(blockIndex)
				, m_iterator(iterator)
				, m_target(target)
			{
				m_blockOrigin = (glm::vec3(m_blockIndex) * m_target.GetBlockSize());
			}

			bool OnDDAIntersection(const glm::ivec3& p)
			{
				if (glm::all(glm::greaterThanEqual(p, glm::ivec3(0))) &&
					glm::all(glm::lessThan(p, glm::ivec3(VoxelModel::c_clumpsPerBlock))))
				{
					m_accessor.SetClumpIndex(p);
					const glm::vec3 clumpOrigin = m_blockOrigin + glm::vec3(p) * (m_target.GetVoxelSize() * 2.0f);
					const glm::vec3 voxelCenter = m_target.GetVoxelSize() * 0.5f;
					return !m_iterator(m_accessor, clumpOrigin, m_target.GetVoxelSize(), voxelCenter);
				}
				return false;
			}

		private:
			typename ModelType::ClumpDataAccessor& m_accessor;
			typename ModelRaymarcher<ModelType>::ClumpIterator& m_iterator;
			ModelType& m_target;
			glm::ivec3 m_blockIndex;
			glm::vec3 m_blockOrigin;
		};

		template< class ModelType>
		class ModelBlockRaymarcher
		{
		public:
			ModelBlockRaymarcher(ModelType& target, const glm::vec3& rayStart, const glm::vec3& rayEnd, 
								 typename ModelRaymarcher<ModelType>::ClumpIterator& iterator)
				: m_target(target)
				, m_rayStart(rayStart)
				, m_rayEnd(rayEnd)
				, m_it(iterator)
			{
			}

			bool OnDDAIntersection(const glm::ivec3& p)
			{
				ModelType::ModelDataAccessor accessor(m_target);
				if (accessor.HasBlockData(p))
				{
					ModelType::ClumpDataAccessor clumpAccessor(&m_target, p);

					const auto voxelSize = m_target.GetVoxelSize();
					const auto blockSize = m_target.GetBlockSize();
					const auto blockStartCoord = glm::vec3(p) * blockSize;
					const auto blockEndCoord = blockStartCoord + blockSize;

					// Calculate where this ray lands in block-space using ray-AABB intersection points
					float rayHitNear = 0.0f, rayHitFar = 0.0f;
					Math::RayIntersectsAAB(m_rayStart, m_rayEnd, Math::Box3(blockStartCoord, blockEndCoord), rayHitNear, rayHitFar);

					glm::vec3 rayDistance = m_rayEnd - m_rayStart;
					glm::vec3 blockRayStart = m_rayStart + (rayDistance * rayHitNear);
					glm::vec3 blockRayEnd = m_rayStart + (rayDistance * rayHitFar);

					// And move into local block-space
					blockRayStart -= blockStartCoord;
					blockRayEnd -= blockStartCoord;

					// Run DDA for this block at clump resolution
					ModelClumpRaymarcher<ModelType> clumpRaymarcher(m_target, clumpAccessor, p, m_it);
					Math::DDAIntersect(blockRayStart, blockRayEnd, m_target.GetVoxelSize() * 2.0f, clumpRaymarcher);
				}
				return false;	// We can't stop the DDA here since there may be more intersecting blocks
			}

		private:
			ModelType& m_target;
			glm::vec3 m_rayStart;
			glm::vec3 m_rayEnd;
			typename ModelRaymarcher<ModelType>::ClumpIterator& m_it;
		};
	}

	template< class ModelType>
	ModelRaymarcher<ModelType>::ModelRaymarcher(ModelType& target)
		: m_target(target)
	{
	}

	template< class ModelType>
	ModelRaymarcher<ModelType>::~ModelRaymarcher()
	{
	}

	template< class ModelType>
	void ModelRaymarcher<ModelType>::Raymarch(const glm::vec3& rayStart, const glm::vec3& rayEnd, ClumpIterator iterator)
	{
		// First we run DDA at block resolution, which gives us all potentially
		// intersecting blocks.
		auto modelBlockSize = m_target.GetBlockSize();
		Internal::ModelBlockRaymarcher<ModelType> blockMarcher(m_target, rayStart, rayEnd, iterator);
		Math::DDAIntersect(rayStart, rayEnd, modelBlockSize, blockMarcher);
	}
}