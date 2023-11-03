#pragma once
#ifndef BNSCUP_TELEPORTANIM_H_
#define BNSCUP_TELEPORTANIM_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class TeleportAnim
	{
	public:

		explicit TeleportAnim();
		virtual ~TeleportAnim();

		void update();
		void draw() const;

		void reset();
		void setPos(const Vec2& pos);
		void setEnable(bool isEnable);

		bool isEnd() const;
		bool isEnable() const;

	private:

		bool m_isEnable;
		double m_timer;
		int32 m_index;
		Array<Texture> m_textures;
		Vec2 m_pos;
		Audio m_se;
	};
}

#endif // !BNSCUP_TELEPORTANIM_H_
