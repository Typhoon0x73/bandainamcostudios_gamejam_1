#pragma once
#ifndef BNSCUP_UNIT_H_
#define BNSCUP_UNIT_H_

namespace bnscup
{
	class Unit
	{
	public:

		explicit Unit();
		virtual ~Unit();

		void update();
		void draw() const;

		void setPos(const Vec2& pos);
		const Vec2& getPos() const;

		void setTexture(AssetNameView assetName);
		void setAnimRect(const Array<std::pair<Duration, RectF>>& animRects);

	private:

		double m_animTimer;
		uint32 m_animRectNo;
		Texture m_texture;
		Array<std::pair<Duration, RectF>> m_animRects;
		Vec2 m_pos;
		Vec2 m_targetPos;
	};
}

#endif // !BNSCUP_UNIT_H_
