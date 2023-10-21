#include "LoadScene.h"
#include "../../Common/Common.h"
#include "../../AssetRegister/AssetRegister.h"

namespace bnscup
{
	namespace
	{
		static const HashTable<SceneKey, const Array<FilePath>> TABLE =
		{
			{
				SceneKey::Title,
				{
					U"",
				}
			},
		};
	}

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

		Impl(AssetRegister* pAssetRegister, SceneKey nextScene);
		~Impl();

		void update();
		void draw() const;

		bool isEnd() const;

	private:

		Step m_step;
		AssetRegister* m_pAssetRegister;
		SceneKey m_nextScene;
	};

	//==================================================

	LoadScene::Impl::Impl(AssetRegister* pAssetRegister, SceneKey nextScene)
		: m_step{ Step::RegistAsync }
		, m_pAssetRegister{ pAssetRegister }
		, m_nextScene{ nextScene }
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
			// 先に登録済みを破棄
			m_pAssetRegister->unregist();
			m_pAssetRegister->reset();

			// テーブルからパックを登録
			const auto& assets = TABLE.at(m_nextScene);
			for (const auto& asset : assets)
			{
				m_pAssetRegister->addRegistPackFile(asset);
			}
			m_pAssetRegister->asyncRegist();
			m_step = Step::RegistWait;
			[[fallthrough]];
		}
		case Step::RegistWait:
		{
			// 各パックを読み込むまで待機
			if (not(m_pAssetRegister->isReady()))
			{
				return;
			}
			m_step = Step::LoadAsync;
			[[fallthrough]];
		}
		case Step::LoadAsync:
		{
			// 各アセットごとに読み込み
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
			// 各アセット読み込み待ち
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
			// ここには来ないはず
			DEBUG_BREAK(true);
			return;
		}
	}

	void LoadScene::Impl::draw() const
	{
		// 黒背景
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
		m_pImpl.reset(new Impl(getData().pAssetRegister, getData().nextScene));
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
