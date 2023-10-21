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

		bool isPlayButtonSelected() const;
		bool isReturnTitleSelected() const;

	private:

		void createDisp();

	private:

		Font m_playButtonFont;
		Texture m_returnMarkIcon;
		std::unique_ptr<Button> m_pPlayGameButton;
		std::unique_ptr<Button> m_pReturnTitleButton;

	};
}

#endif // !BNSCUP_STAGESELECTVIEW_H_
