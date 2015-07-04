/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "math/box3.h"
#include <vector>

namespace Vox
{
	// This class implements greedy-meshing of quads
	// It works by splitting a mesh into slices across each axis, 
	// building a mask of quads for each slice, then merging the quads
	template<class ModelType>
	class GreedyQuadExtractor
	{
	public:
		GreedyQuadExtractor(ModelType& targetModel);
		~GreedyQuadExtractor();

		void ExtractQuads(const Math::Box3& modelSpaceBounds);

		struct QuadDescriptor
		{
			glm::vec3 m_vertices[4];
		};
		typename std::vector<QuadDescriptor>::const_iterator Begin() const { return m_quads.begin(); }
		typename std::vector<QuadDescriptor>::const_iterator End() const { return m_quads.end(); }

	private:
		typedef uint8_t MaskType;

		
		void InitialiseSliceMasks();
		void ResetSliceMasks();
		void ClearSliceMask(std::vector<MaskType>&mask, int32_t u, int32_t v, int32_t uMax, int32_t vMax);
		MaskType& MaskVal(std::vector<MaskType>&mask, int32_t u, int32_t v);
		const MaskType& MaskVal(const std::vector<MaskType>&mask, int32_t u, int32_t v) const;

		void ExtractMeshesAlongZ(const glm::ivec3& blockIndex, const glm::ivec3& startClump, const glm::ivec3& endClump);

		void ProcessMaskAndBuildQuads(const glm::ivec3& blockIndex, int32_t slice, std::vector<MaskType>&mask);
		void CalculateMergedQuadsFromMask(const std::vector<MaskType>& mask, int32_t u, int32_t v, int32_t& quadEndU, int32_t& quadEndV);

		std::vector<QuadDescriptor> m_quads;
		ModelType& m_targetModel;
		std::vector<MaskType> m_sliceMaskPositive;	// temporary storage for slice masks. thrown away on completion
		std::vector<MaskType> m_sliceMaskNegative;	// temporary storage for slice masks. thrown away on completion
	};
}

#include "greedy_quad_extractor.inl"