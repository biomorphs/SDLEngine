/*
SDLEngine
Matt Hoyle
*/

#include "model_data_reader.h"

namespace Vox
{
	template<class ModelType>
	SimpleQuadExtractor<ModelType>::SimpleQuadExtractor(const ModelType& targetModel)
		: m_targetModel(targetModel)
	{

	}

	template<class ModelType>
	SimpleQuadExtractor<ModelType>::~SimpleQuadExtractor()
	{

	}

	template<class ModelType>
	void SimpleQuadExtractor<ModelType>::ExtractQuadsForVoxel(const typename ModelType::VoxDataType(&data)[3][3][3],
		const glm::vec3& voxelCenter)
	{
		const glm::vec3 c_halfVoxelSize = m_targetModel.GetVoxelSize() * 0.5f;

		if (data[1][1][0] == 0)	//z-1, 'front' face
		{
			QuadDescriptor quad;
			quad.m_vertices[0] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, -c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[1] = voxelCenter + glm::vec3(c_halfVoxelSize.x, -c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[2] = voxelCenter + glm::vec3(c_halfVoxelSize.x, c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[3] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, c_halfVoxelSize.y, -c_halfVoxelSize.z);
			m_quads.push_back(quad);
		}

		if (data[1][1][2] == 0)	//z+1, 'back' face
		{
			QuadDescriptor quad;
			quad.m_vertices[0] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, -c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[1] = voxelCenter + glm::vec3(c_halfVoxelSize.x, -c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[2] = voxelCenter + glm::vec3(c_halfVoxelSize.x, c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[3] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, c_halfVoxelSize.y, c_halfVoxelSize.z);
			m_quads.push_back(quad);
		}

		if (data[0][1][1] == 0)	//x-1, 'left' face
		{
			QuadDescriptor quad;
			quad.m_vertices[0] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, -c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[1] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, -c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[2] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[3] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, c_halfVoxelSize.y, -c_halfVoxelSize.z);
			m_quads.push_back(quad);
		}

		if (data[2][1][1] == 0)	//x-1, 'right' face
		{
			QuadDescriptor quad;
			quad.m_vertices[0] = voxelCenter + glm::vec3(c_halfVoxelSize.x, -c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[1] = voxelCenter + glm::vec3(c_halfVoxelSize.x, -c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[2] = voxelCenter + glm::vec3(c_halfVoxelSize.x, c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[3] = voxelCenter + glm::vec3(c_halfVoxelSize.x, c_halfVoxelSize.y, -c_halfVoxelSize.z);
			m_quads.push_back(quad);
		}

		if (data[1][2][1] == 0)	//y+1, 'top' face
		{
			QuadDescriptor quad;
			quad.m_vertices[0] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[1] = voxelCenter + glm::vec3(c_halfVoxelSize.x, c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[2] = voxelCenter + glm::vec3(c_halfVoxelSize.x, c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[3] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, c_halfVoxelSize.y, c_halfVoxelSize.z);
			m_quads.push_back(quad);
		}

		if (data[1][0][1] == 0)	//y+1, 'top' face
		{
			QuadDescriptor quad;
			quad.m_vertices[0] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, -c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[1] = voxelCenter + glm::vec3(c_halfVoxelSize.x, -c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[2] = voxelCenter + glm::vec3(c_halfVoxelSize.x, -c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[3] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, -c_halfVoxelSize.y, c_halfVoxelSize.z);
			m_quads.push_back(quad);
		}
	}

	template<class ModelType>
	void SimpleQuadExtractor<ModelType>::ExtractQuadsFromBlock(const glm::ivec3& blockIndex, const glm::ivec3& startVoxel, const glm::ivec3& endVoxel)
	{
		ModelDataReader<ModelType> dataAccessor(m_targetModel);

		// moore neighbourhood storage
		typename ModelType::VoxDataType neighbourhood[3][3][3];

		const glm::vec3 voxelSize = m_targetModel.GetVoxelSize();
		for (int32_t vz = startVoxel.z; vz < endVoxel.z; ++vz)
		{
			for (int32_t vy = startVoxel.y; vy < endVoxel.y; ++vy)
			{
				for (int32_t vx = startVoxel.x; vx < endVoxel.x; ++vx)
				{
					const glm::ivec3 voxelIndex(vx, vy, vz);

					// fastpath, if no voxel data, then we dont care about the neighbourhood!
					if (dataAccessor.VoxelAt(blockIndex, voxelIndex) == 0)
					{
						continue;
					}

					const glm::vec3 voxelCenter = m_targetModel.GetVoxelCenterPosition(blockIndex, voxelIndex);

					// extract von-neumann neighbourhood for this voxel
					dataAccessor.MooreNeighbours(blockIndex, voxelIndex, neighbourhood);

					// finally, get the quads
					ExtractQuadsForVoxel(neighbourhood, voxelCenter);
				}
			}
		}
	}

	template<class ModelType>
	void SimpleQuadExtractor<ModelType>::ExtractQuads(const Math::Box3& modelSpaceBounds)
	{
		// first, calculate block indices
		glm::ivec3 blockStartIndices;
		glm::ivec3 blockEndIndices;
		m_targetModel.GetBlockIterationParameters(modelSpaceBounds, blockStartIndices, blockEndIndices);

		// for each block we are interested in
		for (int32_t bx = blockStartIndices.x; bx <= blockEndIndices.x; ++bx)
		{
			for (int32_t by = blockStartIndices.y; by <= blockEndIndices.y; ++by)
			{
				for (int32_t bz = blockStartIndices.z; bz <= blockEndIndices.z; ++bz)
				{
					// calculate voxel area we are interested in
					glm::ivec3 blockIndex(bx, by, bz);
					glm::ivec3 voxelStartIndices, voxelEndIndices;
					m_targetModel.GetVoxelIterationParameters(blockIndex, modelSpaceBounds, voxelStartIndices, voxelEndIndices);

					// pull out quads
					ExtractQuadsFromBlock(blockIndex, voxelStartIndices, voxelEndIndices);
				}
			}
		}
	}
}