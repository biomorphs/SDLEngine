/*
SDLEngine
Matt Hoyle
*/

#include "sprite_manager.h"

namespace Render
{
	SpriteManager::SpriteManager()
	{

	}

	SpriteManager::~SpriteManager()
	{
		SDE_ASSERT(m_spriteBucket.Size() == 0, "There are still sprite definitions loaded!");
	}

	SpriteDefHandle SpriteManager::AddSpriteDefinition(const TextureHandle& texture, const Math::Rect& spriteRect)
	{
		RefcountedSpriteDefinition* newDefinition = m_spriteBucket.AddObject();
		SDE_ASSERT(newDefinition);

		SpriteDefinition theSprite;
		theSprite.m_sourceRect = spriteRect;
		theSprite.m_texture = texture;
		*newDefinition = std::move(RefcountedSpriteDefinition(theSprite));

		return SpriteDefHandle(newDefinition);
	}

	void SpriteManager::Flush()
	{
		auto it = m_spriteBucket.Begin();
		while( it != m_spriteBucket.End() )
		{
			if ((*it).GetReferenceCount() == 0)
			{
				m_spriteBucket.RemoveObject(&(*it));
				it = m_spriteBucket.Begin();
				continue;
			}
			++it;
		}
	}
}