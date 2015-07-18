/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <glm.hpp>
#include <unordered_map>

namespace Render
{
	// Contains a set of name -> value pairs for a set of uniforms
	class UniformBuffer
	{
	public:
		UniformBuffer() { }
		~UniformBuffer() { }

		void SetValue(const char* name, const glm::vec4& value);
		const std::unordered_map<uint32_t, glm::vec4>& Vec4Values() const { return m_vec4Values; }
		
	private:
		std::unordered_map<uint32_t, glm::vec4> m_vec4Values;
	};
}