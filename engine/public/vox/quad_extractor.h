/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "math/box3.h"
#include <unordered_map>

namespace Vox
{
	// This class extracts quads from a voxel mesh for the purpose of triangulation
	// QuadUserData = anything extra required for a quad (material, custom normals, whatever)
	// Quads are stored in slices along the x,y,z axis
	template<class ModelType>
	class QuadExtractor
	{
	public:
		QuadExtractor(ModelType& targetModel);
		~QuadExtractor();

		void ExtractQuads(const Math::Box3& modelSpaceBounds);

		struct QuadDescriptor
		{
			glm::vec3 m_vertices[4];
		};

		typename std::vector<QuadDescriptor>::const_iterator BeginXAxis() const { return m_xAxisQuads.begin(); }
		typename std::vector<QuadDescriptor>::const_iterator EndXAxis() const { return m_xAxisQuads.end(); }
		typename std::vector<QuadDescriptor>::const_iterator BeginYAxis() const { return m_yAxisQuads.begin(); }
		typename std::vector<QuadDescriptor>::const_iterator EndYAxis() const { return m_yAxisQuads.end(); }
		typename std::vector<QuadDescriptor>::const_iterator BeginZAxis() const { return m_zAxisQuads.begin(); }
		typename std::vector<QuadDescriptor>::const_iterator EndZAxis() const { return m_zAxisQuads.end(); }

	private:

		void ExtractQuadsForVoxel(const typename ModelType::BlockType::ClumpType::VoxelDataType(&data)[3][3][3], 
								  const glm::vec3& voxelCenter);
		void ExtractQuadsFromBlock(const glm::ivec3& blockIndex, const glm::ivec3& startClump, const glm::ivec3& endClump);
		ModelType& m_targetModel;

		// separate quads per-axis. this way we can do greedy-meshing later with the results
		std::vector<QuadDescriptor> m_xAxisQuads;
		std::vector<QuadDescriptor> m_yAxisQuads;
		std::vector<QuadDescriptor> m_zAxisQuads;
	};
}

#include "quad_extractor.inl"