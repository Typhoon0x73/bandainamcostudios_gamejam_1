#pragma once
#ifndef BNSCUP_GAMESCENE_H_
#define BNSCUP_GAMESCENE_H_

#include "../SceneDefine.h"

namespace bnscup
{
	class GameScene : public GameApp::Scene
	{
		using super = GameApp::Scene;
	public:

		explicit GameScene(const super::InitData& init);
		virtual ~GameScene();

		virtual void update() override;
		virtual void draw() const override;

	private:

		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}

#endif // !BNSCUP_GAMESCENE_H_
