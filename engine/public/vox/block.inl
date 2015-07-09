/*
SDLEngine
Matt Hoyle
*/

#include "kernel/assert.h"
#include "math/morton_encoding.h"

//#define USE_SIMPLE_VOXEL_PACKING	// Turns out simple array indices are faster for random access

namespace Vox
{
	template<class DataType, uint32_t t_voxelDimensions, class Allocator>
	Block<DataType, t_voxelDimensions, Allocator>::Block()
	{
		SDE_ASSERT((t_voxelDimensions & (t_voxelDimensions - 1)) == 0);
		void* blockData = Allocator::AllocateBlock(c_maxEntries * sizeof(DataType));
		m_voxelData = static_cast<DataType*>(blockData);
		SDE_ASSERT(m_voxelData);
	}

	template<class DataType, uint32_t t_voxelDimensions, class Allocator>
	Block<DataType, t_voxelDimensions, Allocator>::~Block()
	{
		Allocator::FreeBlock(m_voxelData);
		m_voxelData = nullptr;
	}

	template<class DataType, uint32_t t_voxelDimensions, class Allocator>
	Block<DataType, t_voxelDimensions, Allocator>::Block(Block<DataType, t_voxelDimensions, Allocator>&& other)
	{
		if (this != &other)
		{
			*this = std::move(other);
		}
	}
	
	template<class DataType, uint32_t t_voxelDimensions, class Allocator>
	Block<DataType, t_voxelDimensions, Allocator>& 
		Block<DataType, t_voxelDimensions, Allocator>::operator=(Block<DataType, t_voxelDimensions, Allocator>&& other)
	{
		if (this != &other)
		{
			if (m_voxelData != nullptr)
			{
				Allocator::FreeBlock(m_voxelData);
			}

			m_voxelData = other.m_voxelData;
			other.m_voxelData = nullptr;
		}

		return *this;
	}

	template<class DataType, uint32_t t_voxelDimensions, class Allocator>
	DataType& Block<DataType, t_voxelDimensions, Allocator>::VoxelAt(uint32_t vx, uint32_t vy, uint32_t vz)
	{
		SDE_ASSERT(vx < t_voxelDimensions);
		SDE_ASSERT(vy < t_voxelDimensions);
		SDE_ASSERT(vz < t_voxelDimensions);

#ifdef USE_SIMPLE_VOXEL_PACKING
		return m_voxelData[ vx + (vy * t_voxelDimensions) + (vz * t_voxelDimensions * t_voxelDimensions)];
#else
		uint64_t mortonIndex = Math::MortonEncode(vx, vy, vz);
		SDE_ASSERT(mortonIndex < c_maxEntries);
		return m_voxelData[mortonIndex];
#endif
	}

	template<class DataType, uint32_t t_voxelDimensions, class Allocator>
	const DataType& Block<DataType, t_voxelDimensions, Allocator>::VoxelAt(uint32_t vx, uint32_t vy, uint32_t vz) const
	{
		return const_cast<Block<DataType, t_voxelDimensions, Allocator>*>(this)->VoxelAt(vx, vy, vz);
	}
}