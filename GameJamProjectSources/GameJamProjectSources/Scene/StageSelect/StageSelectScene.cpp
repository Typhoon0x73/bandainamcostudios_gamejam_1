#include "StageSelectScene.h"
#include "../../Common/Common.h"
#include "StageSelectView.h"

namespace bnscup
{
	class StageSelectScene::Impl
	{
	public:

		Impl();
		~Impl();

		void update();
		void draw() const;

		SceneKey getNextScene() const;
		bool isEnd() const;

	private:

		SceneKey m_nextScene;
		bool m_isEnd;

		StageSelectView m_stageSelectView;
	};

	//==================================================
	
	StageSelectScene::Impl::Impl()
		: m_nextScene{ SceneKey::StageSelect }
		, m_isEnd{ false }
		, m_stageSelectView{}
	{
	}

	StageSelectScene::Impl::~Impl()
	{
	}

	void StageSelectScene::Impl::update()
	{
		m_stageSelectView.update();

		// ゲーム開始ボタン
		if (m_stageSelectView.isPlayButtonSelected())
		{
			m_nextScene = SceneKey::Game;
			m_isEnd = true;
			return;
		}
		// タイトルへ戻る
		if (m_stageSelectView.isReturnTitleSelected())
		{
			m_nextScene = SceneKey::Title;
			m_isEnd = true;
			return;
		}
	}

	void StageSelectScene::Impl::draw() const
	{
		m_stageSelectView.draw();
	}

	SceneKey StageSelectScene::Impl::getNextScene() const
	{
		return m_nextScene;
	}

	bool StageSelectScene::Impl::isEnd() const
	{
		return m_isEnd;
	}
	 
	//==================================================

	StageSelectScene::StageSelectScene(const super::InitData& init)
		: super{ init }
		, m_pImpl{ nullptr }
	{
		m_pImpl.reset(new Impl());
	}

	StageSelectScene::~StageSelectScene()
	{
	}

	void StageSelectScene::update()
	{
		if (m_pImpl)
		{
			m_pImpl->update();
			if (m_pImpl->isEnd())
			{
				auto& sceneData = getData();
				sceneData.nextScene = m_pImpl->getNextScene();
				changeScene(SceneKey::Load, 1.0s);
			}
		}
	}

	void StageSelectScene::draw() const
	{
		if (m_pImpl)
		{
			m_pImpl->draw();
		}
	}
}
