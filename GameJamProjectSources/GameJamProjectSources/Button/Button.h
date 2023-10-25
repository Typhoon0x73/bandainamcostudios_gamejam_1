#pragma once
#ifndef BNSCUP_BUTTON_H_
#define BNSCUP_BUTTON_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class Button
	{
	public:

		enum class Sounds
		{
			OK,
			Cancel,
			Select,
		};

	private:

		enum class CollisionType
		{
			Rect,
			Circle,
		};

	public:

		explicit Button(const RectF& rect);
		explicit Button(const Circle& circle);
		virtual ~Button();

		void update();

		void setEnable(bool enable);

		bool isHold() const;

		bool isSelected(Sounds sd) const;

		bool isEnable() const;

		RectF& getRect();
		const RectF& getRect() const;

		Circle& getCircle();
		const Circle& getCircle() const;

	private:

		CollisionType m_collisionType;
		RectF m_rect;
		Circle m_circle;
		bool m_isHold;
		bool m_isSelected;
		bool m_isEnable;
	};
}

#endif // !BNSCUP_BUTTON_H_
