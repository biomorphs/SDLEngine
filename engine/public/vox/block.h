/*
SDLEngine
Matt Hoyle
*/
#pragma once

namespace Vox
{
	// a block is a t_voxelDimensions^3 set of tightly packed voxels 
	// stored in morten order. Dimensions MUST be power of two
	// internal data is allocated in RAII fashion
	template<class DataType, uint32_t t_voxelDimensions, class Allocator>
	class Block
	{
	public:
		Block();
		~Block();
		Block(Block&& other);
		Block& operator=(Block&& other);
		Block(const Block& other) = delete;
		Block& operator=(const Block& other) = delete;

		typedef DataType VoxelDataType;
		enum
		{
			VoxelDimensions = t_voxelDimensions,
		};

		// Voxel access (in voxel coords)
		DataType& VoxelAt(uint32_t vx, uint32_t vy, uint32_t vz);
		const DataType& VoxelAt(uint32_t vx, uint32_t vy, uint32_t vz) const;

	private:
		static const uint32_t c_maxEntries = t_voxelDimensions * t_voxelDimensions * t_voxelDimensions;
		DataType* m_voxelData;
	};
}

#include "block.inl"