/*
SDLEngine
Matt Hoyle
*/

#include <functional>
#include "model_data_reader.h"

namespace Vox
{
	namespace Internal
	{
		template< class ModelType> class ModelVoxelRaymarcher;
		template< class ModelType> class ModelBlockRaymarcher;
	}

	template< class ModelType>
	class ModelRaymarcherParams
	{
		friend class Internal::ModelBlockRaymarcher<ModelType>;
		friend class Internal::ModelVoxelRaymarcher<ModelType>;
	public:
		ModelRaymarcherParams()		{ }
		~ModelRaymarcherParams()	{ }

		glm::vec3 VoxelPosition() const;
		typename const ModelType::VoxelDataType VoxelData() const;
		const glm::vec3& RayStart() const { return m_rayStart; }

	protected:
		ModelRaymarcherParams(ModelDataReader<ModelType>* reader, const glm::ivec3& block, const Math::Box3& blockBounds,
			const glm::vec3& voxSize, const glm::vec3& rS, const glm::vec3& rE)
			: m_reader(reader), m_blockIndex(block), m_blockBounds(blockBounds), m_voxelSize(voxSize), m_rayStart(rS), m_rayEnd(rE)
		{
		}

		ModelDataReader<ModelType>* m_reader;
		glm::ivec3 m_blockIndex;
		glm::ivec3 m_voxelIndex;
		Math::Box3 m_blockBounds;
		glm::vec3 m_voxelSize;
		glm::vec3 m_rayStart;
		glm::vec3 m_rayEnd;
	};

	template< class ModelType>
	class ModelRaymarcher
	{
	public:
		ModelRaymarcher(const ModelType& target);
		~ModelRaymarcher();

		typedef std::function<bool(const ModelRaymarcherParams<ModelType>&)> VoxelIterator;
		void Raymarch(const glm::vec3& rayStart, const glm::vec3& rayEnd, VoxelIterator iterator);

	private:
		const ModelType& m_target;
	};
}

#include "model_ray_marcher.inl"