/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "block.h"
#include "math/math.h"
#include <unordered_map>

namespace Vox
{
	// Handles paging of blocks in arbitrary 3d space
	// Only blocks containing data are stored
	template< class BlockType >
	class PagedBlocks
	{
	public:
		PagedBlocks();
		~PagedBlocks();

		// Block accessors (in block coords).
		BlockType* BlockAt(const glm::ivec3& coords, bool createNewBlocks = true);
		const BlockType* BlockAt(const glm::ivec3& coords) const;

		// Clump accoessors (in clump coords).
		typename BlockType::ClumpType* ClumpAt(const glm::ivec3& coords);

		// Voxel accessors (dito)
		typename BlockType::ClumpType::VoxelDataType* VoxelAt(const glm::ivec3& coords);

		// Remove all data
		void Clear();

		// Stats
		uint64_t TotalBlocks() const;
		uint64_t TotalVoxelMemory() const;

		// Block iterators
		typename std::unordered_map<uint64_t, BlockType*>::iterator begin() const { return m_blockData.begin(); }
		typename std::unordered_map<uint64_t, BlockType*>::iterator end() const { return m_blockData.end(); }

	private:
		uint64_t HashCoords(const glm::ivec3& coords) const;
		std::unordered_map<uint64_t, BlockType*> m_blockData;
	};
}

#include "paged_blocks.inl"