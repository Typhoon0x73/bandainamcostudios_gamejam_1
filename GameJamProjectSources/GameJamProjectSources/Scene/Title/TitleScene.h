#pragma once
#ifndef BNSCUP_TITLESCENE_H_
#define BNSCUP_TITLESCENE_H_

#include "../SceneDefine.h"

namespace bnscup
{
	class TitleScene : public GameApp::Scene
	{
		using super = GameApp::Scene;
	public:

		explicit TitleScene(const super::InitData& init);
		virtual ~TitleScene();

		virtual void update() override;
		virtual void draw() const override;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}

#endif // !BNSCUP_TITLESCENE_H_
