/*
SDLEngine
Matt Hoyle
*/
#include "uniform_buffer.h"
#include "texture.h"
#include "core/string_hashing.h"

namespace Render
{
	void UniformBuffer::SetValue(const char* name, const glm::mat4& value)
	{
		const uint32_t hash = Core::StringHashing::GetHash(name);
		m_mat4Values[hash] = value;
	}

	void UniformBuffer::SetValue(const char* name, const glm::vec4& value)
	{
		const uint32_t hash = Core::StringHashing::GetHash(name);
		m_vec4Values[hash] = value;
	}

	void UniformBuffer::SetSampler(const char* name, uint32_t handle)
	{
		const uint32_t hash = Core::StringHashing::GetHash(name);
		m_textureSamplers[hash] = handle;
	}

	void UniformBuffer::SetArraySampler(const char* name, uint32_t handle)
	{
		const uint32_t hash = Core::StringHashing::GetHash(name);
		m_textureArraySamplers[hash] = handle;
	}
}