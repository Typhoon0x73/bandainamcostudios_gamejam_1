#include "Button.h"

namespace bnscup
{
	namespace
	{
		constexpr const char32_t* SOUND_OK = U"sd_button_ok";
		constexpr const char32_t* SOUND_CANCEL = U"sd_button_cancel";

		static const HashTable<Button::Sounds, AssetName> SOUND_TABLE =
		{
			{ Button::Sounds::OK, SOUND_OK },
			{ Button::Sounds::Cancel, SOUND_CANCEL },
		};
	}

	Button::Button(const RectF& rect)
		: m_rect{ rect }
		, m_isHold{ false }
		, m_isSelected{ false }
	{
	}

	Button::~Button()
	{
	}

	void Button::update()
	{
		m_isSelected = false;
		if (MouseL.up())
		{
			if (m_isHold and m_rect.mouseOver())
			{
				m_isSelected = true;
			}
			m_isHold = false;
		}

		if (not(m_isHold))
		{
			m_isHold = m_rect.leftClicked();
		}
	}

	bool Button::isHold() const
	{
		return m_isHold;
	}

	bool Button::isSelected(Sounds sd) const
	{
		if (not(m_isSelected))
		{
			return false;
		}
		AudioAsset(SOUND_TABLE.at(sd)).playOneShot();
		return true;
	}

	RectF& Button::getRect()
	{
		return m_rect;
	}

	const RectF& Button::getRect() const
	{
		return m_rect;
	}
}


