#include "TitleView.h"
#include "../../Button/Button.h"

namespace
{
	static const RectF RECT_STAGE_SELECT_BUTTON =
	{
		100, 500,
		300, 100
	};

	static const RectF RECT_EXIT_BUTTON =
	{
		100, 630,
		300, 100
	};
}

namespace bnscup
{
	TitleView::TitleView()
		: m_pToStageSelectButton{ nullptr }
		, m_pExitButton{ nullptr }
		, m_logoFont{}
		, m_buttonFont{}
	{
		createDisp();
	}

	TitleView::~TitleView()
	{
	}

	void TitleView::update()
	{
		if (m_pToStageSelectButton)
		{
			m_pToStageSelectButton->update();
		}
		if (m_pExitButton)
		{
			m_pExitButton->update();
		}
	}

	void TitleView::draw() const
	{
		// ステージ選択ボタン
		if (m_pToStageSelectButton)
		{
			const auto& selectText = m_buttonFont(U"ステージ選択");
			selectText.draw(m_pToStageSelectButton->getRect());
		}

		// ゲーム終了ボタン
		if (m_pExitButton)
		{
			const auto& selectText = m_buttonFont(U"ゲーム終了");
			selectText.draw(m_pExitButton->getRect());
		}

		// ロゴ
		{
			const auto& logoText = m_logoFont(U"きゅうじょたい");
			const auto& region = logoText.region();
			const double topMargin = 50.0;
			logoText.drawAt(Scene::CenterF().x, region.centerY() + topMargin);
		}
	}

	bool TitleView::isStageSelectButtonSelected() const
	{
		if (m_pToStageSelectButton.get() == nullptr)
		{
			return false;
		}
		return m_pToStageSelectButton->isSelected(Button::Sounds::OK);
	}

	bool TitleView::isExitButtonSelected() const
	{
		if (m_pExitButton.get() == nullptr)
		{
			return false;
		}
		return m_pExitButton->isSelected(Button::Sounds::OK);
	}

	void TitleView::createDisp()
	{
		// 各フォントの設定
		{
			m_logoFont = FontAsset(U"font_title_logo");
			m_buttonFont = FontAsset(U"font_title_button");
		}

		// ステージ選択ボタンの作成
		{
			auto* pStageSelectButton = new Button(RECT_STAGE_SELECT_BUTTON);
			m_pToStageSelectButton.reset(pStageSelectButton);
		}

		// ゲーム終了ボタンの作成
		{
			auto* pExitButton = new Button(RECT_EXIT_BUTTON);
			m_pExitButton.reset(pExitButton);
		}
	}
}

