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

	static const RectF RECT_STAGE_BUTTON =
	{
		ROUNDRECT_BASE.x + 50, ROUNDRECT_BASE.y + 50,
		200, 100
	};
}

namespace bnscup
{

	StageSelectView::StageSelectView()
		: m_pPlayGameButton{ nullptr }
		, m_pReturnTitleButton{ nullptr }
		, m_selectStageNo{}
		, m_pStageButtons{}
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
		for (size_t i : step(m_pStageButtons.size()))
		{
			auto* pButton = m_pStageButtons[i].get();
			pButton->update();
			if (pButton->isSelected(Button::Sounds::Select))
			{
				m_selectStageNo = static_cast<int32>(i);
			}
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

		for (size_t i : step(m_pStageButtons.size()))
		{
			auto* pButton = m_pStageButtons[i].get();
			const auto& buttonRect = pButton->getRect();
			buttonRect.rounded(5).draw(Palette::Palegoldenrod).drawFrame();
			m_stageNoFont(U"ステージ{}"_fmt(i + 1)).drawAt(buttonRect.center(), Palette::Black);
			if (i == m_selectStageNo)
			{
				buttonRect.rounded(5).drawFrame(1.0, Palette::Blue);
			}
		}
	}

	int32 StageSelectView::getSelectStageNo() const
	{
		return m_selectStageNo;
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
			m_stageNoFont = FontAsset(U"font_stage_no_button");
			DEBUG_BREAK(m_stageNoFont.isEmpty());
		}

		// アイコンの設定
		{
			m_returnMarkIcon = TextureAsset(U"icon_return_mark");
			DEBUG_BREAK(m_returnMarkIcon.isEmpty());
		}

		// ステージ選択ボタン
		for (int32 i : step(3))
		{
			auto* pButton = new Button(RECT_STAGE_BUTTON.movedBy(Vec2{ i * (RECT_STAGE_BUTTON.size.x + 10.0), 0.0}));
			m_pStageButtons.emplace_back(pButton);
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
