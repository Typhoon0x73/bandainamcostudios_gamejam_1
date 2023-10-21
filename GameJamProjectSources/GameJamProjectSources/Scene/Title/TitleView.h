#pragma once
#ifndef BNSCUP_TITLEVIEW_H_
#define BNSCUP_TITLEVIEW_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class Button;
	class TitleView
	{
	public:

		explicit TitleView();
		virtual ~TitleView();

		void update();
		void draw() const;

		bool isStageSelectButtonSelected() const;
		bool isExitButtonSelected() const;

	private:

		void createDisp();

	private:

		Font m_logoFont;
		Font m_buttonFont;
		std::unique_ptr<Button> m_pToStageSelectButton;
		std::unique_ptr<Button> m_pExitButton;
	};
}

#endif // !BNSCUP_TITLEVIEW_H_
