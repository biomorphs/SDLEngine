/*
SDLEngine
Matt Hoyle
*/

#include "math/dda.h"
#include "math/intersections.h"
#include "model_data_reader.h"

namespace Vox
{
	namespace Internal
	{
		template< class ModelType>
		class ModelVoxelRaymarcher
		{
		public:
			ModelVoxelRaymarcher(ModelRaymarcherParams<ModelType>& params,
								 typename ModelRaymarcher<ModelType>::VoxelIterator& iterator)
				: m_params(params)
				, m_iterator(iterator)
			{
			}

			bool OnDDAIntersection(const glm::ivec3& p)
			{
				if (glm::all(glm::greaterThanEqual(p, glm::ivec3(0))) &&
					glm::all(glm::lessThan(p, glm::ivec3(VoxelModel::BlockType::VoxelDimensions))))
				{
					m_params.m_voxelIndex = p;
					return m_iterator(m_params);
				}
				return true;
			}

		private:
			ModelRaymarcherParams<ModelType>& m_params;
			typename ModelRaymarcher<ModelType>::VoxelIterator& m_iterator;
		};

		template< class ModelType>
		class ModelBlockRaymarcher
		{
		public:
			ModelBlockRaymarcher(const ModelType& target, const glm::vec3& rayStart, const glm::vec3& rayEnd,
				typename ModelRaymarcher<ModelType>::VoxelIterator& iterator)
				: m_target(target)
				, m_rayStart(rayStart)
				, m_rayEnd(rayEnd)
				, m_it(iterator)
			{
			}

			bool OnDDAIntersection(const glm::ivec3& p)
			{
				ModelDataReader<ModelType> accessor(m_target);
				if (accessor.HasBlockData(p))
				{
					const auto voxelSize = m_target.GetVoxelSize();
					const auto blockSize = m_target.GetBlockSize();
					const auto blockStartCoord = glm::vec3(p) * blockSize;
					const auto blockEndCoord = blockStartCoord + blockSize;
					const Math::Box3 blockBounds(blockStartCoord, blockEndCoord);

					// Calculate where this ray lands in block-space using ray-AABB intersection points
					float rayHitNear = 0.0f, rayHitFar = 0.0f;
					Math::RayIntersectsAAB(m_rayStart, m_rayEnd, Math::Box3(blockStartCoord, blockEndCoord), rayHitNear, rayHitFar);

					glm::vec3 rayDistance = m_rayEnd - m_rayStart;
					glm::vec3 blockRayStart = m_rayStart + (rayDistance * rayHitNear);
					glm::vec3 blockRayEnd = m_rayStart + (rayDistance * rayHitFar);

					// And move into local block-space
					blockRayStart -= blockStartCoord;
					blockRayEnd -= blockStartCoord;

					ModelRaymarcherParams<ModelType> params(&accessor, p, blockBounds, voxelSize);

					// Run DDA for this block at voxel resolution
					ModelVoxelRaymarcher<ModelType> voxelRaymarch(params, m_it);
					return Math::DDAIntersect(blockRayStart, blockRayEnd, voxelSize, voxelRaymarch);
				}
				return true;	// We can't stop the DDA here since there may be more intersecting blocks
			}

		private:
			const ModelType& m_target;
			glm::vec3 m_rayStart;
			glm::vec3 m_rayEnd;
			typename ModelRaymarcher<ModelType>::VoxelIterator& m_it;
		};
	}

	template< class ModelType>
	inline glm::vec3 ModelRaymarcherParams<ModelType>::VoxelPosition() const
	{
		return m_blockBounds.Min() + (glm::vec3(m_voxelIndex) * m_voxelSize) + (m_voxelSize * 0.5f);
	}

	template< class ModelType>
	inline typename const ModelType::VoxelDataType ModelRaymarcherParams<ModelType>::VoxelData() const
	{
		return m_reader->VoxelAt(m_blockIndex, m_voxelIndex);
	}

	template< class ModelType>
	ModelRaymarcher<ModelType>::ModelRaymarcher(const ModelType& target)
		: m_target(target)
	{
	}

	template< class ModelType>
	ModelRaymarcher<ModelType>::~ModelRaymarcher()
	{
	}

	template< class ModelType>
	void ModelRaymarcher<ModelType>::Raymarch(const glm::vec3& rayStart, const glm::vec3& rayEnd, VoxelIterator iterator)
	{
		// First we run DDA at block resolution, which gives us all potentially
		// intersecting blocks.
		auto modelBlockSize = m_target.GetBlockSize();
		Internal::ModelBlockRaymarcher<ModelType> blockMarcher(m_target, rayStart, rayEnd, iterator);
		Math::DDAIntersect(rayStart, rayEnd, modelBlockSize, blockMarcher);
	}
}