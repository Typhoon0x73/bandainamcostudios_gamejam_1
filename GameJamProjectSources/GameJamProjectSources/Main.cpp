# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Common/Common.h"

# ifdef _DEBUG
# include "DebugPlayer/DebugPlayer.h"

namespace
{
	bnscup::DebugPlayer g_debugPlayer;
}
# endif //_DEBUG

# include "Scene/SceneDefine.h"
# include "Scene/Load/LoadScene.h"
# include "Scene/Title/TitleScene.h"
# include "Scene/StageSelectScene/StageSelectScene.h"
# include "Scene/Exit/ExitScene.h"
# include "AssetRegister/AssetRegister.h"

namespace
{
	static const Array<FilePath> COMMON =
	{
		U"resource/com_button.json",
	};
	bnscup::AssetRegister g_commonRegister;
}

void Main()
{
	// ウィンドウの設定
	{
		Window::Resize(bnscup::WINDOW_SIZE_W, bnscup::WINDOW_SIZE_H);
		Window::SetTitle(U"ゲームタイトル（仮）");
	}

	{
		// コモンデータを登録、読み込み
		for (const auto& asset : COMMON)
		{
			g_commonRegister.addRegistPackFile(asset);
		}
		g_commonRegister.asyncRegist();
		while (not(g_commonRegister.isReady())) {};
		for (const auto& pack : g_commonRegister.getPackInfos())
		{
			for (const auto& info : pack.audioAssetNames)
			{
				AudioAsset::Load(info);
			}
			for (const auto& info : pack.fontAssetNames)
			{
				FontAsset::Load(info);
			}
			for (const auto& info : pack.textureAssetNames)
			{
				TextureAsset::Load(info);
			}
		}
	}

	// シーン用アセット登録インスタンス
	std::unique_ptr<bnscup::AssetRegister> pAssetRegister;
	{
		pAssetRegister.reset(new bnscup::AssetRegister());
	}

	// シーン共通データ
	std::shared_ptr<bnscup::SceneData> pSceneData;
	{
		pSceneData.reset(new bnscup::SceneData());
		pSceneData->pAssetRegister = pAssetRegister.get();
		pSceneData->nextScene = bnscup::SceneKey::Title;
	}

	// ゲームシーンの登録
	bnscup::GameApp gameApp{ pSceneData };
	gameApp
		.add<bnscup::LoadScene>(bnscup::SceneKey::Load)
		.add<bnscup::TitleScene>(bnscup::SceneKey::Title)
		.add<bnscup::StageSelectScene>(bnscup::SceneKey::StageSelect)
		.add<bnscup::ExitScene>(bnscup::SceneKey::Exit)
		.init(bnscup::SceneKey::Load);

	while (System::Update())
	{
#ifdef _DEBUG
		// デバッグ用再生制御
		g_debugPlayer.refresh();

		if (not(g_debugPlayer.isPause()))
		{
			if (not(gameApp.updateScene()))
			{
				// error
				return;
			}
		}
		gameApp.drawScene();

#else //_DEBUG

		if (not(gameApp.update()))
		{
			// error
			return;
		}

#endif //_DEBUG
	}
}
