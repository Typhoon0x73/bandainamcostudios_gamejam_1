#pragma once
#ifndef BNSCUP_MESSAGEBOX_H_
#define BNSCUP_MESSAGEBOX_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class Button;
	class MessageBox
	{
	public:

		enum class ButtonStyle : uint8
		{
			None = 0,
			OnlyOK,
			OKCancel,
			YesNo,
		};

		using ExistCrossButton = YesNo<struct ExistCrossButton_tag>;

		explicit MessageBox(ButtonStyle btnStyle, ExistCrossButton existCross, StringView bodyMessage);
		virtual ~MessageBox();

		void update();
		void draw() const;

		static void SetFont(const Font* font);

		bool isPositiveSelected() const;
		bool isNegativeSelected() const;

		bool isYesSelected() const;
		bool isNoSelected() const;
		bool isOKSelected() const;
		bool isCancelSelected() const;
		bool isExitCrossSelected() const;

	private:

		void calcRegion();

	private:

		static const Font* m_spFont;
		Texture m_crossTexture;
		RectF m_bodyRect;
		RectF m_messageRect;
		ButtonStyle m_buttonStyle;
		String m_bodyMessage;
		String m_positiveText;
		String m_negativeText;
		std::unique_ptr<Button> m_pPositiveButton;
		std::unique_ptr<Button> m_pNegativeButton;
		std::unique_ptr<Button> m_pExitCrossButton;
	};
}

#endif // !BNSCUP_MESSAGEBOX_H_
