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
	template< class DataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	class Model
	{
	public:
		Model();
		~Model();
		void SetVoxelSize(const glm::vec3& voxelSize);
		inline const glm::vec3& GetVoxelSize() const { return m_voxelDimensions; }
		inline const glm::vec3& GetBlockSize() const { return m_blockDimensions; }

		typedef Block< DataType, t_blockDimensionVx, BlockAllocator > BlockType;
		typedef DataType VoxelDataType;

		const BlockType* BlockAt(const glm::ivec3& blockIndex) const		{ return m_voxelData.BlockAt(blockIndex); }
		BlockType* BlockAt(const glm::ivec3& blockIndex, bool createNew)	{ return m_voxelData.BlockAt(blockIndex, createNew); }

		void GetBlockIterationParameters(const Math::Box3& modelSpaceBounds, glm::ivec3& start, glm::ivec3& end) const;
		void GetVoxelIterationParameters(const glm::ivec3& blockIndex, const Math::Box3& modelSpaceBounds, glm::ivec3& start, glm::ivec3& end)  const;
		glm::vec3 GetVoxelCenterPosition(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex)  const;

		void PreallocateMemory(Math::Box3 modelSpaceBounds);

	private:
		glm::ivec3 ModelspaceToBlockIndices(const glm::vec3& coordinate) const;

		PagedBlocks< BlockType > m_voxelData;	// Voxel data container
		glm::vec3 m_voxelDimensions;			// Size of a single voxel. Max bounds should be divisible by this
		glm::vec3 m_blockDimensions;			// Cached for speed
	};
}

#include "model.inl"