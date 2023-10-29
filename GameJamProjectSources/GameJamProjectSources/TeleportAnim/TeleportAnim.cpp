#include "TeleportAnim.h"

namespace bnscup
{
	TeleportAnim::TeleportAnim()
		: m_timer{ 0.0 }
		, m_index{ 0 }
		, m_textures{}
		, m_isEnable{ false }
	{
		for (int32 i : step(100))
		{
			m_textures.emplace_back(TextureAsset(U"teleport_anim_{:0>4}"_fmt(i + 1)));
		}
	}

	TeleportAnim::~TeleportAnim()
	{
	}

	void TeleportAnim::update()
	{
		if (isEnd() or not(isEnable()))
		{
			return;
		}
		m_timer += Scene::DeltaTime();
		if (m_timer > 0.032)
		{
			m_timer = 0.0;
			m_index++;
		}
	}

	void TeleportAnim::draw() const
	{
		if (isEnd() or not(isEnable()))
		{
			return;
		}
		const double scale = 0.5;
		int32 h = m_textures[m_index].height();
		const double offset = h * 0.15 * scale;
		Vec2 pos = m_pos - Vec2{ -3.0, h * 0.5 * scale - offset };
		m_textures[m_index].scaled(scale).drawAt(pos);
	}

	void TeleportAnim::reset()
	{
		m_index = 0;
		m_timer = 0.0;
		m_isEnable = false;
	}

	void TeleportAnim::setPos(const Vec2& pos)
	{
		m_pos = pos;
	}

	void TeleportAnim::setEnable(bool isEnable)
	{
		m_isEnable = isEnable;
	}

	bool TeleportAnim::isEnd() const
	{
		return (m_index == m_textures.size());
	}

	bool TeleportAnim::isEnable() const
	{
		return m_isEnable;
	}

}



