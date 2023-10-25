#pragma once
#ifndef BNSCUP_ITEM_H_
#define BNSCUP_ITEM_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class Item
	{
	public:

		explicit Item();
		virtual ~Item();

		void draw() const;

		void setPos(const Vec2& pos);
		void setTexture(AssetNameView name);
		void setSrcRect(const RectF& srcRect);

	private:

		Vec2 m_pos;
		Texture m_texture;
		RectF m_srcRect;
	};
}

#endif // !BNSCUP_ITEM_H_
