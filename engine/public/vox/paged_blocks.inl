/*
SDLEngine
Matt Hoyle
*/

namespace Vox
{
	template< class BlockType >
	PagedBlocks<BlockType>::PagedBlocks()
	{
	}

	template< class BlockType >
	PagedBlocks<BlockType>::~PagedBlocks()
	{
		Clear();
	}

	template< class BlockType >
	inline uint64_t PagedBlocks<BlockType>::TotalBlocks() const
	{ 
		return m_blockData.size(); 
	}

	template< class BlockType >
	inline uint64_t PagedBlocks<BlockType>::TotalVoxelMemory() const
	{ 
		return m_blockData.size() * sizeof(typename BlockType::ClumpType) *
			(typename BlockType::BlockDimensions * typename BlockType::BlockDimensions * typename BlockType::BlockDimensions);
	}

	template< class BlockType >
	void PagedBlocks<BlockType>::Clear()
	{
		for (auto it : m_blockData)
		{
			delete it.second;
		}
		m_blockData.clear();
	}

	template< class BlockType >
	inline uint64_t PagedBlocks<BlockType>::HashCoords(const glm::ivec3& coords) const
	{
		// 21 bits per axis - 20 + 1 sign bit, so we can store negative coords
		const uint64_t c_20Bits = 0xfffff;
		const uint64_t c_signBitMask = 0x80000000;
		uint64_t hx = (coords.x & c_20Bits) | ((coords.x & c_signBitMask) >> 11);
		uint64_t hy = (coords.y & c_20Bits) | ((coords.y & c_signBitMask) >> 11);
		uint64_t hz = (coords.z & c_20Bits) | ((coords.z & c_signBitMask) >> 11);
		return hx | (hy << 21) | (hz << 42);
	}

	template< class BlockType >
	BlockType* PagedBlocks<BlockType>::BlockAt(const glm::ivec3& coords, bool createNewBlocks = true)
	{
		uint64_t key = HashCoords(coords);
		auto it = m_blockData.find(key);
		if (it != m_blockData.end())
		{
			return it->second;
		}
		else
		{
			if (!createNewBlocks)
			{
				return nullptr;
			}
			else
			{
				BlockType* newBlock = new BlockType();
				SDE_ASSERT(newBlock);
				m_blockData[key] = newBlock;
				return newBlock;
			}
		}
	}

	template< class BlockType >
	const BlockType* PagedBlocks<BlockType>::BlockAt(const glm::ivec3& coords) const
	{
		uint64_t key = HashCoords(coords);
		const auto it = m_blockData.find(key);
		if (it != m_blockData.end())
		{
			return it->second;
		}
		else
		{
			return nullptr;
		}
	}

	template< class BlockType >
	typename BlockType::ClumpType* PagedBlocks<BlockType>::ClumpAt(const glm::ivec3& coords)
	{
		// split the chunk coords from the block coords, keeping the sign
		const uint32_t c_chunkLookupShift = Math::Log2(typename BlockType::BlockDimensions);
		const uint32_t c_chunkLookupMask = typename BlockType::BlockDimensions - 1;
		const uint32_t c_blockLookupMaskNoSign = ~(0x80000000 | c_chunkLookupMask);
		const uint32_t c_signBitMask = 0x80000000;

		// extract block coords + sign bit so we can shift + reconstruct
		glm::ivec3 blockCoords;
		blockCoords.x = (coords.x & c_blockLookupMaskNoSign) >> c_chunkLookupShift;
		blockCoords.y = (coords.y & c_blockLookupMaskNoSign) >> c_chunkLookupShift;
		blockCoords.z = (coords.z & c_blockLookupMaskNoSign) >> c_chunkLookupShift;
		blockCoords.x |= (coords.x & c_signBitMask);
		blockCoords.y |= (coords.x & c_signBitMask);
		blockCoords.z |= (coords.x & c_signBitMask);

		// extract clump coords
		glm::ivec3 chunkCoords;
		chunkCoords.x = coords.x & c_chunkLookupMask;
		chunkCoords.y = coords.y & c_chunkLookupMask;
		chunkCoords.z = coords.z & c_chunkLookupMask;

		auto theBlock = BlockAt(blockCoords);
		return &theBlock->ClumpAt(chunkCoords.x, chunkCoords.y, chunkCoords.z);
	}

	template< class BlockType >
	typename BlockType::ClumpType::VoxelDataType* PagedBlocks<BlockType>::VoxelAt(const glm::ivec3& coords)
	{
		const uint32_t c_signBitMask = 0x80000000;

		glm::ivec3 clumpCoords;
		clumpCoords.x = ((coords.x & ~c_signBitMask) >> 1) | (coords.x & c_signBitMask);
		clumpCoords.y = ((coords.y & ~c_signBitMask) >> 1) | (coords.y & c_signBitMask);
		clumpCoords.z = ((coords.z & ~c_signBitMask) >> 1) | (coords.z & c_signBitMask);

		auto theClump = ClumpAt(clumpCoords);
		return &theClump->VoxelAt(coords.x & 0x1, coords.y & 0x1, coords.z & 0x1);
	}
}