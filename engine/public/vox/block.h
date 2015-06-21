/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "clump.h"

namespace Vox
{
	// a block is a t_clumpDimensions^3 set of tightly packed clumps, 
	// stored in morten order, dimensions MUST be power of two
	// internal data is allocated in RAII fashion, but we can change that later
	template<class DataType, uint32_t t_clumpDimensions, class Allocator>
	class Block
	{
	public:
		Block();
		~Block();
		Block(Block&& other);
		Block& operator=(Block&& other);
		Block(const Block& other) = delete;
		Block& operator=(const Block& other) = delete;
		typedef Clump<DataType> ClumpType;
		enum
		{
			BlockDimensions = t_clumpDimensions
		};

		// Clump access (in clump coords)
		ClumpType& ClumpAt(uint32_t cx, uint32_t cy, uint32_t cz);
		const ClumpType& ClumpAt(uint32_t cx, uint32_t cy, uint32_t cz) const;

		// Voxel access (in voxel coords)
		DataType& VoxelAt(uint32_t vx, uint32_t vy, uint32_t vz);
		const DataType& VoxelAt(uint32_t vx, uint32_t vy, uint32_t vz) const;

		// Boundary test
		bool IsClumpInBoundaries(uint32_t x, uint32_t y, uint32_t z) const;
	private:
		static const uint32_t c_maxEntries = t_clumpDimensions * t_clumpDimensions * t_clumpDimensions;
		ClumpType* m_clumpData;
	};
}

#include "block.inl"