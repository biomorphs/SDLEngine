/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "model_data_writer.h"
#include "math/box3.h"
#include <functional>

namespace Vox
{
	template<class ModelType> class ModelAreaDataWriter;

	template<class ModelType>
	class ModelAreaDataWriterParams
	{
		friend class ModelAreaDataWriter<ModelType>;
	public:
		ModelAreaDataWriterParams() { }
		~ModelAreaDataWriterParams() { }

		inline const glm::ivec3& StartVoxel() const			{ return m_startVoxelIndex; }
		inline const glm::ivec3& EndVoxel() const			{ return m_endVoxelIndex; }

		inline glm::vec3 VoxelPosition(int32_t vx, int32_t vy, int32_t vz) const;
		inline void WriteVoxel(int32_t vx, int32_t vy, int32_t vz, typename const ModelType::VoxelDataType& data);
		inline typename ModelType::VoxelDataType& VoxelAt(int32_t vx, int32_t vy, int32_t vz);

	protected:
		ModelAreaDataWriterParams(ModelDataWriter<ModelType>* writer, const glm::ivec3& block, const Math::Box3& blockBounds,
			const glm::vec3& voxSize, const glm::ivec3& startVoxel, const glm::ivec3& endVoxel)
			: m_writer(writer), m_blockIndex(block), m_blockBounds(blockBounds), m_voxelSize(voxSize)
			, m_startVoxelIndex(startVoxel), m_endVoxelIndex(endVoxel)
		{
		}
		ModelDataWriter<ModelType>* m_writer;
		glm::ivec3 m_blockIndex;
		Math::Box3 m_blockBounds;
		glm::vec3 m_voxelSize;
		glm::ivec3 m_startVoxelIndex;
		glm::ivec3 m_endVoxelIndex;
	};

	template<class ModelType>
	class ModelAreaDataWriter
	{
	public:
		ModelAreaDataWriter(ModelType& model);
		~ModelAreaDataWriter();

		typedef std::function<void(ModelAreaDataWriterParams<ModelType>&)> AreaCallback;
		void WriteArea(const Math::Box3& modelSpaceBounds, AreaCallback callback);

	private:
		ModelType& m_target;
	};
}

#include "model_area_data_writer.inl"