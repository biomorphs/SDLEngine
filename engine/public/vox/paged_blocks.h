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
	// Voxel data is assumed to be tightly packed in a grid
	template< class BlockType >
	class PagedBlocks
	{
	public:
		PagedBlocks();
		~PagedBlocks();

		// Block accessors (in block coords). Non-const version creates new
		// blocks, so do NOT use it for iteration
		BlockType* BlockAt(uint32_t bx, uint32_t by, uint32_t bz);
		const BlockType* BlockAt(uint32_t bx, uint32_t by, uint32_t bz) const;

		// Clump accoessors (in clump coords). Same deal as above, no create in const version
		typename BlockType::ClumpType* ClumpAt(uint32_t cx, uint32_t cy, uint32_t cz);
		const typename BlockType::ClumpType* ClumpAt(uint32_t cx, uint32_t cy, uint32_t cz) const;

		// Voxel accessors (dito)
		typename BlockType::ClumpType::VoxelDataType* VoxelAt(uint32_t vx, uint32_t vy, uint32_t vz);

		// Remove all data
		void Clear();

		// Stats
		uint64_t TotalBlocks() const;
		uint64_t TotalVoxelMemory() const;

	private:
		uint64_t HashCoords(uint32_t bx, uint32_t by, uint32_t bz) const;
		std::unordered_map<uint64_t, BlockType*> m_blockData;
	};
}

#include "paged_blocks.inl"