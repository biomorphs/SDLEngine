/*
SDLEngine
Matt Hoyle
*/

namespace Vox
{
	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	class Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::ModelDataAccessor
	{
	public:
		ModelDataAccessor(const Model& target)
			: m_target(target) 
			, m_cachedBlock(nullptr)
			, m_lastBlockIndex(INT32_MIN)
		{ }

		inline const typename BlockType* GetBlockAt(const glm::ivec3& blockIndex)
		{
			if (m_lastBlockIndex != blockIndex)
			{
				m_cachedBlock = m_target.m_voxelData.BlockAt(blockIndex);
				m_lastBlockIndex = blockIndex;
			}
			return m_cachedBlock;
		}

		inline const typename VoxelDataType GetVoxelAt(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex)
		{
			if (m_lastBlockIndex != blockIndex)
			{
				m_cachedBlock = m_target.m_voxelData.BlockAt(blockIndex);
				m_lastBlockIndex = blockIndex;
			}

			if (m_cachedBlock != nullptr)
			{
				return m_cachedBlock->VoxelAt(voxelIndex.x, voxelIndex.y, voxelIndex.z);
			}
			return 0;
		}

		inline const typename VoxelDataType GetVoxelNeighbour(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex, const glm::ivec3& voxelOffset)
		{
			// fixup block index and voxel index so we can support negative offsets
			glm::ivec3 thisVoxelIndex;
			glm::ivec3 thisBlockIndex;
			FixupVNIndex(voxelIndex.x + voxelOffset.x, blockIndex.x, thisBlockIndex.x, thisVoxelIndex.x);
			FixupVNIndex(voxelIndex.y + voxelOffset.y, blockIndex.y, thisBlockIndex.y, thisVoxelIndex.y);
			FixupVNIndex(voxelIndex.z + voxelOffset.z, blockIndex.z, thisBlockIndex.z, thisVoxelIndex.z);

			if (m_lastBlockIndex != thisBlockIndex)
			{
				m_cachedBlock = m_target.m_voxelData.BlockAt(thisBlockIndex);
				m_lastBlockIndex = thisBlockIndex;
			}

			if (m_cachedBlock != nullptr)
			{
				return m_cachedBlock->VoxelAt(thisVoxelIndex.x, thisVoxelIndex.y, thisVoxelIndex.z);
			}
			return 0;
		}

		inline void GetMooreNeighbours(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex, VoxelDataType (&data)[3][3][3])
		{
			glm::ivec3 thisVoxelIndex;
			glm::ivec3 thisBlockIndex;
			for (int32_t vZ = -1; vZ < 2; ++vZ)
			{
				FixupVNIndex(voxelIndex.z + vZ, blockIndex.z, thisBlockIndex.z, thisVoxelIndex.z);
				for (int32_t vY = -1; vY < 2; ++vY)
				{
					FixupVNIndex(voxelIndex.y + vY, blockIndex.y, thisBlockIndex.y, thisVoxelIndex.y);
					for (int32_t vX = -1; vX < 2; ++vX)
					{
						FixupVNIndex(voxelIndex.x + vX, blockIndex.x, thisBlockIndex.x, thisVoxelIndex.x);
						data[vX+1][vY+1][vZ+1] = GetVoxelAt(thisBlockIndex, thisVoxelIndex);
					}
				}
			}
		}
	private:
		inline void FixupVNIndex(int32_t voxelIndex, int32_t blockIndex, int32_t& blockIndexOut, int32_t& voxelIndexOut) const
		{
			// handles block boundaries and adjusts block / voxel indices as appropriate
			if (voxelIndex >= 0 && voxelIndex < t_blockDimensionVx)
			{
				blockIndexOut = blockIndex;
				voxelIndexOut = voxelIndex;
			}
			else if (voxelIndex < 0)
			{
				blockIndexOut = blockIndex - 1;
				voxelIndexOut = t_blockDimensionVx - 1;
			}
			else
			{
				blockIndexOut = blockIndex + 1;
				voxelIndexOut = 0;
			}
		}
		const Model& m_target;

		// caching of blocks to speed up lookups
		glm::ivec3 m_lastBlockIndex;
		const typename Model::BlockType* m_cachedBlock;
	};

	// Helper for area iteration
	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	class Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::AreaIteratorParams
	{
	public:
		AreaIteratorParams(Model* m, const glm::ivec3& block, const glm::ivec3& clumpStart, const glm::ivec3& clumpEnd, const Math::Box3& clumpBounds)
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
		ClumpDataAccessor(Model* target, const glm::ivec3& blockIndex)
			: m_targetModel(target)
			, m_targetClump(nullptr)
			, m_targetBlock(nullptr)
			, m_targetBlockIndex(blockIndex)
		{
		}
		inline void SetClumpIndex(const glm::ivec3& clumpIndex)
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
	void Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::SetVoxelSize(const glm::vec3& voxelSize)
	{
		m_voxelDimensions = voxelSize;
		m_blockDimensions = m_voxelDimensions * 2.0f * (float)BlockType::BlockDimensions;
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	glm::ivec3 Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::ModelspaceToBlockIndices(const glm::vec3& coordinate) const
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
					const glm::ivec3 clumpIndex(cx, cy, cz);
					const glm::vec3 clumpFromOrigin = glm::vec3(clumpIndex - params.ClumpStartIndices());
					const glm::vec3 clumpOrigin = params.ClumpModelspaceBounds().Min() + (clumpFromOrigin * clumpSize);
					
					clumpPolicy.SetClumpIndex(clumpIndex);
					iterator(clumpPolicy, clumpOrigin, params.VoxelDimensions(), voxelCenterOffset);
				}
			}
		}
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	void Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::
		GetBlockIterationParameters(const Math::Box3& modelSpaceBounds, glm::ivec3& start, glm::ivec3& end)  const
	{
		start = ModelspaceToBlockIndices(modelSpaceBounds.Min());
		end = ModelspaceToBlockIndices(modelSpaceBounds.Max());
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	void Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::
		GetClumpIterationParameters(const glm::ivec3& blockIndex, const Math::Box3& modelSpaceBounds, glm::ivec3& start, glm::ivec3& end)  const
	{
		const glm::vec3 c_clumpSize = m_voxelDimensions * 2.0f;
		const glm::ivec3 c_clumpsPerBlockVec(c_clumpsPerBlock);

		// calculate the block area we need to update
		const glm::vec3 blockBoundsMin = m_blockDimensions * glm::vec3(blockIndex);
		const glm::vec3 blockBoundsMax = blockBoundsMin + m_blockDimensions;
		const glm::vec3 blockIterStart = glm::max(blockBoundsMin, modelSpaceBounds.Min());
		const glm::vec3 blockIterEnd = glm::min(blockBoundsMax, modelSpaceBounds.Max());
		const Math::Box3 clumpBounds(blockIterStart, blockIterEnd);

		// calculate the clump range and clamp to proper range
		start = glm::floor((blockIterStart - blockBoundsMin) / c_clumpSize);
		glm::ivec3 clumpEndIndices = glm::floor((blockIterEnd - blockBoundsMin) / c_clumpSize);
		end = glm::min(clumpEndIndices, c_clumpsPerBlockVec);
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	glm::vec3 Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::
		GetVoxelCenterPosition(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex)  const
	{
		const glm::vec3 blockPosition = glm::vec3(blockIndex) * m_blockDimensions;
		const glm::vec3 voxelCornerPos = blockPosition + (glm::vec3(voxelIndex) * m_voxelDimensions);
		return voxelCornerPos + (m_voxelDimensions * 0.5f);
	}

	template< class VoxelDataType, uint32_t t_blockDimensionVx, class BlockAllocator >
	void Model<VoxelDataType, t_blockDimensionVx, BlockAllocator>::
		IterateForArea(Math::Box3 modelSpaceBounds, IteratorAccess itType, ClumpIterator iterator)
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