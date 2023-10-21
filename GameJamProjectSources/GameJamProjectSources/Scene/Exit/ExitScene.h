#pragma once
#ifndef BNSCUP_EXITSCENE_H_
#define BNSCUP_EXITSCENE_H_

#include "../SceneDefine.h"

namespace bnscup
{
	class ExitScene : public GameApp::Scene
	{
		using super = GameApp::Scene;
	public:

		explicit ExitScene(const super::InitData& init);
		virtual ~ExitScene();

		virtual void update() override;
		virtual void draw() const override;
	};
}

#endif // !BNSCUP_EXITSCENE_H_
