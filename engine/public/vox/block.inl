/*
SDLEngine
Matt Hoyle
*/

#include "kernel/assert.h"
#include "math/morton_encoding.h"

namespace Vox
{
	template<class DataType, uint32_t t_clumpDimensions, class Allocator>
	Block<DataType, t_clumpDimensions, Allocator>::Block()
	{
		SDE_ASSERT((t_clumpDimensions & (t_clumpDimensions - 1)) == 0);
		void* blockData = Allocator::AllocateBlock(c_maxEntries * sizeof(ClumpType));
		m_clumpData = static_cast<ClumpType*>(blockData);
		SDE_ASSERT(m_clumpData);
	}

	template<class DataType, uint32_t t_clumpDimensions, class Allocator>
	Block<DataType, t_clumpDimensions, Allocator>::~Block()
	{
		Allocator::FreeBlock(m_clumpData);
		m_clumpData = nullptr;
	}

	template<class DataType, uint32_t t_clumpDimensions, class Allocator>
	Block<DataType, t_clumpDimensions, Allocator>::Block(Block<DataType, t_clumpDimensions, Allocator>&& other)
	{
		if (this != &other)
		{
			*this = std::move(other);
		}
	}
	
	template<class DataType, uint32_t t_clumpDimensions, class Allocator>
	Block<DataType, t_clumpDimensions, Allocator>& 
		Block<DataType, t_clumpDimensions, Allocator>::operator=(Block<DataType, t_clumpDimensions, Allocator>&& other)
	{
		if (this != &other)
		{
			if (m_clumpData != nullptr)
			{
				Allocator::FreeBlock(m_clumpData);
			}

			m_clumpData = other.m_clumpData;
			other.m_clumpData = nullptr;
		}

		return *this;
	}

	template<class DataType, uint32_t t_clumpDimensions, class Allocator>
	inline bool Block<DataType, t_clumpDimensions, Allocator>::IsClumpInBoundaries(uint32_t x, uint32_t y, uint32_t z) const
	{
		return (x < t_clumpDimensions) && (y < t_clumpDimensions) && (z < t_clumpDimensions);
	}

	template<class DataType, uint32_t t_clumpDimensions, class Allocator>
	inline typename Block<DataType, t_clumpDimensions, Allocator>::ClumpType& 
		Block<DataType, t_clumpDimensions, Allocator>::ClumpAt(uint32_t x, uint32_t y, uint32_t z)
	{
		SDE_ASSERT(IsClumpInBoundaries(x, y, z));
		SDE_ASSERT(m_clumpData != nullptr);
		uint64_t mortonKey = Math::MortonEncode(x, y, z);
		return m_clumpData[mortonKey];
	}

	template<class DataType, uint32_t t_clumpDimensions, class Allocator>
	inline const typename Block<DataType, t_clumpDimensions, Allocator>::ClumpType& 
		Block<DataType, t_clumpDimensions, Allocator>::ClumpAt(uint32_t x, uint32_t y, uint32_t z) const
	{
		return const_cast<Block<DataType, t_clumpDimensions, Allocator>*>(this)->ClumpAt(x, y, z);
	}

	template<class DataType, uint32_t t_clumpDimensions, class Allocator>
	DataType& Block<DataType, t_clumpDimensions, Allocator>::VoxelAt(uint32_t vx, uint32_t vy, uint32_t vz)
	{
		// Find the matching clump
		uint32_t clumpX = vx >> 1;
		uint32_t clumpY = vy >> 1;
		uint32_t clumpZ = vz >> 1;
		SDE_ASSERT(IsClumpInBoundaries(clumpX, clumpY, clumpZ));
		auto& theClump = ClumpAt(clumpX, clumpY, clumpZ);

		// Find the voxel in that clump
		uint32_t voxInClumpX = vx & 0x1;
		uint32_t voxInClumpY = vy & 0x1;
		uint32_t voxInClumpZ = vz & 0x1;

		return theClump.VoxelAt(voxInClumpX, voxInClumpY, voxInClumpZ);
	}

	template<class DataType, uint32_t t_clumpDimensions, class Allocator>
	const DataType& Block<DataType, t_clumpDimensions, Allocator>::VoxelAt(uint32_t vx, uint32_t vy, uint32_t vz) const
	{
		return const_cast<Block<DataType, t_clumpDimensions, Allocator>*>(this)->VoxelAt(vx, vy, vz);
	}
}