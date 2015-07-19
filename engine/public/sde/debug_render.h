/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include <glm.hpp>
#include <memory>
#include <vector>

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

		bool Create(uint32_t maxVertices = 16384);
		void Destroy();
		void PushToRenderPass(Render::Camera& camera, Render::RenderPass& targetPass);

		void AddLine(const glm::vec3& v0, const glm::vec3& v1);
		void AddLine(const glm::vec3& v0, const glm::vec3& v1, const glm::vec4& c0, const glm::vec4& c1);
		void AddAxisAtPoint(glm::vec3 point, float scale = 1.0f);
		void AddBox(const glm::vec3& boxCenter, const glm::vec3& boxSize, const glm::vec4& colour);

	private:
		bool CreateMaterial();
		bool CreateMesh(uint32_t maxVertices);
		void PushLinesToMesh(Render::Mesh& target);

		struct LineDefinition
		{
			glm::vec3 m_points[2];
			glm::vec4 m_colours[2];
		};

		std::unique_ptr<Render::ShaderProgram> m_shader;
		std::unique_ptr<Render::Material> m_material;
		std::unique_ptr<Render::Mesh> m_renderMesh[2];		// double-buffered
		uint32_t m_currentWriteMesh;						// mesh to write to this frame
		std::vector<float> m_tempWriteBuffers[2];			// used to write vertex data before pushing to gpu. one per stream
		std::vector<LineDefinition> m_linesToDraw;
	};
}