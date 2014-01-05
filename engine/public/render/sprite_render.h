/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>
#include "kernel/array.h"

namespace Render
{
	class Device;
	class SpriteLayer;

	// Represents a list of sprite layers that will be drawn in order
	class SpriteRender
	{
	public:
		SpriteRender(Device* renderDevice, uint32_t maxLayers);
		~SpriteRender();

		uint32_t AddLayer(uint32_t maxInstances);
		SpriteLayer* GetLayer(uint32_t index);

		void DrawLayers();

		static const uint32_t InvalidLayer = (uint32_t)-1;

	private:
		Array< SpriteLayer* > m_layers;
		Device* m_device;
	};
}