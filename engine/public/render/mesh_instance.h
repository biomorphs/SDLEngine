/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <memory>
#include <glm.hpp>

namespace Render
{
	class Mesh;

	// This contains everything used to render a mesh
	class MeshInstance
	{
	public:
		MeshInstance() { }
		explicit MeshInstance(const std::shared_ptr<Mesh>& mesh, const glm::mat4& modelTransform)
			: m_mesh(mesh)
			, m_transform(modelTransform)
		{

		}
		~MeshInstance() { }

		// do not let the instance go out of scope while holding this pointer!
		inline Mesh* GetMesh() const					{ return m_mesh.get(); }	
		inline const glm::mat4& GetTransform() const	{ return m_transform;  }

	private:
		std::shared_ptr<Mesh> m_mesh;
		glm::mat4 m_transform;
	};
}