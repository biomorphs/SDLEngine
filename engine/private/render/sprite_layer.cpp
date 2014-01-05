/*
SDLEngine
Matt Hoyle
*/

#include "sprite_layer.h"
#include "device.h"

namespace Render
{
	SpriteLayer::SpriteLayer(uint32_t maxSprites)
	{
		m_instances.Reserve(maxSprites);
	}

	SpriteLayer::~SpriteLayer()
	{

	}

	void SpriteLayer::DrawInstances(Device& device)
	{
		// draw stuff
		uint32_t instanceCount = m_instances.Size();
		for (uint32_t i = 0; i < instanceCount; ++i)
		{
			device.DrawSprite(**m_instances[i].m_spriteDefinition->m_texture,
				m_instances[i].m_spriteDefinition->m_sourceRect,
				m_instances[i].m_destination,
				m_instances[i].m_rotation,
				m_instances[i].m_rotOrigin);
		}

		Clear();
	}

	void SpriteLayer::PushInstance(const SpriteDefHandle& spriteDefinition, const Math::Vector2& position, const Math::Vector2& size)
	{
		SpriteInstance* newInstance = m_instances.PushBack();
		if (newInstance)
		{
			newInstance->m_spriteDefinition = spriteDefinition;
			newInstance->m_rotation = 0.0f;
			newInstance->m_rotOrigin = Math::Vector2(0, 0);
			newInstance->m_destination = Math::Rect(position, size);
		}
	}

	void SpriteLayer::Clear()
	{
		m_instances.ResizeFast(0);
	}
}