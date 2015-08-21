/*
SDLEngine
Matt Hoyle
*/
#include "debug_gui_render.h"
#include "render/mesh.h"
#include "render/shader_binary.h"
#include "render/shader_program.h"
#include "render/material.h"
#include "render/texture_source.h"
#include "render/texture.h"
#include "render/mesh_builder.h"
#include "render/render_pass.h"
#include <imggui\imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DebugGui
{
	static const char* c_vertexShader = "#version 330 core\r\n"
		"uniform mat4 ProjMtx;\n"
		"layout(location = 0) in vec2 Position;\r\n"
		"layout(location = 1) in vec2 UV;\r\n"
		"layout(location = 2) in vec4 Color;\r\n"
		"out vec2 Frag_UV;\r\n"
		"out vec4 Frag_Color;\r\n"
		"void main()\r\n"
		"{\r\n"
		"	Frag_UV = UV;\r\n"
		"	Frag_Color = Color;\r\n"
		"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\r\n"
		"}\r\n";

	static const char* c_fragmentShader = "#version 330 core\r\n"
		"uniform sampler2D Texture;\r\n"
		"in vec2 Frag_UV;\r\n"
		"in vec4 Frag_Color;\r\n"
		"out vec4 Out_Color;\r\n"
		"void main()\r\n"
		"{\r\n"
		"	Out_Color = Frag_Color * texture( Texture, Frag_UV );\r\n"
		"}\r\n";

	glm::vec4 ColourToVec4(uint32_t c)
	{
		uint32_t r = (c & 0xff);
		uint32_t g = (c & 0xff00) >> 8;
		uint32_t b = (c & 0xff0000) >> 16;
		uint32_t a = (c & 0xff000000) >> 24;
		return glm::vec4((float)r, (float)g, (float)b, (float)a) / 255.0f;
	}

	DebugGuiRender* g_thisInstance = nullptr;	// Horrible, but not much choice
	void DebugGuiRender::ImgGui_RenderDrawLists(ImDrawData* draw_data)
	{
		// We need some temporary buffers to store the raw vertex data before upload to the mesh
		std::vector<float> tempDataBuffers[3];

		// There are multiple command lists to process
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const auto& thisCommandList = draw_data->CmdLists[n];
			const auto& vertexBuffer = thisCommandList->VtxBuffer;
			const auto& indexBuffer = thisCommandList->IdxBuffer;

			// Add all vertices for this command to the temp buffer
			uint32_t startVertexThisCmd = (uint32_t)tempDataBuffers[0].size() / 2;
			for (int thisIndex = 0;thisIndex < indexBuffer.size(); ++thisIndex)
			{
				const auto& theVert = vertexBuffer[indexBuffer[thisIndex]];
				tempDataBuffers[0].push_back(theVert.pos.x);	tempDataBuffers[0].push_back(theVert.pos.y);
				tempDataBuffers[1].push_back(theVert.uv.x);		tempDataBuffers[1].push_back(theVert.uv.y);
				glm::vec4 colour = ColourToVec4(theVert.col);
				tempDataBuffers[2].insert(tempDataBuffers[2].end(), glm::value_ptr(colour), glm::value_ptr(colour) + 4);
			}

			// For each command in this list, we make a chunk in the mesh, and cache any data we need to render later
			uint32_t chunkStartVertex = startVertexThisCmd;
			for (int cmd_i = 0; cmd_i < thisCommandList->CmdBuffer.size(); cmd_i++)
			{
				const ImDrawCmd* pcmd = &thisCommandList->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					pcmd->UserCallback(thisCommandList, pcmd);
				}
				else
				{
					uint32_t vertexCount = pcmd->ElemCount;
					uint32_t textureID = (uint32_t)(uintptr_t)pcmd->TextureId;
					glm::vec4 clipRect(pcmd->ClipRect.x, pcmd->ClipRect.y, pcmd->ClipRect.w, pcmd->ClipRect.z);
					g_thisInstance->AddMeshChunk(chunkStartVertex, vertexCount, textureID, clipRect);
					chunkStartVertex += vertexCount;
				}
			}
		}
		g_thisInstance->UploadVertexData(tempDataBuffers[0], tempDataBuffers[1], tempDataBuffers[2]);
	}

	void DebugGuiRender::AddMeshChunk(uint32_t startVert, uint32_t vertCount, uint32_t textureId, const glm::vec4& clipRect)
	{
		SDE_ASSERT(vertCount % 3 == 0);
		Render::MeshChunk newChunk;
		newChunk.m_firstVertex = startVert;
		newChunk.m_vertexCount = vertCount;
		newChunk.m_primitiveType = Render::PrimitiveType::Triangles;
		m_mesh->GetChunks().push_back(newChunk);
		
		ChunkInstanceProperties newProps;
		newProps.m_textureHandle = textureId;
		newProps.m_clipRect = clipRect;
		m_chunkInstanceProps.push_back(newProps);
	}

	void DebugGuiRender::UploadVertexData(std::vector<float> pos, std::vector<float> uvs, std::vector<float> colour)
	{
		auto& posStream = m_mesh->GetStreams()[0];
		auto& uvStream = m_mesh->GetStreams()[1];
		auto& colourStream = m_mesh->GetStreams()[2];
		SDE_ASSERT(posStream.GetSize() >= (pos.size() * sizeof(float)));
		SDE_ASSERT(uvStream.GetSize() >= (uvs.size() * sizeof(float)));
		SDE_ASSERT(colourStream.GetSize() >= (colour.size() * sizeof(float)));

		posStream.SetData(0, pos.size() * sizeof(float), pos.data());
		uvStream.SetData(0, uvs.size() * sizeof(float), uvs.data());
		colourStream.SetData(0, colour.size() * sizeof(float), colour.data());
	}

	void DebugGuiRender::SubmitToPass(Render::RenderPass& pass)
	{
		SDE_ASSERT(m_mesh->GetChunks().size() == m_chunkInstanceProps.size());

		glm::mat4 orthoProjection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);

		// Submit each mesh chunk as a separate instance (so they can have different textures/cliprect)
		for (uint32_t meshChunk = 0; meshChunk < m_mesh->GetChunks().size(); ++meshChunk)
		{
			Render::UniformBuffer instanceUniforms;
			instanceUniforms.SetValue("ProjMtx", orthoProjection);
			instanceUniforms.SetSampler("Texture", m_chunkInstanceProps[meshChunk].m_textureHandle);
			pass.AddInstance(m_mesh.get(), std::move(instanceUniforms), meshChunk, meshChunk + 1);
		}
	}

	DebugGuiRender::~DebugGuiRender()
	{
		Destroy();
	}

	uint32_t fontTexture;
	void DebugGuiRender::RebuildMesh()
	{
		g_thisInstance = this;
		m_mesh->GetChunks().clear();	// Remove all old chunks
		m_chunkInstanceProps.clear();
		ImGui::Render();				// Calls internal callback to rebuild mesh buffers
	}

	bool DebugGuiRender::Create(float viewportWidth, float viewportHeight)
	{
		m_viewportWidth = viewportWidth;
		m_viewportHeight = viewportHeight;

		if (!CreateFontTexture())
		{
			SDE_LOGC(DebugGui, "Failed to create font texture");
			return false;
		}

		if (!CreateMaterial())
		{
			SDE_LOGC(DebugGui, "Failed to create shader");
			return false;
		}

		if (!CreateMesh())
		{
			SDE_LOGC(DebugGui, "Failed to create mesh");
			return false;
		}

		// Register rendering callback
		ImGuiIO& io = ImGui::GetIO();
		io.RenderDrawListsFn = DebugGuiRender::ImgGui_RenderDrawLists;

		// Set up display size (only once - we don't support window resizing)
		io.DisplaySize = ImVec2(viewportWidth, viewportHeight);

		return true;
	}

	bool DebugGuiRender::CreateMaterial()
	{
		Render::ShaderBinary vertexShader, fragmentShader;
		std::string compileResult;
		if (!vertexShader.CompileFromBuffer(Render::ShaderType::VertexShader, c_vertexShader, compileResult))
		{
			SDE_LOGC(SDE, "Failed to compile gui vertex shader: %s", compileResult.c_str());
			return false;
		}
		if (!fragmentShader.CompileFromBuffer(Render::ShaderType::FragmentShader, c_fragmentShader, compileResult))
		{
			SDE_LOGC(SDE, "Failed to compile gui fragment shader: %s", compileResult.c_str());
			return false;
		}
		m_shader = std::make_unique<Render::ShaderProgram>();
		if (!m_shader->Create(vertexShader, fragmentShader, compileResult))
		{
			SDE_LOGC(SDE, "Failed to create gui shader program: %s", compileResult.c_str());
			return false;
		}
		m_shader->AddUniform("Texture");
		m_shader->AddUniform("ProjMtx");

		m_material = std::make_unique<Render::Material>();
		m_material->SetShaderProgram(m_shader.get());

		return true;
	}

	bool DebugGuiRender::CreateFontTexture()
	{
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
		size_t dataSize = width * height * sizeof(uint8_t) * 4;
		if (dataSize == 0)
		{
			return false;
		}

		// Create a TextureSource object containing the data we need
		std::vector<Render::TextureSource::MipDesc> mipData;
		Render::TextureSource::MipDesc oneMip;
		oneMip.m_width = width;
		oneMip.m_height = height;
		oneMip.m_offset = 0;
		oneMip.m_size = dataSize;
		mipData.push_back(oneMip);

		// Copy the data to vector, sucks but thats the interface
		std::vector<uint8_t> dataCopy(dataSize);
		memcpy(dataCopy.data(), pixels, dataSize);

		Render::TextureSource texSrc(width, height, Render::TextureSource::Format::RGBA8, std::move(mipData), std::move(dataCopy));
		std::vector<Render::TextureSource> textureSources;
		textureSources.push_back(texSrc);

		m_fontTexture = std::make_unique < Render::Texture >();
		if (!m_fontTexture->Create(textureSources))
		{
			return false;
		}
		io.Fonts->TexID = (void*)(intptr_t)m_fontTexture->GetHandle();

		// Cleanup (don't clear the input data if you want to append new fonts later)
		io.Fonts->ClearInputData();
		io.Fonts->ClearTexData();

		return true;
	}

	bool DebugGuiRender::CreateMesh()
	{
		m_mesh = std::make_unique<Render::Mesh>();
		auto& streams = m_mesh->GetStreams();
		auto& vertexArray = m_mesh->GetVertexArray();
		auto& chunks = m_mesh->GetChunks();

		// init vertex streams and vertex array
		streams.resize(3);
		Render::RenderBuffer &posBuffer = streams[0], &uvBuffer = streams[1], &colourBuffer = streams[2];
		if (!posBuffer.Create(c_maxVertices * 2 * sizeof(float), Render::RenderBufferType::VertexData, Render::RenderBufferModification::Dynamic))
		{
			return false;
		}
		if (!uvBuffer.Create(c_maxVertices * 2 * sizeof(float), Render::RenderBufferType::VertexData, Render::RenderBufferModification::Dynamic))
		{
			return false;
		}
		if (!colourBuffer.Create(c_maxVertices * 4 * sizeof(float), Render::RenderBufferType::VertexData, Render::RenderBufferModification::Dynamic))
		{
			return false;
		}
		vertexArray.AddBuffer(0, &streams[0], Render::VertexDataType::Float, 2);
		vertexArray.AddBuffer(1, &streams[1], Render::VertexDataType::Float, 2);
		vertexArray.AddBuffer(2, &streams[2], Render::VertexDataType::Float, 4);
		if (!vertexArray.Create())
		{
			return false;
		}

		m_mesh->SetMaterial(m_material.get());
		return true;
	}

	void DebugGuiRender::Destroy()
	{
		m_mesh = nullptr;
		m_fontTexture = nullptr;
		m_material = nullptr;
		m_shader = nullptr;
	}
}