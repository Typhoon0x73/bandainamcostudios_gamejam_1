#pragma once
#ifndef BNSCUP_ITEM_H_
#define BNSCUP_ITEM_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class Unit;
	class Item
	{
	public:

		enum class Type
		{
			GoldKey,
		};

		explicit Item(Type type);
		virtual ~Item();

		void draw() const;

		void notifyEnd();

		void setPos(const Vec2& pos);
		void setTexture(AssetNameView name);
		void setSrcRect(const RectF& srcRect);
		void setOwner(const Unit* pUnit);
		const Vec2& getPos() const;

		Item::Type getType() const;

		bool existOwer() const;

		bool isKey() const;

		bool isEnd() const;

	private:

		Type m_type;
		const Unit* m_pOwner;

		Vec2 m_pos;
		Texture m_texture;
		RectF m_srcRect;

		bool m_isEnd;
	};
}

#endif // !BNSCUP_ITEM_H_
