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
#include <gtc/type_ptr.hpp>

namespace SDE
{
	static const char* c_vertexShader = "#version 330 core\r\n"
		"layout(location = 0) in vec3 pos_modelSpace;\r\n"
		"layout(location = 1) in vec4 colour;\r\n"
		"uniform mat4 MVP;\r\n"
		"out vec4 colourV;\r\n"
		"void main()\r\n"
		"{\r\n"
		"	vec4 v = vec4(pos_modelSpace, 1);\r\n"
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
	{
		m_linesToDraw.reserve(1024);
		m_tempWriteBuffers[0].reserve(4 * 1024 * sizeof(float) * 3);
		m_tempWriteBuffers[1].reserve(4 * 1024 * sizeof(float) * 4);
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

	bool DebugRender::CreateMesh(uint32_t maxVertices)
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
			if (!posBuffer.Create(maxVertices * 3 * sizeof(float), Render::RenderBufferType::VertexData, Render::RenderBufferModification::Dynamic))
			{
				SDE_LOGC(SDE, "Failed to create debug pos buffer");
				return false;
			}
			if (!colourBuffer.Create(maxVertices * 4 * sizeof(float), Render::RenderBufferType::VertexData, Render::RenderBufferModification::Dynamic))
			{
				SDE_LOGC(SDE, "Failed to create debug colour buffer");
				return false;
			}
			vertexArray.AddBuffer(0, &streams[0], Render::VertexDataType::Float, 3);
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

	bool DebugRender::Create(uint32_t maxVertices)
	{
		// Set up the shader and material first
		if (!CreateMaterial())
		{
			Destroy();
			return false;
		}

		if (!CreateMesh(maxVertices))
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

	void DebugRender::AddLine(const glm::vec3& v0, const glm::vec3& v1)
	{
		const glm::vec4 c_colour(1.0f);
		LineDefinition lineDef;
		lineDef.m_points[0] = v0;
		lineDef.m_points[1] = v1;
		lineDef.m_colours[0] = c_colour;
		lineDef.m_colours[1] = c_colour;
		m_linesToDraw.push_back(lineDef);
	}

	void DebugRender::AddBox(const glm::vec3& boxCenter, const glm::vec3& boxSize, const glm::vec4& colour)
	{
		const glm::vec3 halfSize = boxSize * 0.5f;
		const glm::vec3 bl = boxCenter - halfSize;
		const glm::vec3 tr = boxCenter + halfSize;

		AddLine(glm::vec3(bl.x, bl.y, bl.z), glm::vec3(tr.x, bl.y, bl.z), colour, colour);
		AddLine(glm::vec3(tr.x, bl.y, bl.z), glm::vec3(tr.x, tr.y, bl.z), colour, colour);
		AddLine(glm::vec3(tr.x, tr.y, bl.z), glm::vec3(bl.x, tr.y, bl.z), colour, colour);
		AddLine(glm::vec3(bl.x, tr.y, bl.z), glm::vec3(bl.x, bl.y, bl.z), colour, colour);
		AddLine(glm::vec3(bl.x, bl.y, tr.z), glm::vec3(tr.x, bl.y, tr.z), colour, colour);
		AddLine(glm::vec3(tr.x, bl.y, tr.z), glm::vec3(tr.x, tr.y, tr.z), colour, colour);
		AddLine(glm::vec3(tr.x, tr.y, tr.z), glm::vec3(bl.x, tr.y, tr.z), colour, colour);
		AddLine(glm::vec3(bl.x, tr.y, tr.z), glm::vec3(bl.x, bl.y, tr.z), colour, colour);
		AddLine(glm::vec3(bl.x, bl.y, bl.z), glm::vec3(bl.x, bl.y, tr.z), colour, colour);
		AddLine(glm::vec3(tr.x, bl.y, bl.z), glm::vec3(tr.x, bl.y, tr.z), colour, colour);
		AddLine(glm::vec3(bl.x, tr.y, bl.z), glm::vec3(bl.x, tr.y, tr.z), colour, colour);
		AddLine(glm::vec3(tr.x, tr.y, bl.z), glm::vec3(tr.x, tr.y, tr.z), colour, colour);
	}

	void DebugRender::AddLine(const glm::vec3& v0, const glm::vec3& v1, const glm::vec4& c0, const glm::vec4& c1)
	{
		LineDefinition lineDef;
		lineDef.m_points[0] = v0;
		lineDef.m_points[1] = v1;
		lineDef.m_colours[0] = c0;
		lineDef.m_colours[1] = c1;
		m_linesToDraw.push_back(lineDef);
	}

	void DebugRender::AddAxisAtPoint(glm::vec3 point, float scale)
	{
		const glm::vec3 xAxis(scale, 0.0f, 0.0f);
		const glm::vec3 yAxis(0.0f, scale, 0.0f);
		const glm::vec3 zAxis(0.0f, 0.0f, scale);
		const glm::vec4 xColour(1.0f, 0.0f, 0.0f, 1.0f);
		const glm::vec4 yColour(0.0f, 1.0f, 0.0f, 1.0f);
		const glm::vec4 zColour(0.0f, 0.0f, 1.0f, 1.0f);
		AddLine(point, point + xAxis, xColour, xColour);
		AddLine(point, point + yAxis, yColour, yColour);
		AddLine(point, point + zAxis, zColour, zColour);
	}

	void DebugRender::PushLinesToMesh(Render::Mesh& target)
	{
		auto& theChunk = target.GetChunks()[0];
		auto& posStream = target.GetStreams()[0];
		auto& colourStream = target.GetStreams()[1];

		// reset write buffers
		m_tempWriteBuffers[0].clear();
		m_tempWriteBuffers[0].reserve(m_linesToDraw.size() * 2 * sizeof(float) * 3);
		m_tempWriteBuffers[1].clear();
		m_tempWriteBuffers[1].reserve(m_linesToDraw.size() * 2 * sizeof(float) * 4);

		for (auto& line : m_linesToDraw)
		{
			// position
			m_tempWriteBuffers[0].insert(m_tempWriteBuffers[0].end(), glm::value_ptr(line.m_points[0]), glm::value_ptr(line.m_points[0]) + 3);
			m_tempWriteBuffers[0].insert(m_tempWriteBuffers[0].end(), glm::value_ptr(line.m_points[1]), glm::value_ptr(line.m_points[1]) + 3);
			//colour
			m_tempWriteBuffers[1].insert(m_tempWriteBuffers[1].end(), glm::value_ptr(line.m_colours[0]), glm::value_ptr(line.m_colours[0]) + 4);
			m_tempWriteBuffers[1].insert(m_tempWriteBuffers[1].end(), glm::value_ptr(line.m_colours[1]), glm::value_ptr(line.m_colours[1]) + 4);
		}

		// push data to gpu
		posStream.SetData(0, m_tempWriteBuffers[0].size() * sizeof(float), m_tempWriteBuffers[0].data());
		colourStream.SetData(0, m_tempWriteBuffers[1].size() * sizeof(float), m_tempWriteBuffers[1].data());

		// update chunk
		theChunk.m_firstVertex = 0;
		theChunk.m_vertexCount = (uint32_t)m_linesToDraw.size() * 2;

		// remove old lines
		m_linesToDraw.clear();
	}

	void DebugRender::PushToRenderPass(Render::Camera& camera, Render::RenderPass& targetPass)
	{
		PushLinesToMesh(*m_renderMesh[m_currentWriteMesh]);
		
		// render newly written mesh
		const glm::mat4 mvp = camera.ProjectionMatrix() * camera.ViewMatrix();
		Render::UniformBuffer instanceUniforms;
		instanceUniforms.SetValue("MVP", mvp);
		targetPass.AddInstance(m_renderMesh[m_currentWriteMesh].get(), std::move(instanceUniforms));
		
		// flip buffers
		m_currentWriteMesh = (m_currentWriteMesh + 1) & 1;
	}
}


