/*
SDLEngine
Matt Hoyle
*/

namespace Vox
{
	template<class ModelType>
	QuadExtractor<ModelType>::QuadExtractor(ModelType& targetModel)
		: m_targetModel(targetModel)
	{

	}

	template<class ModelType>
	QuadExtractor<ModelType>::~QuadExtractor()
	{

	}

	template<class ModelType>
	void QuadExtractor<ModelType>::ExtractQuadsForVoxel(const typename ModelType::BlockType::ClumpType::VoxelDataType(&data)[3][3][3],
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
			m_zAxisQuads.push_back(quad);
		}

		if (data[1][1][2] == 0)	//z+1, 'back' face
		{
			QuadDescriptor quad;
			quad.m_vertices[0] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, -c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[1] = voxelCenter + glm::vec3(c_halfVoxelSize.x, -c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[2] = voxelCenter + glm::vec3(c_halfVoxelSize.x, c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[3] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, c_halfVoxelSize.y, c_halfVoxelSize.z);
			m_zAxisQuads.push_back(quad);
		}

		if (data[0][1][1] == 0)	//x-1, 'left' face
		{
			QuadDescriptor quad;
			quad.m_vertices[0] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, -c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[1] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, -c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[2] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[3] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, c_halfVoxelSize.y, -c_halfVoxelSize.z);
			m_yAxisQuads.push_back(quad);
		}

		if (data[2][1][1] == 0)	//x-1, 'right' face
		{
			QuadDescriptor quad;
			quad.m_vertices[0] = voxelCenter + glm::vec3(c_halfVoxelSize.x, -c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[1] = voxelCenter + glm::vec3(c_halfVoxelSize.x, -c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[2] = voxelCenter + glm::vec3(c_halfVoxelSize.x, c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[3] = voxelCenter + glm::vec3(c_halfVoxelSize.x, c_halfVoxelSize.y, -c_halfVoxelSize.z);
			m_yAxisQuads.push_back(quad);
		}

		if (data[1][2][1] == 0)	//y+1, 'top' face
		{
			QuadDescriptor quad;
			quad.m_vertices[0] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[1] = voxelCenter + glm::vec3(c_halfVoxelSize.x, c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[2] = voxelCenter + glm::vec3(c_halfVoxelSize.x, c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[3] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, c_halfVoxelSize.y, c_halfVoxelSize.z);
			m_xAxisQuads.push_back(quad);
		}

		if (data[1][0][1] == 0)	//y+1, 'top' face
		{
			QuadDescriptor quad;
			quad.m_vertices[0] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, -c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[1] = voxelCenter + glm::vec3(c_halfVoxelSize.x, -c_halfVoxelSize.y, -c_halfVoxelSize.z);
			quad.m_vertices[2] = voxelCenter + glm::vec3(c_halfVoxelSize.x, -c_halfVoxelSize.y, c_halfVoxelSize.z);
			quad.m_vertices[3] = voxelCenter + glm::vec3(-c_halfVoxelSize.x, -c_halfVoxelSize.y, c_halfVoxelSize.z);
			m_xAxisQuads.push_back(quad);
		}
	}

	template<class ModelType>
	void QuadExtractor<ModelType>::ExtractQuadsFromBlock(const glm::ivec3& blockIndex, const glm::ivec3& startClump, const glm::ivec3& endClump)
	{
		ModelType::ModelDataAccessor dataAccessor(m_targetModel);

		// moore neighbourhood storage
		typename ModelType::BlockType::ClumpType::VoxelDataType neighbourhood[3][3][3];

		// calculate voxel indices so we don't need to look through clumps separately (avoid tight loop)
		const glm::ivec3 voxelStartIndices = startClump * 2;
		const glm::ivec3 voxelEndIndices = endClump * 2;
		const glm::vec3 voxelSize = m_targetModel.GetVoxelSize();
		for (int32_t vz = voxelStartIndices.z; vz < voxelEndIndices.z; ++vz)
		{
			for (int32_t vy = voxelStartIndices.y; vy < voxelEndIndices.y; ++vy)
			{
				for (int32_t vx = voxelStartIndices.x; vx < voxelEndIndices.x; ++vx)
				{
					const glm::ivec3 voxelIndex(vx, vy, vz);

					// fastpath, if no voxel data, then we dont care about the neighbourhood!
					if (dataAccessor.GetVoxelAt(blockIndex, voxelIndex) == 0)
					{
						continue;
					}

					const glm::vec3 voxelCenter = m_targetModel.GetVoxelCenterPosition(blockIndex, voxelIndex);

					// extract von-neumann neighbourhood for this voxel
					dataAccessor.GetMooreNeighbours(blockIndex, voxelIndex, neighbourhood);

					// finally, get the quads
					ExtractQuadsForVoxel(neighbourhood, voxelCenter);
				}
			}
		}
	}

	template<class ModelType>
	void QuadExtractor<ModelType>::ExtractQuads(const Math::Box3& modelSpaceBounds)
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
					// calculate clump area we are interested in
					glm::ivec3 blockIndex(bx, by, bz);
					glm::ivec3 clumpStartIndices;
					glm::ivec3 clumpEndIndices;
					m_targetModel.GetClumpIterationParameters(blockIndex, modelSpaceBounds, clumpStartIndices, clumpEndIndices);

					// pull out quads
					ExtractQuadsFromBlock(blockIndex, clumpStartIndices, clumpEndIndices);
				}
			}
		}
	}
}