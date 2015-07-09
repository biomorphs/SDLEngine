/*
SDLEngine
Matt Hoyle
*/
#pragma once

namespace Vox
{
	template<class ModelType>
	inline glm::vec3 ModelAreaDataWriterParams<ModelType>::VoxelPosition(int32_t vx, int32_t vy, int32_t vz) const
	{
		return m_blockBounds.Min() + (m_voxelSize * glm::vec3(vx, vy, vz)) + (m_voxelSize * 0.5f);
	}

	template<class ModelType>
	inline void ModelAreaDataWriterParams<ModelType>::WriteVoxel(int32_t vx, int32_t vy, int32_t vz, typename const ModelType::VoxDataType& data)
	{
		m_writer->WriteVoxel(m_blockIndex, glm::ivec3(vx, vy, vz), data);
	}

	template<class ModelType>
	inline ModelAreaDataWriter<ModelType>::ModelAreaDataWriter(ModelType& model)
		: m_target(model)
	{
	}
	
	template<class ModelType>
	inline ModelAreaDataWriter<ModelType>::~ModelAreaDataWriter()
	{
	}

	template<class ModelType>
	inline void ModelAreaDataWriter<ModelType>::WriteArea(const Math::Box3& modelSpaceBounds, AreaCallback callback)
	{
		glm::ivec3 startBlockIndices, endBlockIndices;
		m_target.GetBlockIterationParameters(modelSpaceBounds, startBlockIndices, endBlockIndices);

		// For each block that matches, calculate the range of voxels, and run the write callback
		for (int32_t bx = startBlockIndices.x; bx <= endBlockIndices.x; ++bx)
		{
			for (int32_t by = startBlockIndices.y; by <= endBlockIndices.y; ++by)
			{
				for (int32_t bz = startBlockIndices.z; bz <= endBlockIndices.z; ++bz)
				{
					const glm::ivec3 blockIndex(bx, by, bz);

					glm::ivec3 voxelStartIndices, voxelEndIndices;
					m_target.GetVoxelIterationParameters(blockIndex, modelSpaceBounds, voxelStartIndices, voxelEndIndices);

					// calculate the block area we need to update
					const glm::vec3 blockBoundsMin = m_target.GetBlockSize() * glm::vec3(bx, by, bz);
					const glm::vec3 blockBoundsMax = blockBoundsMin + m_target.GetBlockSize();
					const Math::Box3 blockBounds(blockBoundsMin, blockBoundsMax);
					if (voxelStartIndices != voxelEndIndices)
					{
						ModelDataWriter<ModelType> dataWriter(m_target);
						ModelAreaDataWriterParams<ModelType> areaParams(&dataWriter, blockIndex, blockBounds, m_target.GetVoxelSize(),
																		voxelStartIndices, voxelEndIndices);
						callback(areaParams);
					}
				}
			}
		}
	}
}