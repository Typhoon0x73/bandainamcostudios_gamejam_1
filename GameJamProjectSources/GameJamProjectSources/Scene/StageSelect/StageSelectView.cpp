#include "StageSelectView.h"
#include "../../Common/Common.h"
#include "../../Button/Button.h"

namespace
{
	static const RoundRect ROUNDRECT_BASE =
	{
		70, 70,
		bnscup::WINDOW_SIZE_W - 70 * 2,
		bnscup::WINDOW_SIZE_H - 70 * 2,
		10
	};

	static const RectF RECT_PLAY_GAME_BUTTON =
	{
		870, 750,
		300, 100
	};

	static const RectF RECT_RETURN_TITLE_BUTTON =
	{
		60, 60, 50, 50
	};
}

namespace bnscup
{

	StageSelectView::StageSelectView()
		: m_pPlayGameButton{ nullptr }
		, m_pReturnTitleButton{ nullptr }
		, m_playButtonFont{}
		, m_returnMarkIcon{}
	{
		createDisp();
	}

	StageSelectView::~StageSelectView()
	{
	}

	void StageSelectView::update()
	{
		if (m_pPlayGameButton)
		{
			m_pPlayGameButton->update();
		}
		if (m_pReturnTitleButton)
		{
			m_pReturnTitleButton->update();
		}
	}

	void StageSelectView::draw() const
	{
		ROUNDRECT_BASE.draw(Palette::Darkkhaki).drawFrame(2.0, Palette::Khaki);

		if (m_pPlayGameButton)
		{
			const auto& rect = m_pPlayGameButton->getRect();
			const auto& playText = m_playButtonFont(U"ゲーム開始");
			rect.rounded(5).draw(Palette::DefaultBackground).drawFrame();
			playText.drawAt(rect.center());
		}

		if (m_pReturnTitleButton)
		{
			const auto& rect = m_pReturnTitleButton->getRect();
			rect.rounded(5).draw(Palette::Orangered).drawFrame();
			m_returnMarkIcon.drawAt(rect.center());
		}
	}

	bool StageSelectView::isPlayButtonSelected() const
	{
		if (m_pPlayGameButton.get() == nullptr)
		{
			return false;
		}
		return m_pPlayGameButton->isSelected(Button::Sounds::OK);
	}

	bool StageSelectView::isReturnTitleSelected() const
	{
		if (m_pReturnTitleButton.get() == nullptr)
		{
			return false;
		}
		return m_pReturnTitleButton->isSelected(Button::Sounds::Cancel);
	}

	void StageSelectView::createDisp()
	{
		// フォントの設定
		{
			m_playButtonFont = FontAsset(U"font_play_button");
			DEBUG_BREAK(m_playButtonFont.isEmpty());
		}

		// アイコンの設定
		{
			m_returnMarkIcon = TextureAsset(U"icon_return_mark");
			DEBUG_BREAK(m_returnMarkIcon.isEmpty());
		}

		// ゲーム開始ボタンの作成
		{
			auto* pPlayButton = new Button(RECT_PLAY_GAME_BUTTON);
			m_pPlayGameButton.reset(pPlayButton);
		}

		// タイトルへ戻るボタンの作成
		{
			auto* pReturnTitleButton = new Button(RECT_RETURN_TITLE_BUTTON);
			m_pReturnTitleButton.reset(pReturnTitleButton);
		}
	}

}
