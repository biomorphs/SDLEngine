/*
SDLEngine
Matt Hoyle
*/

#include "kernel/assert.h"
#include "math/morton_encoding.h"

namespace Vox
{
	template<class DataType, uint32_t t_clumpDimensions>
	Block<DataType, t_clumpDimensions>::Block()
	{
		SDE_ASSERT((t_clumpDimensions & (t_clumpDimensions - 1)) == 0);
		m_clumpData = new ClumpType[c_maxEntries];
		SDE_ASSERT(m_clumpData);
		memset(m_clumpData, 0, sizeof(ClumpType)* c_maxEntries);
	}

	template<class DataType, uint32_t t_clumpDimensions>
	Block<DataType, t_clumpDimensions>::~Block()
	{
		delete[] m_clumpData;
		m_clumpData = nullptr;
	}

	template<class DataType, uint32_t t_clumpDimensions>
	Block<DataType, t_clumpDimensions>::Block(Block<DataType, t_clumpDimensions>&& other)
	{
		if (this != &other)
		{
			*this = std::move(other);
		}
	}
	
	template<class DataType, uint32_t t_clumpDimensions>
	Block<DataType, t_clumpDimensions>& Block<DataType, t_clumpDimensions>::operator=(Block<DataType, t_clumpDimensions>&& other)
	{
		if (this != &other)
		{
			if (m_clumpData != nullptr)
			{
				delete[] m_clumpData;
			}

			m_clumpData = other.m_clumpData;
			other.m_clumpData = nullptr;
		}

		return *this;
	}

	template<class DataType, uint32_t t_clumpDimensions>
	inline bool Block<DataType, t_clumpDimensions>::IsClumpInBoundaries(uint32_t x, uint32_t y, uint32_t z) const
	{
		return (x < t_clumpDimensions) && (y < t_clumpDimensions) && (z < t_clumpDimensions);
	}

	template<class DataType, uint32_t t_clumpDimensions>
	typename Block<DataType, t_clumpDimensions>::ClumpType& Block<DataType, t_clumpDimensions>::ClumpAt(uint32_t x, uint32_t y, uint32_t z)
	{
		SDE_ASSERT(IsClumpInBoundaries(x, y, z));
		SDE_ASSERT(m_clumpData != nullptr);
		uint64_t mortonKey = Math::MortonEncode(x, y, z);
		return m_clumpData[mortonKey];
	}

	template<class DataType, uint32_t t_clumpDimensions>
	const typename Block<DataType, t_clumpDimensions>::ClumpType& Block<DataType, t_clumpDimensions>::ClumpAt(uint32_t x, uint32_t y, uint32_t z) const
	{
		return const_cast<Block<DataType, t_clumpDimensions>*>(this)->ClumpAt(x, y, z);
	}

	template<class DataType, uint32_t t_clumpDimensions>
	DataType& Block<DataType, t_clumpDimensions>::VoxelAt(uint32_t vx, uint32_t vy, uint32_t vz)
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

	template<class DataType, uint32_t t_clumpDimensions>
	const DataType& Block<DataType, t_clumpDimensions>::VoxelAt(uint32_t vx, uint32_t vy, uint32_t vz) const
	{
		return const_cast<Block<DataType, t_clumpDimensions>*>(this)->VoxelAt(vx, vy, vz);
	}
}