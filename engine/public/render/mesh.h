/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "vertex_array.h"
#include "render_buffer.h"

namespace Render
{
	class Material;

	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		void Destroy();

		// this is shit, refactor!
		inline std::vector<RenderBuffer>& GetStreams()	{ return m_vertexStreams; }
		inline VertexArray& GetVertexArray() { return m_vertices; }

	private:
		VertexArray m_vertices;
		std::vector<RenderBuffer> m_vertexStreams;
		Material* m_material;
	};
}