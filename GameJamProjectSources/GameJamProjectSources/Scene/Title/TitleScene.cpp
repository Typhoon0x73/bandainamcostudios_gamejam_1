#include "TitleScene.h"
#include "TitleView.h"

namespace bnscup
{
	class TitleScene::Impl
	{
		enum class Step
		{
			CreateDisp,
			Idle,
		};

	public:

		Impl();
		~Impl();

		void update();
		void draw() const;

		SceneKey getNextScene() const;
		bool isEnd() const;

	private:

		Step m_step;
		SceneKey m_nextSceneKey;
		bool m_isEnd;

		TitleView m_titleView;
	};

	//==================================================

	TitleScene::Impl::Impl()
		: m_step{ Step::CreateDisp }
		, m_nextSceneKey{ SceneKey::Title }
		, m_isEnd{ false }
		, m_titleView{}
	{
	}

	TitleScene::Impl::~Impl()
	{
	}

	void TitleScene::Impl::update()
	{
		m_titleView.update();

		// ステージ選択画面へ
		if (m_titleView.isStageSelectButtonSelected())
		{
			m_nextSceneKey = SceneKey::StageSelect;
			m_isEnd = true;
			return;
		}

		// ゲーム終了
		if (m_titleView.isExitButtonSelected())
		{
			m_nextSceneKey = SceneKey::Exit;
			m_isEnd = true;
			return;
		}
	}

	void TitleScene::Impl::draw() const
	{
		m_titleView.draw();
	}

	SceneKey TitleScene::Impl::getNextScene() const
	{
		return m_nextSceneKey;
	}

	bool TitleScene::Impl::isEnd() const
	{
		return m_isEnd;
	}

	//==================================================

	TitleScene::TitleScene(const super::InitData& init)
		: super{ init }
		, m_pImpl{ nullptr }
	{
		m_pImpl.reset(new Impl());
	}

	TitleScene::~TitleScene()
	{
	}

	void TitleScene::update()
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

	void TitleScene::draw() const
	{
		if (m_pImpl)
		{
			m_pImpl->draw();
		}
	}
}
