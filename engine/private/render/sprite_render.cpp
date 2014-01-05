/*
SDLEngine
Matt Hoyle
*/

#include "sprite_render.h"
#include "sprite_layer.h"
#include "device.h"

namespace Render
{
	SpriteRender::SpriteRender(Device* renderDevice, uint32_t maxLayers)
		: m_device(renderDevice)
	{
		m_layers.Reserve(maxLayers);
	}

	SpriteRender::~SpriteRender()
	{
		uint32_t layerCount = m_layers.Size();
		for (uint32_t index = 0; index < layerCount; ++index)
		{
			delete m_layers[index];
			m_layers[index] = nullptr;
		}
	}

	uint32_t SpriteRender::AddLayer(uint32_t maxInstances)
	{
		SpriteLayer** newLayer = m_layers.PushBack();
		if (newLayer)
		{
			*newLayer = new SpriteLayer(maxInstances);
			return m_layers.Size() - 1;
		}

		return InvalidLayer;
	}

	SpriteLayer* SpriteRender::GetLayer(uint32_t index)
	{
		return m_layers[index];
	}

	void SpriteRender::DrawLayers()
	{
		uint32_t layerCount = m_layers.Size();
		for (uint32_t index = 0; index < layerCount; ++index)
		{
			m_layers[index]->DrawInstances(*m_device);
		}
	}
}
