/*
SDLEngine
Matt Hoyle
*/

namespace Vox
{
	template<class ModelType>
	GreedyQuadExtractor<ModelType>::GreedyQuadExtractor(ModelType& targetModel)
		: m_targetModel(targetModel)
	{
		// slice masks can be allocated straight awey, since we just need to handle
		// voxels per block * 2 (one mask per direction for a slice)
		const size_t voxelsPerBlock = ModelType::c_clumpsPerBlock * 2;
		m_sliceMaskPositive.resize(voxelsPerBlock * voxelsPerBlock);
		m_sliceMaskNegative.resize(voxelsPerBlock * voxelsPerBlock);
	}

	template<class ModelType>
	GreedyQuadExtractor<ModelType>::~GreedyQuadExtractor()
	{

	}

	template<class ModelType>
	void GreedyQuadExtractor<ModelType>::ResetSliceMasks()
	{
		memset(m_sliceMaskPositive.data(), 0, sizeof(MaskType) * m_sliceMaskPositive.size());
		memset(m_sliceMaskNegative.data(), 0, sizeof(MaskType) * m_sliceMaskNegative.size());
	}

	template<class ModelType>
	void GreedyQuadExtractor<ModelType>::ClearSliceMask(std::vector<MaskType>&mask, int32_t u, int32_t v, int32_t uMax, int32_t vMax)
	{
		for (int32_t vv = v;vv < vMax;++vv)
		{
			memset(&MaskVal(mask,u,vv), 0, sizeof(MaskType) * (uMax - u));
		}
	}

	template<class ModelType>
	typename GreedyQuadExtractor<ModelType>::MaskType& GreedyQuadExtractor<ModelType>::MaskVal(std::vector<MaskType>&mask, int32_t u, int32_t v)
	{
		const size_t voxelsPerBlock = ModelType::c_clumpsPerBlock * 2;
		return mask[u + (v * voxelsPerBlock)];
	}

	template<class ModelType>
	const typename GreedyQuadExtractor<ModelType>::MaskType& GreedyQuadExtractor<ModelType>::MaskVal(const std::vector<MaskType>&mask, int32_t u, int32_t v) const
	{
		const size_t voxelsPerBlock = ModelType::c_clumpsPerBlock * 2;
		return mask[u + (v * voxelsPerBlock)];
	}

	template<class ModelType>
	void GreedyQuadExtractor<ModelType>::CalculateMergedQuadsFromMask(const std::vector<MaskType>& mask, int32_t u, int32_t v, int32_t& quadEndU, int32_t& quadEndV)
	{
		const int32_t c_maxMask = typename ModelType::c_clumpsPerBlock * 2;

		// we run across the u axis, until we can't merge any more
		int32_t endU, endV;
		for (endU = u + 1; endU < c_maxMask; ++endU)
		{
			if (MaskVal(mask,endU,v) == false)
			{
				break;
			}
		}
		quadEndU = endU;

		// run 'up' the v axis, merging when an entire span matches the u -> quadEnd critera
		for (endV = v + 1; endV < c_maxMask; ++endV)
		{
			bool canMerge = true;
			for (int32_t i = u; i < endU; ++i)
			{
				if (MaskVal(mask,i,endV) == false)
				{
					canMerge = false;
					break;
				}
			}
			if (!canMerge)
			{
				break;
			}
		}
		quadEndV = endV;
	}

	template<class ModelType>
	void GreedyQuadExtractor<ModelType>::ProcessMaskAndBuildQuads(const glm::ivec3& blockIndex, int32_t slice, std::vector<MaskType>&mask)
	{
		glm::vec3 blockOrigin = glm::vec3(blockIndex) * (m_targetModel.GetVoxelSize() * (2.0f * ModelType::c_clumpsPerBlock));
		const int32_t c_maxMask = typename ModelType::c_clumpsPerBlock * 2;
		for (int32_t v = 0; v < c_maxMask; ++v)
		{
			for (int32_t u = 0; u < c_maxMask; ++u)
			{
				if (MaskVal(mask,u,v) == false)
				{
					continue;
				}
				int32_t quadEndU, quadEndV;
				CalculateMergedQuadsFromMask(mask, u, v, quadEndU, quadEndV);

				// at this point, we have a u->quadEndU, v->quadEndV pair that describes a quad. so add the damn thing!
				glm::vec3 voxelSize = m_targetModel.GetVoxelSize();
				glm::vec3 voxelOrigin = blockOrigin + glm::vec3(u * voxelSize.x, v * voxelSize.y, slice * voxelSize.z);
				glm::vec3 quadTopCorner = glm::vec3((quadEndU-u) * voxelSize.x, (quadEndV-v) * voxelSize.y, 0.0f);

				QuadDescriptor newQuad;
				newQuad.m_vertices[0] = voxelOrigin;
				newQuad.m_vertices[1] = voxelOrigin + glm::vec3(quadTopCorner.x, 0.0f, 0.0f);
				newQuad.m_vertices[2] = voxelOrigin + glm::vec3(quadTopCorner.x, quadTopCorner.y, 0.0f);
				newQuad.m_vertices[3] = voxelOrigin + glm::vec3(0.0f, quadTopCorner.y, 0.0f);
				m_quads.push_back(newQuad);

				// finally, clear out the mask so we dont process these entries later
				ClearSliceMask(mask, u, v, quadEndU, quadEndV);
			}
		}
	}

	template<class ModelType>
	void GreedyQuadExtractor<ModelType>::ExtractMeshesAlongZ(const glm::ivec3& blockIndex, const glm::ivec3& startClump, const glm::ivec3& endClump)
	{
		ModelType::ModelDataAccessor dataAccessor(m_targetModel);

		// skip blocks with no data
		if (dataAccessor.GetBlockAt(blockIndex) == nullptr)
		{
			return;
		}

		// run along z axis, building a mask for each slice
		const glm::ivec3 voxelStartIndices = startClump * 2;
		const glm::ivec3 voxelEndIndices = endClump * 2;
		bool processBlockData = false;
		for (int32_t vz = voxelStartIndices.z; vz < voxelEndIndices.z; ++vz)
		{
			ResetSliceMasks();
			for (int32_t vy = voxelStartIndices.y; vy < voxelEndIndices.y; ++vy)
			{
				for (int32_t vx = voxelStartIndices.x; vx < voxelEndIndices.x; ++vx)
				{
					const glm::ivec3 voxelIndex(vx, vy, vz);

					// get the voxel, as well as its neighbours
					ModelType::BlockType::ClumpType::VoxelDataType thisVoxel = dataAccessor.GetVoxelAt(blockIndex, voxelIndex);
					if (thisVoxel == 0)
					{
						continue;
					}

					ModelType::BlockType::ClumpType::VoxelDataType voxelData[2];
					voxelData[0] = dataAccessor.GetVoxelNeighbour(blockIndex, voxelIndex, glm::ivec3(0, 0, -1));					
					voxelData[1] = dataAccessor.GetVoxelNeighbour(blockIndex, voxelIndex, glm::ivec3(0, 0, 1));

					// now determine whether a quad should be here for each direction
					if(voxelData[0] == 0)
					{
						MaskVal(m_sliceMaskPositive, vx, vy) = 1;
						processBlockData = true;
					}
					if (voxelData[1] == 0)
					{
						MaskVal(m_sliceMaskNegative, vx, vy) = 1;
						processBlockData = true;
					}
				}
			}
			// Now we do the greedy part, running through the masks, combining mergable quads
			if (processBlockData)
			{
				ProcessMaskAndBuildQuads(blockIndex, vz, m_sliceMaskPositive);
				ProcessMaskAndBuildQuads(blockIndex, vz + 1, m_sliceMaskNegative);
			}
		}
	}

	template<class ModelType>
	void GreedyQuadExtractor<ModelType>::ExtractQuads(const Math::Box3& modelSpaceBounds)
	{
		// first, calculate block indices
		glm::ivec3 blockStartIndices;
		glm::ivec3 blockEndIndices;
		m_targetModel.GetBlockIterationParameters(modelSpaceBounds, blockStartIndices, blockEndIndices);

		// for simplicities sake, we only do greedy meshing for individual blocks
		// although the meshes are less optimal, it uses much less memory, and is way easier
		// to implement - the meshes will still be pretty damn good compared to naive meshing
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

					// extract quads for each axis
					ExtractMeshesAlongZ(blockIndex, clumpStartIndices, clumpEndIndices);
				}
			}
		}
	}
}