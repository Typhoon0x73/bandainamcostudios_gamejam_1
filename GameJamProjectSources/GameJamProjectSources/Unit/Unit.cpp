#include "Unit.h"

namespace bnscup
{
	Unit::Unit()
		: m_animTimer{ 0.0 }
		, m_animRectNo{ 0 }
		, m_texture{}
		, m_animRects{}
		, m_pos{}
	{
	}

	Unit::~Unit()
	{
	}

	void Unit::update()
	{
		m_animTimer += Scene::DeltaTime();
		if (m_animRects[m_animRectNo].first.count() <= m_animTimer) {
			m_animRectNo++;
			m_animTimer = 0.0;
			if (m_animRectNo >= m_animRects.size()) {
				m_animRectNo = 0;
			}
		}
	}

	void Unit::draw() const
	{
		const RectF& srcRect = m_animRects[m_animRectNo].second;
		m_texture(srcRect).draw(m_pos - Vec2{ srcRect.w * 0.5, srcRect.h });
	}

	void Unit::setPos(const Vec2& pos)
	{
		m_pos = pos;
	}

	const Vec2& Unit::getPos() const
	{
		return m_pos;
	}

	void Unit::setTexture(AssetNameView assetName)
	{
		m_texture = TextureAsset(assetName);
	}

	void Unit::setAnimRect(const Array<std::pair<Duration, RectF>>& animRects)
	{
		m_animRects = animRects;
	}

}
