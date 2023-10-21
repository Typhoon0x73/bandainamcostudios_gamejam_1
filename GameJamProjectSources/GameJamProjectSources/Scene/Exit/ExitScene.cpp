#include "ExitScene.h"

namespace bnscup
{
	ExitScene::ExitScene(const super::InitData& init)
		: super{ init }
	{
	}

	ExitScene::~ExitScene()
	{
	}

	void ExitScene::update()
	{
		System::Exit();
	}

	void ExitScene::draw() const
	{
		// 黒背景
		RectF{ 0, 0, Scene::Width(), Scene::Height() }.draw(Palette::Black);
	}
}

