#pragma once
#ifndef BNSCUP_STAGESELECTVIEW_H_
#define BNSCUP_STAGESELECTVIEW_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class Button;
	class StageSelectView
	{
	public:

		explicit StageSelectView();
		virtual ~StageSelectView();

		void update();
		void draw() const;

		int32 getSelectStageNo() const;
		bool isPlayButtonSelected() const;
		bool isReturnTitleSelected() const;

	private:

		void createDisp();

	private:

		Font m_playButtonFont;
		Font m_stageNoFont;
		Texture m_returnMarkIcon;

		int32 m_selectStageNo;
		Array<std::unique_ptr<Button>> m_pStageButtons;

		std::unique_ptr<Button> m_pPlayGameButton;
		std::unique_ptr<Button> m_pReturnTitleButton;

	};
}

#endif // !BNSCUP_STAGESELECTVIEW_H_
