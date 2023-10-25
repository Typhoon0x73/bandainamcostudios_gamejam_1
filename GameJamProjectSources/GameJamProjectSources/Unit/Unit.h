#pragma once
#ifndef BNSCUP_UNIT_H_
#define BNSCUP_UNIT_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class Unit
	{
	public:

		explicit Unit();
		virtual ~Unit();

		void update();
		void draw() const;

		void setTargetPos(const Vec2& targetPos);

		void setPos(const Vec2& pos);
		const Vec2& getPos() const;

		void setTexture(AssetNameView assetName);
		void setAnimRect(const Array<std::pair<Duration, RectF>>& animRects);

		bool isMoving() const;

		void setMirror(bool isMirror);

	private:

		double m_moveTimer;
		double m_animTimer;
		uint32 m_animRectNo;
		Texture m_texture;
		Array<std::pair<Duration, RectF>> m_animRects;
		Vec2 m_pos;
		Vec2 m_targetPos;
		Vec2 m_prevPos;
		bool m_isMirror;
	};
}

#endif // !BNSCUP_UNIT_H_
