# include <Siv3D.hpp> // Siv3D v0.6.12

# ifdef _DEBUG
# include "DebugPlayer/DebugPlayer.h"

namespace
{
	bncup::DebugPlayer g_debugPlayer;
}
# endif //_DEBUG

# include "Scene/SceneDefine.h"
# include "Scene/Load/LoadScene.h"
# include "AssetRegister/AssetRegister.h"

void Main()
{
	std::unique_ptr<bncup::AssetRegister> pAssetRegister;
	{
		pAssetRegister.reset(new bncup::AssetRegister());
	}

	std::shared_ptr<bncup::SceneData> pSceneData;
	{
		pSceneData.reset(new bncup::SceneData());
		pSceneData->pAssetRegister = pAssetRegister.get();
		pSceneData->nextScene = bncup::SceneKey::Load;
	}

	bncup::GameApp gameApp{ pSceneData };
	gameApp
		.add<bncup::LoadScene>(bncup::SceneKey::Load)
		.init(bncup::SceneKey::Load);

	while (System::Update())
	{
#ifdef _DEBUG

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
