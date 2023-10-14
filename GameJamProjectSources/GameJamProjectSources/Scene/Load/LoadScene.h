#pragma once
#ifndef BNCUP_LOAD_SCENE_H_
#define BNCUP_LOAD_SCENE_H_

#include "../SceneDefine.h"

namespace bncup
{
	class LoadScene : public GameApp::Scene
	{
		using super = GameApp::Scene;
	public:

		explicit LoadScene(const super::InitData& init);
		virtual ~LoadScene();

		virtual void update() override;
		virtual void draw() const override;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}

#endif // !BNCUP_LOAD_SCENE_H_
