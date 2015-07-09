/*
SDLEngine
Matt Hoyle
*/

#include "model_data_reader.h"

namespace Vox
{
	template<class ModelType>
	GreedyQuadExtractor<ModelType>::GreedyQuadExtractor(const ModelType& targetModel)
		: m_targetModel(targetModel)
	{
		// slice masks can be allocated straight awey, since we just need to handle
		// voxels per block * 2 (one mask per direction for a slice)
		const size_t voxelsPerBlock = ModelType::BlockType::VoxelDimensions;
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
		SDE_ASSERT(u >= 0 && u < ModelType::BlockType::VoxelDimensions);
		SDE_ASSERT(v >= 0 && v < ModelType::BlockType::VoxelDimensions);
		SDE_ASSERT(uMax >= u && uMax <= ModelType::BlockType::VoxelDimensions);
		SDE_ASSERT(vMax >= v && vMax <= ModelType::BlockType::VoxelDimensions);
		for (int32_t vv = v;vv < vMax;++vv)
		{
			memset(&MaskVal(mask,u,vv), 0, sizeof(MaskType) * (uMax - u));
		}
	}

	template<class ModelType>
	typename GreedyQuadExtractor<ModelType>::MaskType& GreedyQuadExtractor<ModelType>::MaskVal(std::vector<MaskType>&mask, int32_t u, int32_t v)
	{
		SDE_ASSERT(u >= 0 && u < ModelType::BlockType::VoxelDimensions);
		SDE_ASSERT(v >= 0 && v < ModelType::BlockType::VoxelDimensions);
		return mask[u + (v * ModelType::BlockType::VoxelDimensions)];
	}

	template<class ModelType>
	const typename GreedyQuadExtractor<ModelType>::MaskType& GreedyQuadExtractor<ModelType>::MaskVal(const std::vector<MaskType>&mask, int32_t u, int32_t v) const
	{
		SDE_ASSERT(u >= 0 && u < ModelType::BlockType::VoxelDimensions);
		SDE_ASSERT(v >= 0 && v < ModelType::BlockType::VoxelDimensions);
		return mask[u + (v * ModelType::BlockType::VoxelDimensions)];
	}

	template<class ModelType>
	void GreedyQuadExtractor<ModelType>::CalculateMergedQuadsFromMask(const std::vector<MaskType>& mask, MaskType sourceVoxel, int32_t u, int32_t v, int32_t& quadEndU, int32_t& quadEndV)
	{
		const int32_t c_maxMask = ModelType::BlockType::VoxelDimensions;
		GreedyQuadVoxelInterpreter::Interpreter<ModelType::VoxelDataType> voxInterpreter;

		// we run across the u axis, until we can't merge any more
		int32_t endU, endV;
		for (endU = u + 1; endU < c_maxMask; ++endU)
		{
			if (!voxInterpreter.ShouldMergeQuad(sourceVoxel, MaskVal(mask,endU,v)))
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
				if (!voxInterpreter.ShouldMergeQuad(sourceVoxel, MaskVal(mask,i,endV)))
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
	inline glm::vec3 GreedyQuadExtractor<ModelType>::BuildQuadVertex(const glm::ivec3& sample, const glm::vec3& blockOrigin, const glm::vec3& voxSize, const glm::ivec3& sampleAxes)
	{
		glm::vec3 vertex;
		vertex[sampleAxes.x] = blockOrigin[sampleAxes.x] + (sample.x * voxSize[sampleAxes.x]);
		vertex[sampleAxes.y] = blockOrigin[sampleAxes.y] + (sample.y * voxSize[sampleAxes.y]);
		vertex[sampleAxes.z] = blockOrigin[sampleAxes.z] + (sample.z * voxSize[sampleAxes.z]);
		return vertex;
	}

	template<class ModelType>
	inline void GreedyQuadExtractor<ModelType>::BuildQuad(const QuadBuildParameters& params)
	{
		const int32_t c_frontFaceIndices[] = { 0,1,2,3 };		// ccw
		const int32_t c_backFaceIndices[] = { 0,3,2,1 };		// cw
		const int32_t* c_indices = params.m_backFace ? c_backFaceIndices : c_frontFaceIndices;
		const glm::vec3 voxelSize = m_targetModel.GetVoxelSize();
		
		QuadDescriptor newQuad;
		newQuad.m_vertices[c_indices[0]] = BuildQuadVertex(glm::ivec3(params.m_u, params.m_v, params.m_slice), params.m_blockOrigin, voxelSize, params.m_sampleAxes);
		newQuad.m_vertices[c_indices[1]] = BuildQuadVertex(glm::ivec3(params.m_uEnd, params.m_v, params.m_slice), params.m_blockOrigin, voxelSize, params.m_sampleAxes);
		newQuad.m_vertices[c_indices[2]] = BuildQuadVertex(glm::ivec3(params.m_uEnd, params.m_vEnd, params.m_slice), params.m_blockOrigin, voxelSize, params.m_sampleAxes);
		newQuad.m_vertices[c_indices[3]] = BuildQuadVertex(glm::ivec3(params.m_u, params.m_vEnd, params.m_slice), params.m_blockOrigin, voxelSize, params.m_sampleAxes);
		newQuad.m_sourceData = params.m_sourceVoxel;
		newQuad.m_normal = params.m_normal;
		m_quads.push_back(newQuad);
	}

	template<class ModelType>
	void GreedyQuadExtractor<ModelType>::ProcessMaskAndBuildQuads(const glm::ivec3& blockIndex, int32_t slice, std::vector<MaskType>&mask, bool backFace, int32_t sliceAxis)
	{
		QuadBuildParameters quadParameters;
		quadParameters.m_blockOrigin = glm::vec3(blockIndex) * m_targetModel.GetBlockSize();
		quadParameters.m_slice = slice;
		quadParameters.m_backFace = backFace;
		quadParameters.m_sampleAxes = glm::ivec3((sliceAxis + 1) % 3, (sliceAxis + 2) % 3, sliceAxis);
		quadParameters.m_normal = static_cast<QuadDescriptor::NormalDirection>((sliceAxis * 2) + (backFace ? 1 : 0));
		
		GreedyQuadVoxelInterpreter::Interpreter<ModelType::VoxelDataType> voxInterpreter;
		const int32_t c_maxMask = ModelType::BlockType::VoxelDimensions;
		for (int32_t v = 0; v < c_maxMask; ++v)
		{
			for (int32_t u = 0; u < c_maxMask; ++u)
			{
				auto thisVoxel = MaskVal(mask, u, v);
				if (!voxInterpreter.ShouldAddQuad(thisVoxel))
				{
					continue;
				}
				int32_t quadEndU, quadEndV;
				CalculateMergedQuadsFromMask(mask, thisVoxel, u, v, quadEndU, quadEndV);

				// at this point, we have a u->quadEndU, v->quadEndV pair that describes a quad. so add the damn thing!
				quadParameters.m_u = u;
				quadParameters.m_v = v;
				quadParameters.m_uEnd = quadEndU;
				quadParameters.m_vEnd = quadEndV;
				quadParameters.m_sourceVoxel = thisVoxel;
				BuildQuad(quadParameters);

				// finally, clear out the mask so we dont process these entries later
				ClearSliceMask(mask, u, v, quadEndU, quadEndV);
			}
		}
	}

	template<class ModelType>
	void GreedyQuadExtractor<ModelType>::ExtractMeshesAlongAxis(const glm::ivec3& blockIndex, const glm::ivec3& startVoxel, const glm::ivec3& endVoxel, int32_t sliceAxis)
	{
		ModelDataReader<ModelType> dataReader(m_targetModel);

		if (!dataReader.HasBlockData(blockIndex))	// skip blocks with no data
		{
			return;
		}

		const int32_t uAxis = (sliceAxis + 1) % 3;
		const int32_t vAxis = (sliceAxis + 2) % 3;
		glm::ivec3 sampleIndex(0);				// loops through slice,v,u, represents current voxel index
		glm::ivec3 neighbourDirection(0);		// used to shift coordinates when getting neighbours
		neighbourDirection[sliceAxis] = 1;
		for (sampleIndex[sliceAxis] = startVoxel[sliceAxis]; sampleIndex[sliceAxis] < endVoxel[sliceAxis]; ++sampleIndex[sliceAxis])
		{
			bool processBlockData = false;
			ResetSliceMasks();
			for (sampleIndex[vAxis] = startVoxel[vAxis]; sampleIndex[vAxis] < endVoxel[vAxis]; ++sampleIndex[vAxis])
			{
				for (sampleIndex[uAxis] = startVoxel[uAxis]; sampleIndex[uAxis] < endVoxel[uAxis]; ++sampleIndex[uAxis])
				{
					const glm::ivec3 voxelIndex(sampleIndex.x, sampleIndex.y, sampleIndex.z);

					// get the voxel, as well as its neighbours
					GreedyQuadVoxelInterpreter::Interpreter<ModelType::VoxelDataType> voxInterpreter;
					ModelType::VoxelDataType thisVoxel = dataReader.VoxelAt(blockIndex, voxelIndex);
					if (!voxInterpreter.ShouldAddQuad(thisVoxel))
					{
						continue;
					}
					ModelType::VoxelDataType voxelData[2];
					voxelData[0] = dataReader.VoxelNeighbour(blockIndex, voxelIndex, -neighbourDirection);
					voxelData[1] = dataReader.VoxelNeighbour(blockIndex, voxelIndex, neighbourDirection);

					// now determine whether a quad should be here for each direction
					if(!voxInterpreter.ShouldAddQuad(voxelData[0]))
					{
						MaskVal(m_sliceMaskNegative, sampleIndex[uAxis], sampleIndex[vAxis]) = thisVoxel;
						processBlockData = true;
					}
					if (!voxInterpreter.ShouldAddQuad(voxelData[1]))
					{
						MaskVal(m_sliceMaskPositive, sampleIndex[uAxis], sampleIndex[vAxis]) = thisVoxel;
						processBlockData = true;
					}
				}
			}
			if (processBlockData)	// If we added anything, run through the masks, merging + building quads
			{
				ProcessMaskAndBuildQuads(blockIndex, sampleIndex[sliceAxis] + 1, m_sliceMaskPositive, false, sliceAxis);	// front face
				ProcessMaskAndBuildQuads(blockIndex, sampleIndex[sliceAxis], m_sliceMaskNegative, true, sliceAxis);			// back face
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
					// calculate voxel area we are interested in
					glm::ivec3 blockIndex(bx, by, bz);
					glm::ivec3 voxelStartIndices, voxelEndIndices;
					m_targetModel.GetVoxelIterationParameters(blockIndex, modelSpaceBounds, voxelStartIndices, voxelEndIndices);

					// extract quads for each axis
					ExtractMeshesAlongAxis(blockIndex, voxelStartIndices, voxelEndIndices, 0);
					ExtractMeshesAlongAxis(blockIndex, voxelStartIndices, voxelEndIndices, 1);
					ExtractMeshesAlongAxis(blockIndex, voxelStartIndices, voxelEndIndices, 2);
				}
			}
		}
	}
}