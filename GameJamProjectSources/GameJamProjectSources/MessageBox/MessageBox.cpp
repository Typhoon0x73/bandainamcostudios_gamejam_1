#include "MessageBox.h"
#include "../Button/Button.h"

namespace bnscup
{
	const Font* MessageBox::m_spFont = nullptr;

	MessageBox::MessageBox(ButtonStyle btnStyle, ExistCrossButton existCross, StringView bodyMessage)
		: m_crossTexture{}
		, m_bodyRect{ RectF::Empty() }
		, m_buttonStyle{ btnStyle }
		, m_bodyMessage{ bodyMessage }
		, m_positiveText{}
		, m_negativeText{}
		, m_pPositiveButton{ nullptr }
		, m_pNegativeButton{ nullptr }
		, m_pExitCrossButton{ nullptr }
	{
		m_crossTexture = TextureAsset(U"icon_cross");
		switch (m_buttonStyle)
		{
		case ButtonStyle::OKCancel:
		{
			auto* pNegativeButton = new Button(RectF::Empty());
			m_negativeText = U"CANCEL";
			m_pNegativeButton.reset(pNegativeButton);
		}
		[[fallthrough]];
		case ButtonStyle::OnlyOK:
		{
			auto* pPositiveButton = new Button(RectF::Empty());
			m_positiveText = U"OK";
			m_pPositiveButton.reset(pPositiveButton);
			break;
		}
		case ButtonStyle::YesNo:
		{
			auto* pNegativeButton = new Button(RectF::Empty());
			m_negativeText = U"いいえ";
			m_pNegativeButton.reset(pNegativeButton);

			auto* pPositiveButton = new Button(RectF::Empty());
			m_positiveText = U"はい";
			m_pPositiveButton.reset(pPositiveButton);
			break;
		}
		default:
			break;
		}
		if (existCross)
		{
			auto* pCrossButton = new Button(RectF::Empty());
			m_pExitCrossButton.reset(pCrossButton);
		}
	}

	MessageBox::~MessageBox()
	{
	}

	void MessageBox::update()
	{
		if (m_bodyRect.isEmpty())
		{
			calcRegion();
		}

		if (m_pPositiveButton)
		{
			m_pPositiveButton->update();
		}
		if (m_pNegativeButton)
		{
			m_pNegativeButton->update();
		}
		if (m_pExitCrossButton)
		{
			m_pExitCrossButton->update();
		}
	}

	void MessageBox::draw() const
	{
		if (m_spFont == nullptr)
		{
			return;
		}

		const double margin = 10.0;
		m_bodyRect.rounded(margin * 0.5).draw(Palette::Darkgreen).drawFrame();
		m_messageRect.rounded(margin * 0.5).draw(Palette::Whitesmoke);
		(*m_spFont)(m_bodyMessage).drawAt(m_messageRect.center(), Palette::Black);
		if (m_pPositiveButton)
		{
			const auto& buttonRect = m_pPositiveButton->getRect();
			buttonRect.rounded(margin * 0.5).draw(Palette::Midnightblue).drawFrame();
			(*m_spFont)(m_positiveText).drawAt(buttonRect.center(), Palette::Black);
		}
		if (m_pNegativeButton)
		{
			const auto& buttonRect = m_pNegativeButton->getRect();
			buttonRect.rounded(margin * 0.5).draw(Palette::Whitesmoke).drawFrame();
			(*m_spFont)(m_negativeText).drawAt(buttonRect.center(), Palette::Black);
		}
		if (m_pExitCrossButton)
		{
			const auto& buttonRect = m_pExitCrossButton->getRect();
			buttonRect.rounded(margin * 0.5).draw(Palette::Darkred).drawFrame();
			m_crossTexture.drawAt(buttonRect.center());
		}
	}

	void MessageBox::SetFont(const Font* font)
	{
		m_spFont = font;
	}

	bool MessageBox::isPositiveSelected() const
	{
		if (m_pPositiveButton.get() == nullptr)
		{
			return false;
		}
		return m_pPositiveButton->isSelected(Button::Sounds::OK);
	}

	bool MessageBox::isNegativeSelected() const
	{
		if (m_pNegativeButton.get() == nullptr)
		{
			return false;
		}
		return m_pNegativeButton->isSelected(Button::Sounds::Cancel);
	}

	bool MessageBox::isOKSelected() const
	{
		return isPositiveSelected();
	}

	bool MessageBox::isCancelSelected() const
	{
		return isNegativeSelected();
	}

	bool MessageBox::isYesSelected() const
	{
		return isPositiveSelected();
	}

	bool MessageBox::isNoSelected() const
	{
		return isNegativeSelected();
	}

	bool MessageBox::isExitCrossSelected() const
	{
		if (m_pExitCrossButton.get() == nullptr)
		{
			return false;
		}
		return m_pExitCrossButton->isSelected(Button::Sounds::Cancel);
	}

	void MessageBox::calcRegion()
	{
		if (m_spFont == nullptr)
		{
			m_spFont = &(SimpleGUI::GetFont());
		}

		const double margin = 10.0;

		m_bodyRect = (*m_spFont)(m_bodyMessage).region();
		auto buttonRegion = (*m_spFont)(U"CANCEL").region();
		buttonRegion = buttonRegion.stretched(margin * 0.5, margin, margin * 0.5, margin);

		double minWidth = buttonRegion.w * 2 + margin * 3;
		if (m_bodyRect.w < minWidth)
		{
			m_bodyRect.w = minWidth;
		}
		m_bodyRect.setCenter(Scene::CenterF());

		const double buttonPosY = m_bodyRect.bottomY() + margin;
		if (m_pPositiveButton)
		{
			auto& positiveRect = m_pPositiveButton->getRect();
			positiveRect.set(buttonRegion);
			switch (m_buttonStyle)
			{
			case ButtonStyle::OnlyOK:
			{
				Vec2 center{ m_bodyRect.centerX(), buttonPosY + buttonRegion.h * 0.5 };
				positiveRect.setCenter(center);
				break;
			}
			case ButtonStyle::OKCancel:
			case ButtonStyle::YesNo:
			{
				Vec2 pos{ m_bodyRect.centerX() - (positiveRect.w + margin * 0.5), buttonPosY };
				positiveRect.setPos(pos);
				break;
			}
			case ButtonStyle::None:
			default: break;
			}
		}
		if (m_pNegativeButton)
		{
			auto& negativeRect = m_pNegativeButton->getRect();
			negativeRect.set(buttonRegion);
			switch (m_buttonStyle)
			{
			case ButtonStyle::OKCancel:
			case ButtonStyle::YesNo:
			{
				Vec2 pos{ m_bodyRect.centerX() + (margin * 0.5), buttonPosY };
				negativeRect.setPos(pos);
				break;
			}
			case ButtonStyle::OnlyOK:
			case ButtonStyle::None:
			default: break;
			}
		}

		double bottomMargin = margin;
		m_messageRect = m_bodyRect;
		m_messageRect = m_messageRect.stretched(margin * 0.5);
		if (m_pPositiveButton)
		{
			bottomMargin += m_pPositiveButton->getRect().h + margin;
		}
		m_bodyRect = m_bodyRect.stretched(margin, margin, bottomMargin, margin);

		if (m_pExitCrossButton)
		{
			auto& crossButton = m_pExitCrossButton->getRect();
			crossButton.setSize(30, 30);
			crossButton.setPos(m_bodyRect.tr() - (Vec2{ crossButton.size.x - margin, margin }));
		}
	}
}
