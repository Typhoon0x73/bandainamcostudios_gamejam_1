#include "PauseView.h"
#include "../../../Common/Common.h"
#include "../../../Button/Button.h"

namespace
{
	enum class PauseViewButton
	{
		ReturnStageSelect = 0,
		ReturnTitle,
		Close,

		Count,
	};
	constexpr int32 PauseViewButtonCount = FromEnum(PauseViewButton::Count);

	static const SizeF SIZE_VIEWAREA{ 600, 600 };
	static const SizeF SIZE_BUTTON{ 300, 100 };
}

namespace bnscup
{
	PauseView::PauseView()
		: m_isEnable{ true }
		, m_textFont{}
		, m_buttonFont{}
		, m_viewArea{ Arg::center(Scene::CenterF()), SIZE_VIEWAREA, 10 }
		, m_pButtons{}
	{
		const double buttonMargin = 10.0;
		for(int32 i : step(PauseViewButtonCount))
		{
			Vec2 center = m_viewArea.center();
			center.y += (buttonMargin + SIZE_BUTTON.y) * i;
			auto* pButton = new Button{ RectF{ Arg::center(center), SIZE_BUTTON } };
			m_pButtons.emplace_back(pButton);
		}
		m_textFont = FontAsset(U"font_pause_view");
		m_buttonFont = FontAsset(U"font_button");
	}

	PauseView::~PauseView()
	{
	}

	void PauseView::update()
	{
		if (not(isEnable()))
		{
			return;
		}
		for (auto& pButton : m_pButtons)
		{
			if (pButton)
			{
				pButton->update();
			}
		}
	}

	void PauseView::draw() const
	{
		if (not(isEnable()))
		{
			return;
		}
		m_viewArea
			.drawShadow(Vec2{ 3, 3 }, 10)
			.draw(Palette::Steelblue)
			.drawFrame(1.0, Palette::Black);

		m_textFont(U"ポーズ").drawAt(m_viewArea.center() - Vec2{ 0.0, m_viewArea.h * 0.25 }, Palette::Black);

		// ステージ選択画面へボタン
		{
			const auto* pButton = getButton(FromEnum(PauseViewButton::ReturnStageSelect));
			if (pButton)
			{
				const auto& buttonRect = pButton->getRect();
				buttonRect.rounded(5)
					.drawShadow(Vec2{ 2, 2 }, 5)
					.draw(Palette::Darkolivegreen)
					.drawFrame(1.0, Palette::Black);
				m_buttonFont(U"ステージ選択へ").drawAt(buttonRect.center(), Palette::Black);
			}
		}
		// タイトル画面へボタン
		{
			const auto* pButton = getButton(FromEnum(PauseViewButton::ReturnTitle));
			if (pButton)
			{
				const auto& buttonRect = pButton->getRect();
				buttonRect.rounded(5)
					.drawShadow(Vec2{ 2, 2 }, 5)
					.draw(Palette::Darkolivegreen)
					.drawFrame(1.0, Palette::Black);
				m_buttonFont(U"タイトルへ").drawAt(buttonRect.center(), Palette::Black);
			}
		}
		// 閉じるボタン
		{
			const auto* pButton = getButton(FromEnum(PauseViewButton::Close));
			if (pButton)
			{
				const auto& buttonRect = pButton->getRect();
				buttonRect.rounded(5)
					.drawShadow(Vec2{ 2, 2 }, 5)
					.draw(Palette::Saddlebrown)
					.drawFrame(1.0, Palette::Black);
				m_buttonFont(U"閉じる").drawAt(buttonRect.center(), Palette::Black);
			}
		}
	}

	void PauseView::setEnable(bool isEnable)
	{
		m_isEnable = isEnable;
	}

	bool PauseView::isEnable() const
	{
		return m_isEnable;
	}

	bool PauseView::isReturnStageSelectButtonSelected() const
	{
		int32 index = FromEnum(PauseViewButton::ReturnStageSelect);
		const auto* pButton = getButton(index);
		if (pButton == nullptr)
		{
			return false;
		}
		return pButton->isSelected(Button::Sounds::Select);
	}

	bool PauseView::isReturnTitleButtonSelected() const
	{
		int32 index = FromEnum(PauseViewButton::ReturnTitle);
		const auto* pButton = getButton(index);
		if (pButton == nullptr)
		{
			return false;
		}
		return pButton->isSelected(Button::Sounds::Select);
	}

	bool PauseView::isCloseViewButtonSelected() const
	{
		int32 index = FromEnum(PauseViewButton::Close);
		const auto* pButton = getButton(index);
		if (pButton == nullptr)
		{
			return false;
		}
		return pButton->isSelected(Button::Sounds::Select);
	}

	const Button* PauseView::getButton(int32 index) const
	{
		if (m_pButtons.size() <= index)
		{
			DEBUG_BREAK(true);
			return nullptr;
		}
		return m_pButtons[index].get();
	}

}
