/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <glm.hpp>
#include <unordered_map>

namespace Render
{
	class Texture;

	// Contains a set of name -> value pairs for a set of uniforms + samplers
	class UniformBuffer
	{
	public:
		UniformBuffer() { }
		~UniformBuffer() { }

		void SetValue(const char* name, const glm::vec4& value);
		void SetValue(const char* name, const Texture* tex);

		const std::unordered_map<uint32_t, glm::vec4>& Vec4Values() const { return m_vec4Values; }
		const std::unordered_map<uint32_t, uint32_t>& Samplers() const { return m_textureSamplers; }
		const std::unordered_map<uint32_t, uint32_t>& ArraySamplers() const { return m_textureArraySamplers; }
		
	private:
		std::unordered_map<uint32_t, glm::vec4> m_vec4Values;
		std::unordered_map<uint32_t, uint32_t> m_textureSamplers;
		std::unordered_map<uint32_t, uint32_t> m_textureArraySamplers;
	};
}