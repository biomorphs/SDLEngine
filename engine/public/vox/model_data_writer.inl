/*
SDLEngine
Matt Hoyle
*/

namespace Vox
{
	template<class ModelType>
	inline ModelDataWriter<ModelType>::ModelDataWriter(ModelType& model)
		: m_target(model)
		, m_cachedBlockIndex(-INT32_MAX)
		, m_cachedBlock(nullptr)
	{
	}

	template<class ModelType>
	inline ModelDataWriter<ModelType>::~ModelDataWriter()
	{
	}

	template<class ModelType>
	typename ModelType::BlockType* ModelDataWriter<ModelType>::GetBlockAndCache(const glm::ivec3& blockIndex)
	{
		if (m_cachedBlockIndex != blockIndex)
		{
			m_cachedBlock = m_target.BlockAt(blockIndex, true);
			m_cachedBlockIndex = blockIndex;
		}
		return m_cachedBlock;
	}

	template<class ModelType>
	inline void ModelDataWriter<ModelType>::WriteVoxel(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex, const VoxelDataType& data)
	{
		auto thisBlock = GetBlockAndCache(blockIndex);
		SDE_ASSERT(thisBlock, "Failed to get voxel for writing. OOM?");
		thisBlock->VoxelAt(voxelIndex.x, voxelIndex.y, voxelIndex.z) = data;
	}
}