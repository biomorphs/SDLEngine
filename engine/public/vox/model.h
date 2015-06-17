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
	template< class VoxelDataType, uint32_t t_blockDimensionVx >
	class Model
	{
	public:
		Model();
		~Model();
		void SetVoxelSize(glm::vec3 voxelSize);

		static const uint32_t c_clumpsPerBlock = t_blockDimensionVx >> 1;
		typedef Block< VoxelDataType, c_clumpsPerBlock > BlockType;
		
		class ClumpDataAccessor;	// Has a single GetClump() function, use it to access data in iterator
		typedef std::function<void(ClumpDataAccessor&, glm::vec3, glm::vec3, glm::vec3)> ClumpIterator;
		enum class IteratorAccess	// Determines whether new blocks can be created when GetChunk is called in an iterator
		{
			ReadWrite,
			ReadOnly
		};
		void IterateForArea(Math::Box3 modelSpaceBounds, IteratorAccess itType, ClumpIterator iterator);

	private:
		class AreaIteratorParams;				// Parameter helper object for area iteration
		void IterateClumpsInArea(AreaIteratorParams& params, ClumpIterator iterator);
		glm::ivec3 ModelspaceToBlockIndices(glm::vec3 coordinate);

		PagedBlocks< BlockType > m_voxelData;	// Voxel data container
		glm::vec3 m_voxelDimensions;			// Size of a single voxel. Max bounds should be divisible by this
		glm::vec3 m_blockDimensions;			// Cached for speed
	};
}

#include "model.inl"