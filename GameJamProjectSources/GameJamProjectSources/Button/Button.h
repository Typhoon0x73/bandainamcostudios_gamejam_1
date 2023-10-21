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
		};

	public:

		explicit Button(const RectF& rect);
		virtual ~Button();

		void update();

		bool isHold() const;

		bool isSelected(Sounds sd) const;

		RectF& getRect();
		const RectF& getRect() const;

	private:

		RectF m_rect;
		bool m_isHold;
		bool m_isSelected;
	};
}

#endif // !BNSCUP_BUTTON_H_
