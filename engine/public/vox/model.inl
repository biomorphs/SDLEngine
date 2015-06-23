/*
SDLEngine
Matt Hoyle
*/

namespace Vox
{
	// Helper for area iteration
	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	class Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::AreaIteratorParams
	{
	public:
		AreaIteratorParams(Model* m, glm::ivec3 block, glm::ivec3 clumpStart, glm::ivec3 clumpEnd, Math::Box3 clumpBounds)
			: m_target(m), m_blockIndex(block), m_clumpStartIndex(clumpStart), m_clumpEndIndex(clumpEnd), m_clumpBounds(clumpBounds)
		{
			SDE_ASSERT(m != nullptr);
			m_voxelSize = m->m_voxelDimensions;
		}
		~AreaIteratorParams()	{ }

		inline const glm::ivec3& BlockIndex() const				{ return m_blockIndex; }
		inline const glm::ivec3& ClumpStartIndices() const		{ return m_clumpStartIndex; }
		inline const glm::ivec3& ClumpEndIndices() const		{ return m_clumpEndIndex; }
		inline const Math::Box3& ClumpModelspaceBounds() const	{ return m_clumpBounds; }
		inline const glm::vec3& VoxelDimensions() const			{ return m_voxelSize; }

	private:
		Model* m_target;
		glm::ivec3 m_blockIndex;
		glm::ivec3 m_clumpStartIndex;
		glm::ivec3 m_clumpEndIndex;
		Math::Box3 m_clumpBounds;
		glm::vec3 m_voxelSize;
	};

	// caches block and clump to avoid thrashing page lookups,
	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	class Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::ClumpDataAccessor
	{
	public:
		ClumpDataAccessor(Model* target, glm::ivec3 blockIndex)
			: m_targetModel(target)
			, m_targetClump(nullptr)
			, m_targetBlock(nullptr)
			, m_targetBlockIndex(blockIndex)
		{
		}
		inline void SetClumpIndex(glm::ivec3 clumpIndex)
		{
			m_targetClump = nullptr;
			m_targetClumpIndex = clumpIndex;
		}
		inline typename BlockType::ClumpType* GetClump()
		{
			if (m_targetClump != nullptr)	// clump cached, fastpath
			{
				return m_targetClump;
			}

			if (m_targetBlock != nullptr)	// block cached, get the clump
			{
				m_targetClump = &m_targetBlock->ClumpAt(m_targetClumpIndex.x, m_targetClumpIndex.y, m_targetClumpIndex.z);
				return m_targetClump;
			}

			// No cached block or chunk, get them both now
			if (m_targetBlock == nullptr)
			{
				// Getting the target block will always succeed, since we early-out on empty blocks for read-only access
				m_targetBlock = m_targetModel->m_voxelData.BlockAt(m_targetBlockIndex);
				m_targetClump = &m_targetBlock->ClumpAt(m_targetClumpIndex.x, m_targetClumpIndex.y, m_targetClumpIndex.z);
			}

			return m_targetClump;
		}
	private:
		Model* m_targetModel;
		typename Model::BlockType* m_targetBlock;
		typename Model::BlockType::ClumpType* m_targetClump;
		glm::ivec3 m_targetBlockIndex;
		glm::ivec3 m_targetClumpIndex;
	};

	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::Model()
	{
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::~Model()
	{
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	void Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::SetVoxelSize(glm::vec3 voxelSize)
	{
		m_voxelDimensions = voxelSize;
		m_blockDimensions = m_voxelDimensions * 2.0f * (float)BlockType::BlockDimensions;
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	glm::ivec3 Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::ModelspaceToBlockIndices(glm::vec3 coordinate)
	{
		return glm::floor(coordinate / m_blockDimensions);
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	void Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::IterateClumpsInArea(AreaIteratorParams& params, ClumpIterator iterator)
	{
		ClumpDataAccessor clumpPolicy(this, params.BlockIndex());

		// pre-calculate what we can
		const glm::vec3 clumpSize = params.VoxelDimensions() * 2.0f;
		const glm::vec3 voxelCenterOffset = params.VoxelDimensions() / 2.0f;
		for (int32_t cz = params.ClumpStartIndices().z; cz < params.ClumpEndIndices().z; ++cz)
		{
			for (int32_t cy = params.ClumpStartIndices().y; cy < params.ClumpEndIndices().y; ++cy)
			{
				for (int32_t cx = params.ClumpStartIndices().x; cx < params.ClumpEndIndices().x; ++cx)
				{
					const glm::vec3 clumpOrigin = params.ClumpModelspaceBounds().Min() + (glm::vec3(cx, cy, cz) * clumpSize);
					const glm::ivec3 clumpIndex(cx, cy, cz);
					clumpPolicy.SetClumpIndex(clumpIndex);
					iterator(clumpPolicy, clumpOrigin, params.VoxelDimensions(), voxelCenterOffset);
				}
			}
		}
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	void Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::IterateForArea(Math::Box3 modelSpaceBounds, IteratorAccess itType, ClumpIterator iterator)
	{
		// Find block indices for iteration
		const glm::ivec3 c_startBlockIndices = ModelspaceToBlockIndices(modelSpaceBounds.Min());
		const glm::ivec3 c_endBlockIndices = ModelspaceToBlockIndices(modelSpaceBounds.Max());
		const glm::vec3 c_clumpSize = m_voxelDimensions * 2.0f;
		const glm::ivec3 c_clumpsPerBlockVec(c_clumpsPerBlock);

		// For each block that matches, populate intersecting clumps
		for (int32_t bx = c_startBlockIndices.x; bx <= c_endBlockIndices.x; ++bx)
		{
			for (int32_t by = c_startBlockIndices.y; by <= c_endBlockIndices.y; ++by)
			{
				for (int32_t bz = c_startBlockIndices.z; bz <= c_endBlockIndices.z; ++bz)
				{
					const glm::ivec3 blockIndex(bx, by, bz);

					// If we are doing read-only iteration, and no block exists, then skip it
					if (itType == IteratorAccess::ReadOnly)
					{
						if (m_voxelData.BlockAt(blockIndex, false) == nullptr)
						{
							continue;
						}
					}

					// calculate the block area we need to update
					const glm::vec3 blockBoundsMin = m_blockDimensions * glm::vec3(bx, by, bz);
					const glm::vec3 blockBoundsMax = blockBoundsMin + m_blockDimensions;
					const glm::vec3 blockIterStart = glm::max(blockBoundsMin, modelSpaceBounds.Min());
					const glm::vec3 blockIterEnd = glm::min(blockBoundsMax, modelSpaceBounds.Max());
					const Math::Box3 clumpBounds(blockIterStart, blockIterEnd);

					// finally(!), calculate the clump range and clamp to proper range
					const glm::ivec3 clumpStartIndices = glm::floor((blockIterStart - blockBoundsMin) / c_clumpSize);
					glm::ivec3 clumpEndIndices = glm::floor((blockIterEnd - blockBoundsMin) / c_clumpSize);
					clumpEndIndices = glm::min(clumpEndIndices, c_clumpsPerBlockVec);
					
					AreaIteratorParams iteratorParams(this, blockIndex, clumpStartIndices, clumpEndIndices, clumpBounds);
					IterateClumpsInArea(iteratorParams, iterator);
				}
			}
		}

		//SDE_LOG("%lld blocks, %lld bytes after population", m_voxelData.TotalBlocks(), m_voxelData.TotalVoxelMemory());
	}
}