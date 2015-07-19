/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include <glm.hpp>
#include <vector>
#include <memory>

namespace Render
{
	class Texture;
	class ShaderProgram;
	class Material;
	class Mesh;
	class RenderPass;
	class Camera;
}

struct ImDrawData;

namespace DebugGui
{
	class DebugGuiRender
	{
	public:
		DebugGuiRender() { }
		~DebugGuiRender();
		bool Create(float viewportWidth, float viewportHeight);
		void Destroy();
		void RebuildMesh();
		void SubmitToPass(Render::RenderPass& pass);

	private:
		static void ImgGui_RenderDrawLists(ImDrawData* draw_data);
		bool CreateFontTexture();
		bool CreateMaterial();
		bool CreateMesh();
		void UploadVertexData(std::vector<float> pos, std::vector<float> uvs, std::vector<float> colour);
		void AddMeshChunk(uint32_t startVert, uint32_t vertCount, uint32_t textureId, const glm::vec4& clipRect);

		struct ChunkInstanceProperties
		{
			uint32_t m_textureHandle;
			glm::vec4 m_clipRect;
		};

		float m_viewportWidth;
		float m_viewportHeight;
		std::unique_ptr<Render::Texture> m_fontTexture;
		std::unique_ptr<Render::ShaderProgram> m_shader;
		std::unique_ptr<Render::Material> m_material;
		std::unique_ptr<Render::Mesh> m_mesh;
		std::vector<ChunkInstanceProperties> m_chunkInstanceProps;

		static const uint32_t c_maxVertices = 16 * 1024;
	};
}