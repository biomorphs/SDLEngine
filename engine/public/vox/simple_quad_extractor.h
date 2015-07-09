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
	// It is naive, and pretty much just serves as a reference implementation of model data iteration
	template<class ModelType>
	class SimpleQuadExtractor
	{
	public:
		SimpleQuadExtractor(const ModelType& targetModel);
		~SimpleQuadExtractor();

		void ExtractQuads(const Math::Box3& modelSpaceBounds);

		struct QuadDescriptor
		{
			glm::vec3 m_vertices[4];
		};

		typename std::vector<QuadDescriptor>::const_iterator Begin() const { return m_quads.begin(); }
		typename std::vector<QuadDescriptor>::const_iterator End() const { return m_quads.end(); }

	private:

		void ExtractQuadsForVoxel(const typename ModelType::VoxelDataType(&data)[3][3][3], 
								  const glm::vec3& voxelCenter);
		void ExtractQuadsFromBlock(const glm::ivec3& blockIndex, const glm::ivec3& startVoxel, const glm::ivec3& endVoxel);
		const ModelType& m_targetModel;

		// vector of resulting quads
		std::vector<QuadDescriptor> m_quads;
	};
}

#include "simple_quad_extractor.inl"