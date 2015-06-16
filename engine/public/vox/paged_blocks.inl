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
	inline uint64_t PagedBlocks<BlockType>::HashCoords(uint32_t bx, uint32_t by, uint32_t bz) const 
	{
		// 20 bits per axis = max 1048576 ^ 3 entries
		const uint64_t c_20Bits = 0xfffff;
		uint64_t hx = bx;
		uint64_t hy = by;
		uint64_t hz = bz;
		return (hx & c_20Bits) | ((hy & c_20Bits) << 20) | ((hz & c_20Bits) << 40);
	}

	template< class BlockType >
	BlockType* PagedBlocks<BlockType>::BlockAt(uint32_t bx, uint32_t by, uint32_t bz)
	{
		uint64_t key = HashCoords(bx, by, bz);
		auto it = m_blockData.find(key);
		if (it != m_blockData.end())
		{
			return it->second;
		}
		else
		{
			BlockType* newBlock = new BlockType();
			SDE_ASSERT(newBlock);
			m_blockData[key] = newBlock;
			return newBlock;
		}
	}

	template< class BlockType >
	const BlockType* PagedBlocks<BlockType>::BlockAt(uint32_t bx, uint32_t by, uint32_t bz) const
	{
		uint64_t key = HashCoords(bx, by, bz);
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
	typename BlockType::ClumpType* PagedBlocks<BlockType>::ClumpAt(uint32_t cx, uint32_t cy, uint32_t cz)
	{
		const uint32_t c_blockLookupShift = Math::Log2(typename BlockType::BlockDimensions);
		const uint32_t c_blockLookupMask = typename BlockType::BlockDimensions - 1;
		auto theBlock = BlockAt(cx >> c_blockLookupShift, cy >> c_blockLookupShift, cz >> c_blockLookupShift);
		return &theBlock->ClumpAt(cx & c_blockLookupMask, cy & c_blockLookupMask, cz & c_blockLookupMask);
	}

	template< class BlockType >
	const typename BlockType::ClumpType* PagedBlocks<BlockType>::ClumpAt(uint32_t cx, uint32_t cy, uint32_t cz) const
	{
		const uint32_t c_blockLookupShift = Math::Log2(typename BlockType::BlockDimensions);
		const uint32_t c_blockLookupMask = typename BlockType::BlockDimensions - 1;
		auto theBlock = BlockAt(cx >> c_blockLookupShift, cy >> c_blockLookupShift, cz >> c_blockLookupShift);
		if (theBlock != nullptr)
		{
			return &theBlock->ClumpAt(cx & c_blockLookupMask, cy & c_blockLookupMask, cz & c_blockLookupMask);
		}
		else
		{
			return nullptr;
		}		
	}

	template< class BlockType >
	typename BlockType::ClumpType::VoxelDataType* PagedBlocks<BlockType>::VoxelAt(uint32_t vx, uint32_t vy, uint32_t vz)
	{
		auto theClump = ClumpAt(vx >> 1, vy >> 1, vz >> 1);
		return &theClump->VoxelAt(vx & 0x1, vy & 0x1, vz & 0x1);
	}
}