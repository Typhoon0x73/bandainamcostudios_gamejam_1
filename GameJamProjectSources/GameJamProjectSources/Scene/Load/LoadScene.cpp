#include "LoadScene.h"
#include "../../Common/Common.h"
#include "../../AssetRegister/AssetRegister.h"

namespace bncup
{
	class LoadScene::Impl
	{
		enum class Step
		{
			RegistAsync,
			RegistWait,
			LoadAsync,
			LoadWait,
			End,
		};

	public:

		Impl(AssetRegister* pAssetRegister);
		~Impl();

		void update();
		void draw() const;

		bool isEnd() const;

	private:

		Step m_step;
		AssetRegister* m_pAssetRegister;
	};

	//==================================================

	LoadScene::Impl::Impl(AssetRegister* pAssetRegister)
		: m_step{ Step::RegistAsync }
		, m_pAssetRegister{ pAssetRegister }
	{
	}

	LoadScene::Impl::~Impl()
	{
	}

	void LoadScene::Impl::update()
	{
		if (m_pAssetRegister == nullptr)
		{
			DEBUG_BREAK(true);
			m_step = Step::End;
			return;
		}

		switch (m_step)
		{
		case Step::RegistAsync:
		{
			m_pAssetRegister->asyncRegist();
			m_step = Step::RegistWait;
			[[fallthrough]];
		}
		case Step::RegistWait:
		{
			if (not(m_pAssetRegister->isReady()))
			{
				return;
			}
			m_step = Step::LoadAsync;
			[[fallthrough]];
		}
		case Step::LoadAsync:
		{
			for (const auto& packInfo : m_pAssetRegister->getPackInfos())
			{
				for (const auto& assetName : packInfo.audioAssetNames)
				{
					AudioAsset::LoadAsync(assetName);
				}
				for (const auto& assetName : packInfo.fontAssetNames)
				{
					FontAsset::LoadAsync(assetName);
				}
				for (const auto& assetName : packInfo.textureAssetNames)
				{
					TextureAsset::LoadAsync(assetName);
				}
			}
			m_step = Step::LoadWait;
			[[fallthrough]];
		}
		case Step::LoadWait:
		{
			for (const auto& packInfo : m_pAssetRegister->getPackInfos())
			{
				for (const auto& assetName : packInfo.audioAssetNames)
				{
					if (not(AudioAsset::IsReady(assetName)))
					{
						return;
					}
				}
				for (const auto& assetName : packInfo.fontAssetNames)
				{
					if (not(FontAsset::IsReady(assetName)))
					{
						return;
					}
				}
				for (const auto& assetName : packInfo.textureAssetNames)
				{
					if (not(TextureAsset::IsReady(assetName)))
					{
						return;
					}
				}
			}
			m_step = Step::End;
			[[fallthrough]];
		}
		case Step::End:
			return;
		default:
			DEBUG_BREAK(true);
			return;
		}
	}

	void LoadScene::Impl::draw() const
	{
		RectF{ 0, 0, Scene::Width(), Scene::Height() }.draw(Palette::Black);
	}

	bool LoadScene::Impl::isEnd() const
	{
		return (m_step == Step::End);
	}

	//==================================================

	LoadScene::LoadScene(const super::InitData& init)
		: super{ init }
		, m_pImpl{ nullptr }
	{
		m_pImpl.reset(new Impl(getData().pAssetRegister));
	}

	LoadScene::~LoadScene()
	{
	}

	void LoadScene::update()
	{
		if (m_pImpl)
		{
			m_pImpl->update();
			if (m_pImpl->isEnd())
			{
				if (getData().nextScene == SceneKey::Load)
				{
					return;
				}
				changeScene(getData().nextScene, 1.0s);
			}
		}
	}

	void LoadScene::draw() const
	{
		if (m_pImpl)
		{
			m_pImpl->draw();
		}
	}
}
