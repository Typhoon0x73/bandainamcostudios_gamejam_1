#pragma once
#ifndef BNSCUP_PAUSEVIEW_H_
#define BNSCUP_PAUSEVIEW_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class Button;

	class PauseView
	{
	public:

		explicit PauseView();
		virtual ~PauseView();

		void update();
		void draw() const;

		void setEnable(bool isEnable);
		bool isEnable() const;

		bool isReturnStageSelectButtonSelected() const;
		bool isReturnTitleButtonSelected() const;
		bool isCloseViewButtonSelected() const;

	private:

		const Button* getButton(int32 index) const;

	private:

		bool m_isEnable;
		Font m_textFont;
		Font m_buttonFont;
		RoundRect m_viewArea;
		Array<std::unique_ptr<Button>> m_pButtons;
	};
}

#endif // !BNSCUP_PAUSEVIEW_H_
