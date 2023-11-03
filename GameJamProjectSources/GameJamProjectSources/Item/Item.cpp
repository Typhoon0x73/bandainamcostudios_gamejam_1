#include "Item.h"
#include "../Unit/Unit.h"

namespace bnscup
{

	Item::Item(Type type)
		: m_type{ type }
		, m_pOwner{ nullptr }
		, m_pos{ Vec2::Zero() }
		, m_texture{}
		, m_srcRect{ RectF::Empty() }
		, m_isEnd{ false }
	{
	}

	Item::~Item()
	{
	}

	void Item::draw() const
	{
		Vec2 pos = m_pos;
		m_texture(m_srcRect).drawAt(pos);
	}

	void Item::notifyEnd()
	{
		m_isEnd = true;
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

	void Item::setOwner(const Unit* pUnit)
	{
		m_pOwner = pUnit;
	}

	const Vec2& Item::getPos() const
	{
		return m_pos;
	}

	Item::Type Item::getType() const
	{
		return m_type;
	}

	bool Item::existOwer() const
	{
		return (m_pOwner != nullptr);
	}

	bool Item::isKey() const
	{
		return (m_type == Type::GoldKey);
	}

	bool Item::isEnd() const
	{
		return m_isEnd;
	}

}
