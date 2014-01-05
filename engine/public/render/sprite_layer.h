/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "sprite_instance.h"
#include "kernel/array.h"

namespace Render
{
	class Device;

	// Represents a single 'layer' of sprites to be rendered
	class SpriteLayer
	{
	public:
		SpriteLayer( uint32_t maxSprites );
		~SpriteLayer();

		void PushInstance(const SpriteDefHandle& spriteDefinition, const Math::Vector2& position, const Math::Vector2& size);
		void Clear();
		void DrawInstances(Device& device);

	private:
		Array< SpriteInstance > m_instances;
	};
}