/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "paged_blocks.h"
#include "math/box3.h"
#include <glm.hpp>

namespace Vox
{
	// A model built using paged voxel table, with bounds
	// and data iteration.
	// Voxel data is aligned via the model max bounds, which are set on creation
	template< class VoxelDataType, uint32_t t_blockDimensionVx >
	class Model
	{
	public:
		Model();
		~Model();

		void SetModelExtents(Math::Box3 maxBounds, glm::vec3 voxelSize);

		static const uint32_t c_clumpsPerBlock = t_blockDimensionVx >> 1;
		typedef Block< VoxelDataType, c_clumpsPerBlock > BlockType;
	
		void WriteData(Math::Box3 modelSpaceBounds);

	private:
		void BlockCoordToPageIndices(glm::vec3 coordinate, glm::ivec3& result);
		void BlockRangeIntersectingBounds(Math::Box3 boundsToSearch, glm::ivec3& startRange, glm::ivec3& endRange);

		PagedBlocks< BlockType > m_voxelData;
		Math::Box3 m_maxDimensions;		// Maximum bounds for the entire model (NOT the actual voxel data bounds)
		glm::vec3 m_voxelDimensions;	// Size of a single voxel. Max bounds should be divisible by this
		glm::vec3 m_blockDimensions;	// Cached for speed
	};
}

#include "model.inl"