#include "GameScene.h"

namespace bnscup
{
	class GameScene::Impl
	{
	public:

		Impl();
		~Impl();

	};

	//==================================================

	GameScene::GameScene(const super::InitData& init)
		: super{ init }
		, m_pImpl{ nullptr }
	{
		m_pImpl.reset(new Impl());
	}

	GameScene::~GameScene()
	{
	}

	void GameScene::update()
	{
	}

	void GameScene::draw() const
	{
	}


}
