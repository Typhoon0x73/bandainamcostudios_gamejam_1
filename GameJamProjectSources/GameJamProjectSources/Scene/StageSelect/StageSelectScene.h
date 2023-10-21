#pragma once
#ifndef BNSCUP_STAGESELECTSCENE_H_
#define BNSCUP_STAGESELECTSCENE_H_

#include "../SceneDefine.h"

namespace bnscup
{
	class StageSelectScene : public GameApp::Scene
	{
		using super = GameApp::Scene;
	public:

		explicit StageSelectScene(const super::InitData& init);
		virtual ~StageSelectScene();

		virtual void update() override;
		virtual void draw() const override;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}

#endif // !BNSCUP_STAGESELECTSCENE_H_
