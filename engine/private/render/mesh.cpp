/*
SDLEngine
Matt Hoyle
*/
#include "mesh.h"

namespace Render
{
	Mesh::Mesh()
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Destroy()
	{
		for (auto& it : m_vertexStreams)
		{
			it.Destroy();
		}
		m_vertices.Destroy();
	}
}