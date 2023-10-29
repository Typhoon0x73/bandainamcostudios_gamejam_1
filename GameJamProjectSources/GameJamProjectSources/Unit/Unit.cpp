#include "Unit.h"

namespace bnscup
{
	Unit::Unit()
		: m_moveTimer{ 0.0 }
		, m_animTimer{ 0.0 }
		, m_animRectNo{ 0 }
		, m_texture{}
		, m_animRects{}
		, m_pos{}
		, m_targetPos{}
		, m_prevPos{}
		, m_isMirror{ false }
		, m_isEnable{ true }
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

		if (m_targetPos != m_pos) {
			m_moveTimer += Scene::DeltaTime();
			m_pos = Math::Lerp(m_prevPos, m_targetPos, m_moveTimer);
			if (m_moveTimer >= 1.0) {
				m_pos = m_targetPos;
				m_moveTimer = 0.0;
			}
		}
	}

	void Unit::draw() const
	{
		const RectF& srcRect = m_animRects[m_animRectNo].second;
		m_texture(srcRect).mirrored(m_isMirror).draw(m_pos - Vec2{srcRect.w * 0.5, srcRect.h});
	}

	void Unit::setTargetPos(const Vec2& targetPos)
	{
		m_targetPos = targetPos;
		m_prevPos = m_pos;
		m_moveTimer = 0.0;
	}

	void Unit::setPos(const Vec2& pos)
	{
		m_pos = pos;
		m_targetPos = pos;
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

	bool Unit::isMoving() const
	{
		return (m_targetPos != m_pos);
	}

	void Unit::setMirror(bool isMirror)
	{
		m_isMirror = isMirror;
	}

	bool Unit::isEnable() const
	{
		return m_isEnable;
	}

	void Unit::setEnable(bool isEnable)
	{
		m_isEnable = isEnable;
	}

}
