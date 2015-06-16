/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>

namespace Vox
{
	// Base voxel data, a 2x2 block of packed voxel data stored in morten order
	template<class DataType>
	struct Clump
	{
		typedef DataType VoxelDataType;
		DataType m_rawData[8];

		inline DataType& VoxelAt(uint32_t x, uint32_t y, uint32_t z)
		{
			SDE_ASSERT(x < 2 && y < 2 && z < 2);
			const uint32_t mortonKey = (z << 2) + (y << 1) + x;
			return m_rawData[mortonKey];
		}

		inline const DataType& VoxelAt(uint32_t x, uint32_t y, uint32_t z) const
		{
			SDE_ASSERT(x < 2 && y < 2 && z < 2);
			const uint32_t mortonKey = (z << 2) + (y << 1) + x;
			return m_rawData[mortonKey];
		}
	};
}