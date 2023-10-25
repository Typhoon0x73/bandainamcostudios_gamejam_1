#include "Item.h"

namespace bnscup
{

	Item::Item()
		: m_pos{ Vec2::Zero() }
		, m_texture{}
		, m_srcRect{ RectF::Empty() }
	{
	}

	Item::~Item()
	{
	}

	void Item::draw() const
	{
		Vec2 pos = m_pos;
		pos.y + m_srcRect.h * 0.5;
		m_texture(m_srcRect).drawAt(pos);
	}

	void Item::setPos(const Vec2& pos)
	{
		m_pos = pos;
	}

	void Item::setTexture(AssetNameView name)
	{
		m_texture = TextureAsset(name);
	}

	void Item::setSrcRect(const RectF& srcRect)
	{
		m_srcRect = srcRect;
	}

}
