/*
SDLEngine
Matt Hoyle
*/

#include <functional>

namespace Vox
{
	template< class ModelType>
	class ModelRaymarcher
	{
	public:
		ModelRaymarcher(ModelType& target);
		~ModelRaymarcher();

		typedef std::function<bool(typename ModelType::ClumpDataAccessor&, const glm::vec3&, const glm::vec3&, const glm::vec3&)> ClumpIterator;
		void Raymarch(const glm::vec3& rayStart, const glm::vec3& rayEnd, ClumpIterator iterator);

	private:
		ModelType& m_target;
	};
}

#include "model_ray_marcher.inl"