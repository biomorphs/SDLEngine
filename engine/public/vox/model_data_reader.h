/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <glm.hpp>

namespace Vox
{
	template<class ModelType>
	class ModelDataReader
	{
	public:
		ModelDataReader(const ModelType& model);
		~ModelDataReader();

		typedef typename ModelType::VoxelDataType VoxelDataType;

		bool HasBlockData(const glm::ivec3& blockIndex) const;
		VoxelDataType VoxelAt(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex);
		VoxelDataType VoxelNeighbour(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex, const glm::ivec3& offset);
		void MooreNeighbours(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex, VoxelDataType(&data)[3][3][3]);

	private:
		const typename ModelType::BlockType* GetBlockAndCache(const glm::ivec3& blockIndex);
		void FixupNeighbourIndex(int32_t voxelIndex, int32_t blockIndex, int32_t& blockIndexOut, int32_t& voxelIndexOut) const;

		const ModelType& m_target;
		glm::ivec3 m_cachedBlockIndex;
		const typename ModelType::BlockType* m_cachedBlock;
	};
}

#include "model_data_reader.inl"