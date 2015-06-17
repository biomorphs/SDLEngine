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
		Destroy();
	}

	void Mesh::Destroy()
	{
		for (auto& it : m_vertexStreams)
		{
			it.Destroy();
		}
		m_vertexStreams.clear();
		m_vertices.Destroy();
		m_material = nullptr;
	}
}