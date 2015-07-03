/*
SDLEngine
Matt Hoyle
*/
#include "render_pass.h"
#include "device.h"
#include "material.h"
#include "mesh.h"

namespace Render
{
	RenderPass::RenderPass()
	{

	}

	RenderPass::~RenderPass()
	{

	}

	void RenderPass::Reset()
	{
		m_instances.Clear();
	}

	void RenderPass::ApplyRenderState(Device& d)
	{
		d.SetDepthState(m_renderState.m_depthTestEnabled, m_renderState.m_depthWritingEnabled);
	}

	void RenderPass::RenderAll(Device& device)
	{
		// Shadow current state to save driver overhead
		const Mesh* currentMesh = nullptr;
		const ShaderProgram* currentProgram = nullptr;
		const glm::mat4 projectionMatrix = m_camera.ProjectionMatrix();
		const glm::mat4 viewMatrix = m_camera.ViewMatrix();
		ApplyRenderState(device);	// Apply any global render state
		for (auto it : m_instances)
		{
			const glm::mat4& modelTransform = it.GetTransform();
			const Mesh* theMesh = it.GetMesh();
			if (theMesh == nullptr)
			{
				continue;	// Should never occur, but just to be safe
			}
			const Material* theMaterial = theMesh->GetMaterial();
			const ShaderProgram* theShader = theMaterial->GetShaderProgram();

			if (theShader != currentProgram)
			{
				currentProgram = theShader;
				device.BindShaderProgram(*theShader);
			}

			// set any global params
			const auto& globalMaterialParams = theMaterial->GlobalDefinitions();
			const glm::mat4 modelViewProjection = projectionMatrix * viewMatrix * modelTransform;
			device.SetUniformValue(globalMaterialParams.m_mvpUniformHandle, modelViewProjection);

			if (theMesh != currentMesh)
			{
				currentMesh = theMesh;
				device.BindVertexArray(theMesh->GetVertexArray());
			}

			for (auto it : theMesh->GetChunks())
			{
				device.DrawPrimitives(it.m_primitiveType, it.m_firstVertex, it.m_vertexCount);
			}
		}
	}
}