/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <glm/glm.hpp>

namespace Vox
{
	template<class ModelType>
	class ModelDataWriter
	{
	public:
		ModelDataWriter(ModelType& model);
		~ModelDataWriter();

		typedef typename ModelType::VoxelDataType VoxelDataType;

		void WriteVoxel(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex, const VoxelDataType& data);
		inline typename ModelType::VoxelDataType& VoxelAt(const glm::ivec3& blockIndex, const glm::ivec3& voxelIndex);

	private:
		typename ModelType::BlockType* GetBlockAndCache(const glm::ivec3& blockIndex);

		ModelType& m_target;
		glm::ivec3 m_cachedBlockIndex;
		typename ModelType::BlockType* m_cachedBlock;
	};
}

#include "model_data_writer.inl"