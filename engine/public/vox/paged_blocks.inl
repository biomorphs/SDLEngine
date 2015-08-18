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
		return m_blockData.size() * sizeof(typename BlockType::VoxelDataType) *
			(typename BlockType::VoxelDimensions * typename BlockType::VoxelDimensions * typename BlockType::VoxelDimensions);
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
	inline BlockType* PagedBlocks<BlockType>::BlockAt(const glm::ivec3& coords, bool createNewBlocks = true)
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
	inline const BlockType* PagedBlocks<BlockType>::BlockAt(const glm::ivec3& coords) const
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
}