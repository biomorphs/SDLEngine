/*
SDLEngine
Matt Hoyle
*/
#include "debug_render.h"
#include "render/shader_binary.h"
#include "render/shader_program.h"
#include "render/material.h"
#include "render/render_buffer.h"
#include "render/mesh.h"
#include "render/render_pass.h"
#include "kernel/log.h"
#include "kernel/assert.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

namespace SDE
{
	static const char* c_vertexShader = "#version 330 core\r\n"
		"layout(location = 0) in vec4 pos_modelSpace;\r\n"
		"layout(location = 1) in vec4 colour;\r\n"
		"uniform mat4 MVP;\r\n"
		"out vec4 colourV;\r\n"
		"void main()\r\n"
		"{\r\n"
		"	vec4 v = vec4(pos_modelSpace.xyz, 1);\r\n"
		"	gl_Position = MVP * v;\r\n"
		"	colourV = colour;\r\n"
		"}\r\n";
	static const char* c_fragmentShader = "#version 330 core\r\n"
										  "in vec4 colourV;\r\n"
										  "out vec4 colour;\r\n"
										  "void main() {\r\n"
										  "	colour = colourV;\r\n"
										  "}";

	DebugRender::DebugRender()
		: m_currentLines(0)
		, m_currentWriteMesh(0)
	{
		auto deleter = [](glm::vec4* p)
		{
			_aligned_free(p);
		};

		void* rawBuffer = _aligned_malloc(c_maxLines * sizeof(glm::vec4) * 2, 16);
		m_posBuffer = std::unique_ptr<glm::vec4, decltype(deleter)>((glm::vec4*)rawBuffer, deleter);

		rawBuffer = _aligned_malloc(c_maxLines * sizeof(glm::vec4) * 2, 16);
		m_colBuffer = std::unique_ptr<glm::vec4, decltype(deleter)>((glm::vec4*)rawBuffer, deleter);
	}

	DebugRender::~DebugRender()
	{
		Destroy();
	}

	bool DebugRender::CreateMaterial()
	{
		Render::ShaderBinary vertexShader, fragmentShader;
		std::string compileResult;
		if (!vertexShader.CompileFromBuffer(Render::ShaderType::VertexShader, c_vertexShader, compileResult))
		{
			SDE_LOGC(SDE, "Failed to compile debug render vertex shader: %s", compileResult.c_str());
			return false;
		}
		if (!fragmentShader.CompileFromBuffer(Render::ShaderType::FragmentShader, c_fragmentShader, compileResult))
		{
			SDE_LOGC(SDE, "Failed to compile debug render fragment shader: %s", compileResult.c_str());
			return false;
		}
		m_shader = std::make_unique<Render::ShaderProgram>();
		if (!m_shader->Create(vertexShader, fragmentShader, compileResult))
		{
			SDE_LOGC(SDE, "Failed to create debug render shader program: %s", compileResult.c_str());
			return false;
		}
		m_shader->AddUniform("MVP");

		m_material = std::make_unique<Render::Material>();
		m_material->SetShaderProgram(m_shader.get());

		return true;
	}

	bool DebugRender::CreateMesh()
	{
		for (uint32_t i = 0; i < 2; ++i)
		{
			auto& mesh = m_renderMesh[i];
			mesh = std::make_unique<Render::Mesh>();
			auto& streams = mesh->GetStreams();
			auto& vertexArray = mesh->GetVertexArray();
			auto& chunks = mesh->GetChunks();
			
			// init vertex streams and vertex array
			streams.resize(2);
			Render::RenderBuffer &posBuffer = streams[0], &colourBuffer = streams[1];
			if (!posBuffer.Create(c_maxLines * sizeof(glm::vec4) * 2, Render::RenderBufferType::VertexData, Render::RenderBufferModification::Dynamic))
			{
				SDE_LOGC(SDE, "Failed to create debug pos buffer");
				return false;
			}
			if (!colourBuffer.Create(c_maxLines * sizeof(glm::vec4) * 2, Render::RenderBufferType::VertexData, Render::RenderBufferModification::Dynamic))
			{
				SDE_LOGC(SDE, "Failed to create debug colour buffer");
				return false;
			}
			vertexArray.AddBuffer(0, &streams[0], Render::VertexDataType::Float, 4);
			vertexArray.AddBuffer(1, &streams[1], Render::VertexDataType::Float, 4);
			if (!vertexArray.Create())
			{
				SDE_LOGC(SDE, "Failed to create debug vertex array");
				return false;
			}

			// add a chunk for each primitive type
			chunks.push_back(Render::MeshChunk(0, 0, Render::PrimitiveType::Lines));

			mesh->SetMaterial(m_material.get());
		}
		return true;
	}

	bool DebugRender::Create()
	{
		// Set up the shader and material first
		if (!CreateMaterial())
		{
			Destroy();
			return false;
		}

		if (!CreateMesh())
		{
			Destroy();
			return false;
		}

		return true;
	}

	void DebugRender::Destroy()
	{
		for (uint32_t i = 0; i < 2; ++i)
		{
			m_renderMesh[i] = nullptr;
		}
		m_material = nullptr;
		m_shader = nullptr;
	}

	void DebugRender::AddLinesInternal(const __m128* posBuffer, const __m128* colBuffer, uint32_t count)
	{
		uint32_t toAdd = count;
		if ((m_currentLines + count) > c_maxLines)
		{
			toAdd = c_maxLines - m_currentLines;
		}

		if (count > 0)
		{
			glm::vec4* posData = m_posBuffer.get() + (m_currentLines * 2);
			glm::vec4* colData = m_colBuffer.get() + (m_currentLines * 2);
			memcpy(posData, posBuffer, toAdd * sizeof(glm::vec4) * 2);
			memcpy(colData, colBuffer, toAdd * sizeof(glm::vec4) * 2);
			m_currentLines += toAdd;
		}
		SDE_ASSERT(m_currentLines < c_maxLines);
	}

	void DebugRender::AddLines(const glm::vec4* v, const glm::vec4* c, uint32_t count)
	{
		uint32_t toAdd = count;
		if ((m_currentLines + count) > c_maxLines)
		{
			toAdd = c_maxLines - m_currentLines;
		}

		if (count > 0)
		{
			glm::vec4* posData = m_posBuffer.get() + (m_currentLines * 2);
			glm::vec4* colData = m_colBuffer.get() + (m_currentLines * 2);
			memcpy(posData, v, toAdd * sizeof(glm::vec4) * 2);
			memcpy(colData, c, toAdd * sizeof(glm::vec4) * 2);
			m_currentLines += toAdd;
		}
		SDE_ASSERT(m_currentLines < c_maxLines);
	}

	void DebugRender::AddBox(const glm::vec3& boxCenter, const glm::vec3& boxSize, const glm::vec4& colour)
	{
		const glm::vec3 halfSize = boxSize * 0.5f;
		const glm::vec3 bl = boxCenter - halfSize;
		const glm::vec3 tr = boxCenter + halfSize;

		/*AddLine(glm::vec4(bl.x, bl.y, bl.z, 0.0f), glm::vec4(tr.x, bl.y, bl.z, 0.0f), colour, colour);
		AddLine(glm::vec4(tr.x, bl.y, bl.z, 0.0f), glm::vec4(tr.x, tr.y, bl.z, 0.0f), colour, colour);
		AddLine(glm::vec4(tr.x, tr.y, bl.z, 0.0f), glm::vec4(bl.x, tr.y, bl.z, 0.0f), colour, colour);
		AddLine(glm::vec4(bl.x, tr.y, bl.z, 0.0f), glm::vec4(bl.x, bl.y, bl.z, 0.0f), colour, colour);
		AddLine(glm::vec4(bl.x, bl.y, tr.z, 0.0f), glm::vec4(tr.x, bl.y, tr.z, 0.0f), colour, colour);
		AddLine(glm::vec4(tr.x, bl.y, tr.z, 0.0f), glm::vec4(tr.x, tr.y, tr.z, 0.0f), colour, colour);
		AddLine(glm::vec4(tr.x, tr.y, tr.z, 0.0f), glm::vec4(bl.x, tr.y, tr.z, 0.0f), colour, colour);
		AddLine(glm::vec4(bl.x, tr.y, tr.z, 0.0f), glm::vec4(bl.x, bl.y, tr.z, 0.0f), colour, colour);
		AddLine(glm::vec4(bl.x, bl.y, bl.z, 0.0f), glm::vec4(bl.x, bl.y, tr.z, 0.0f), colour, colour);
		AddLine(glm::vec4(tr.x, bl.y, bl.z, 0.0f), glm::vec4(tr.x, bl.y, tr.z, 0.0f), colour, colour);
		AddLine(glm::vec4(bl.x, tr.y, bl.z, 0.0f), glm::vec4(bl.x, tr.y, tr.z, 0.0f), colour, colour);
		AddLine(glm::vec4(tr.x, tr.y, bl.z, 0.0f), glm::vec4(tr.x, tr.y, tr.z, 0.0f), colour, colour);*/
	}

	void DebugRender::AddAxisAtPoint(const glm::vec4& point, float scale)
	{
		const __m128 c_xAxis = { scale, 0.0f, 0.0f, 0.0f };
		const __m128 c_yAxis = { 0.0f, scale, 0.0f, 0.0f };
		const __m128 c_zAxis = { 0.0f, 0.0f, scale, 0.0f };
		__declspec(align(16)) glm::vec4 pointAligned = point;
		const __m128 c_point = _mm_load_ps(glm::value_ptr(pointAligned));
		const __m128 c_xColour = { 1.0f, 0.0f, 0.0f, 1.0f };
		const __m128 c_yColour = { 0.0f, 1.0f, 0.0f, 1.0f };
		const __m128 c_zColour = { 0.0f, 0.0f, 1.0f, 1.0f };
		__declspec(align(16)) const __m128 c_colours[] = {
			c_xColour, c_xColour,
			c_yColour, c_yColour,
			c_zColour, c_zColour
		};

		__declspec(align(16)) __m128 positions[12];
		positions[0] = c_point;	positions[1] = _mm_add_ps(c_point, c_xAxis);
		positions[2] = c_point;	positions[3] = _mm_add_ps(c_point, c_yAxis);
		positions[4] = c_point;	positions[5] = _mm_add_ps(c_point, c_zAxis);

		AddLinesInternal(positions, c_colours, 6);
	}

	void DebugRender::PushLinesToMesh(Render::Mesh& target)
	{
		auto& theChunk = target.GetChunks()[0];
		auto& posStream = target.GetStreams()[0];
		auto& colourStream = target.GetStreams()[1];

		// push data to gpu
		posStream.SetData(0, m_currentLines * sizeof(glm::vec4) * 2, m_posBuffer.get());
		colourStream.SetData(0, m_currentLines * sizeof(glm::vec4) * 2, m_colBuffer.get());

		// update chunk
		theChunk.m_firstVertex = 0;
		theChunk.m_vertexCount = m_currentLines * 2;

		// remove old lines
		m_currentLines = 0;
	}

	void DebugRender::PushToRenderPass(Render::Camera& camera, Render::RenderPass& targetPass)
	{
		PushLinesToMesh(*m_renderMesh[m_currentWriteMesh]);
		
		// render newly written mesh
		auto currentRenderMesh = m_currentWriteMesh;
		const glm::mat4 mvp = camera.ProjectionMatrix() * camera.ViewMatrix();
		Render::UniformBuffer instanceUniforms;
		instanceUniforms.SetValue("MVP", mvp);
		targetPass.AddInstance(m_renderMesh[currentRenderMesh].get(), std::move(instanceUniforms));
		
		// flip buffers
		m_currentWriteMesh = (m_currentWriteMesh + 1) & 1;
	}
}


