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

	private:
		void ExtractQuadsFromBlock(const glm::ivec3& blockIndex, const glm::ivec3& startClump, const glm::ivec3& endClump);
		ModelType& m_targetModel;
	};
}

#include "quad_extractor.inl"