/*
SDLEngine
Matt Hoyle
*/
#include "uniform_buffer.h"
#include "core/string_hashing.h"

namespace Render
{
	void UniformBuffer::SetValue(const char* name, const glm::vec4& value)
	{
		const uint32_t hash = Core::StringHashing::GetHash(name);
		m_vec4Values[hash] = value;
	}
}