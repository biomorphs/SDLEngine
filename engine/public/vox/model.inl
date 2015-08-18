/*
SDLEngine
Matt Hoyle
*/

namespace Vox
{
	template< class DataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	Model<DataType, t_blockDimensionVx, BlockAllocator>::Model()
		: m_totalBounds(glm::vec3(100000.0f), glm::vec3(-100000.0f))
	{
	}

	template< class DataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	Model<DataType, t_blockDimensionVx, BlockAllocator>::~Model()
	{
	}

	template< class DataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	void Model<DataType, t_blockDimensionVx, BlockAllocator>::SetVoxelSize(const glm::vec3& voxelSize)
	{
		m_voxelDimensions = voxelSize;
		m_blockDimensions = voxelSize * glm::vec3(t_blockDimensionVx);
	}

	template< class DataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	glm::ivec3 Model<DataType, t_blockDimensionVx, BlockAllocator>::ModelspaceToBlockIndices(const glm::vec3& coordinate) const
	{
		return glm::floor(coordinate / m_blockDimensions);
	}

	template< class DataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	void Model<DataType, t_blockDimensionVx, BlockAllocator>::
		GetBlockIterationParameters(const Math::Box3& modelSpaceBounds, glm::ivec3& start, glm::ivec3& end)  const
	{
		start = ModelspaceToBlockIndices(modelSpaceBounds.Min());
		end = ModelspaceToBlockIndices(modelSpaceBounds.Max());
	}

	template< class DataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	void Model<DataType, t_blockDimensionVx, BlockAllocator>::
		GetVoxelIterationParameters(const glm::ivec3& blockIndex, const Math::Box3& modelSpaceBounds, glm::ivec3& start, glm::ivec3& end)  const
	{
		// calculate the block area we need to update
		const glm::vec3 blockBoundsMin = m_blockDimensions * glm::vec3(blockIndex);
		const glm::vec3 blockBoundsMax = blockBoundsMin + m_blockDimensions;
		const glm::vec3 blockIterStart = glm::max(blockBoundsMin, modelSpaceBounds.Min());
		const glm::vec3 blockIterEnd = glm::min(blockBoundsMax, modelSpaceBounds.Max());

		// calculate the voxel range we are interested in
		start = glm::floor((blockIterStart - blockBoundsMin) / m_voxelDimensions);
		glm::ivec3 voxelEndIndices = glm::floor((blockIterEnd - blockBoundsMin) / m_voxelDimensions);
		end = glm::min(voxelEndIndices, glm::ivec3(typename BlockType::VoxelDimensions));
	}

	template< class DataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	glm::vec3 Model<DataType, t_blockDimensionVx, BlockAllocator>::
		GetVoxelCenterPosition(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex)  const
	{
		const glm::vec3 blockPosition = glm::vec3(blockIndex) * m_blockDimensions;
		const glm::vec3 voxelCornerPos = blockPosition + (glm::vec3(voxelIndex) * m_voxelDimensions);
		return voxelCornerPos + (m_voxelDimensions * 0.5f);
	}

	template< class DataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	void Model<DataType, t_blockDimensionVx, BlockAllocator>::RemoveAllBlocks()
	{
		m_voxelData.Clear();
		m_totalBounds = Math::Box3(glm::vec3(100000.0f), glm::vec3(-100000.0f));
	}

	template< class DataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	void Model<DataType, t_blockDimensionVx, BlockAllocator>::
		PreallocateMemory(Math::Box3 modelSpaceBounds)
	{
		glm::ivec3 c_startBlockIndices = ModelspaceToBlockIndices(modelSpaceBounds.Min());
		glm::ivec3 c_endBlockIndices = ModelspaceToBlockIndices(modelSpaceBounds.Max());
		for (int32_t bx = c_startBlockIndices.x; bx <= c_endBlockIndices.x; ++bx)
		{
			for (int32_t by = c_startBlockIndices.y; by <= c_endBlockIndices.y; ++by)
			{
				for (int32_t bz = c_startBlockIndices.z; bz <= c_endBlockIndices.z; ++bz)
				{
					const glm::ivec3 blockIndex(bx, by, bz);
					m_voxelData.BlockAt(blockIndex, true);	// Force block creation
				}
			}
		}
	}

	template< class DataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	const typename Model<DataType, t_blockDimensionVx, BlockAllocator>::BlockType* 
		Model<DataType, t_blockDimensionVx, BlockAllocator>::BlockAt(const glm::ivec3& blockIndex) const
	{
		return m_voxelData.BlockAt(blockIndex); 
	}

	template< class DataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	typename Model<DataType, t_blockDimensionVx, BlockAllocator>::BlockType* 
		Model<DataType, t_blockDimensionVx, BlockAllocator>::BlockAt(const glm::ivec3& blockIndex, bool createNew)
	{ 
		if (createNew)
		{
			const glm::vec3 blockBoundsMin = m_blockDimensions * glm::vec3(blockIndex);
			const glm::vec3 blockBoundsMax = blockBoundsMin + m_blockDimensions;
			m_totalBounds.Min() = glm::min(m_totalBounds.Min(), blockBoundsMin);
			m_totalBounds.Max() = glm::max(m_totalBounds.Max(), blockBoundsMax);
		}
		return m_voxelData.BlockAt(blockIndex, createNew); 
	}
}