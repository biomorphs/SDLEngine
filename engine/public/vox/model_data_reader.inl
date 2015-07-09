/*
SDLEngine
Matt Hoyle
*/

namespace Vox
{
	template<class ModelType>
	inline ModelDataReader<ModelType>::ModelDataReader(const ModelType& model)
		: m_target(model)
		, m_cachedBlockIndex(-INT32_MAX)
		, m_cachedBlock(nullptr)
	{
	}

	template<class ModelType>
	inline ModelDataReader<ModelType>::~ModelDataReader()
	{
	}

	template<class ModelType>
	const typename ModelType::BlockType* ModelDataReader<ModelType>::GetBlockAndCache(const glm::ivec3& blockIndex)
	{
		if (m_cachedBlockIndex != blockIndex)
		{
			m_cachedBlock = m_target.BlockAt(blockIndex);
			m_cachedBlockIndex = blockIndex;
		}
		return m_cachedBlock;
	}

	template<class ModelType>
	inline bool ModelDataReader<ModelType>::HasBlockData(const glm::ivec3& blockIndex) const
	{
		return m_target.BlockAt(blockIndex) != nullptr;
	}

	template<class ModelType>
	inline typename ModelDataReader<ModelType>::VoxelDataType 
		ModelDataReader<ModelType>::VoxelAt(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex)
	{
		auto thisBlock = GetBlockAndCache(blockIndex);
		if (thisBlock != nullptr)
		{
			return thisBlock->VoxelAt(voxelIndex.x, voxelIndex.y, voxelIndex.z);
		}
		return 0;
	}

	template<class ModelType>
	inline typename ModelDataReader<ModelType>::VoxelDataType 
		ModelDataReader<ModelType>::VoxelNeighbour(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex, const glm::ivec3& offset)
	{
		glm::ivec3 thisVoxelIndex;
		glm::ivec3 thisBlockIndex;
		FixupNeighbourIndex(voxelIndex.x + offset.x, blockIndex.x, thisBlockIndex.x, thisVoxelIndex.x);
		FixupNeighbourIndex(voxelIndex.y + offset.y, blockIndex.y, thisBlockIndex.y, thisVoxelIndex.y);
		FixupNeighbourIndex(voxelIndex.z + offset.z, blockIndex.z, thisBlockIndex.z, thisVoxelIndex.z);

		auto thisBlock = GetBlockAndCache(thisBlockIndex);
		if (thisBlock != nullptr)
		{
			return thisBlock->VoxelAt(thisVoxelIndex.x, thisVoxelIndex.y, thisVoxelIndex.z);
		}
		return 0;
	}

	template<class ModelType>
	inline void ModelDataReader<ModelType>::MooreNeighbours(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex, VoxelDataType(&data)[3][3][3])
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
					data[vX + 1][vY + 1][vZ + 1] = GetVoxelAt(thisBlockIndex, thisVoxelIndex);
				}
			}
		}
	}

	template<class ModelType>
	inline void ModelDataReader<ModelType>::
		FixupNeighbourIndex(int32_t voxelIndex, int32_t blockIndex, int32_t& blockIndexOut, int32_t& voxelIndexOut) const
	{
		// handles block boundaries and adjusts block / voxel indices as appropriate
		if (voxelIndex >= 0 && voxelIndex < ModelType::BlockType::VoxelDimensions)
		{
			blockIndexOut = blockIndex;
			voxelIndexOut = voxelIndex;
		}
		else if (voxelIndex < 0)
		{
			blockIndexOut = blockIndex - 1;
			voxelIndexOut = ModelType::BlockType::VoxelDimensions - 1;
		}
		else
		{
			blockIndexOut = blockIndex + 1;
			voxelIndexOut = 0;
		}
	}
}