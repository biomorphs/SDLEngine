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
	void QuadExtractor<ModelType>::ExtractQuadsFromBlock(const glm::ivec3& blockIndex, const glm::ivec3& startClump, const glm::ivec3& endClump)
	{
		ModelType::ModelDataAccessor dataAccessor(m_targetModel);

		// von-neumann neighbourhood storage
		typename ModelType::BlockType::ClumpType::VoxelDataType neighbourhood[3][3][3];

		// calculate voxel indices so we don't need to look through clumps separately (avoid tight loop)
		const glm::ivec3 voxelStartIndices = startClump * 2;
		const glm::ivec3 voxelEndIndices = endClump * 2;
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

					// extract von-neumann neighbourhood for this voxel
					dataAccessor.GetVonNeumann(blockIndex, voxelIndex, neighbourhood);
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