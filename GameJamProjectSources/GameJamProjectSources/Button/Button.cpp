#include "Button.h"

namespace bnscup
{
	namespace
	{
		constexpr const char32_t* SOUND_OK = U"sd_button_ok";
		constexpr const char32_t* SOUND_CANCEL = U"sd_button_cancel";
		constexpr const char32_t* SOUND_SELECT = U"sd_button_select";

		static const HashTable<Button::Sounds, AssetName> SOUND_TABLE =
		{
			{ Button::Sounds::OK, SOUND_OK },
			{ Button::Sounds::Cancel, SOUND_CANCEL },
			{ Button::Sounds::Select, SOUND_SELECT },
		};
	}

	Button::Button(const RectF& rect)
		: m_rect{ rect }
		, m_isHold{ false }
		, m_isSelected{ false }
		, m_isEnable{ true }
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

	void Button::setEnable(bool enable)
	{
		m_isEnable = enable;
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

	bool Button::isEnable() const
	{
		return m_isEnable;
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


