/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <functional>

namespace Render
{
	class ShaderProgram;
	class Mesh;
	class Material;
	class RenderPass;
	class Camera;
}

namespace SDE
{
	// Provides an interface for drawing debug stuff, lines, quads, etc
	class DebugRender
	{
	public:
		DebugRender();
		~DebugRender();

		bool Create();
		void Destroy();
		void PushToRenderPass(Render::Camera& camera, Render::RenderPass& targetPass);
		void AddLines(const glm::vec4* v, const glm::vec4* c, uint32_t count);
		void AddAxisAtPoint(const glm::vec4& point, float scale = 1.0f);
		void AddBox(const glm::vec3& boxCenter, const glm::vec3& boxSize, const glm::vec4& colour);

	private:
		bool CreateMaterial();
		bool CreateMesh();
		void PushLinesToMesh(Render::Mesh& target);

		std::unique_ptr<glm::vec4, std::function<void(glm::vec4*)>> m_posBuffer;
		std::unique_ptr<glm::vec4, std::function<void(glm::vec4*)>> m_colBuffer;

		static const uint32_t c_maxLines = 1024 * 1024 * 1;
		uint32_t m_currentLines;
		std::unique_ptr<Render::ShaderProgram> m_shader;
		std::unique_ptr<Render::Material> m_material;
		std::unique_ptr<Render::Mesh> m_renderMesh[2];		// double-buffered
		uint32_t m_currentWriteMesh;						// mesh to write to this frame
	};
}