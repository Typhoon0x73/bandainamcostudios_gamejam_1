#include "StageSelectScene.h"

namespace bnscup
{
	class StageSelectScene::Impl
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

	private:

		Step m_step;
	};

	//==================================================
	
	StageSelectScene::Impl::Impl()
		: m_step{ Step::CreateDisp }
	{
	}

	StageSelectScene::Impl::~Impl()
	{
	}

	void StageSelectScene::Impl::update()
	{
	}

	void StageSelectScene::Impl::draw() const
	{
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
