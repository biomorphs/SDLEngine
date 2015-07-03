/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "paged_blocks.h"
#include "math/box3.h"
#include <glm.hpp>
#include <functional>

namespace Vox
{
	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	class Model
	{
	public:
		Model();
		~Model();
		void SetVoxelSize(const glm::vec3& voxelSize);
		inline const glm::vec3& GetVoxelSize() const { return m_voxelDimensions; }

		static const uint32_t c_clumpsPerBlock = t_blockDimensionVx >> 1;
		typedef Block< VoxelDataType, c_clumpsPerBlock, BlockAllocator > BlockType;
		
		class ModelDataAccessor;	// Used to *read* voxel data from model, does internal caching
		class ClumpDataAccessor;	// Has a single GetClump() function, use it to access data in iterator
		typedef std::function<void(ClumpDataAccessor&, const glm::vec3&, const glm::vec3&, const glm::vec3&)> ClumpIterator;
		enum class IteratorAccess	// Determines whether new blocks can be created when GetChunk is called in an iterator
		{
			ReadWrite,
			ReadOnly
		};
		void IterateForArea(Math::Box3 modelSpaceBounds, IteratorAccess itType, ClumpIterator iterator);

		void GetBlockIterationParameters(const Math::Box3& modelSpaceBounds, glm::ivec3& start, glm::ivec3& end);
		void GetClumpIterationParameters(const glm::ivec3& blockIndex, const Math::Box3& modelSpaceBounds, glm::ivec3& start, glm::ivec3& end);
		glm::vec3 GetVoxelCenterPosition(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex);

	private:
		class AreaIteratorParams;				// Parameter helper object for area iteration
		void IterateClumpsInArea(AreaIteratorParams& params, ClumpIterator iterator);
		glm::ivec3 ModelspaceToBlockIndices(const glm::vec3& coordinate);

		PagedBlocks< BlockType > m_voxelData;	// Voxel data container
		glm::vec3 m_voxelDimensions;			// Size of a single voxel. Max bounds should be divisible by this
		glm::vec3 m_blockDimensions;			// Cached for speed
	};
}

#include "model.inl"