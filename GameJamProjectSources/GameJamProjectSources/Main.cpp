# include <Siv3D.hpp> // Siv3D v0.6.12

# ifdef _DEBUG
# include "DebugPlayer/DebugPlayer.h"

namespace
{
	bnscup::DebugPlayer g_debugPlayer;
}
# endif //_DEBUG

# include "Scene/SceneDefine.h"
# include "Scene/Load/LoadScene.h"
# include "AssetRegister/AssetRegister.h"

void Main()
{
	std::unique_ptr<bnscup::AssetRegister> pAssetRegister;
	{
		pAssetRegister.reset(new bnscup::AssetRegister());
	}

	std::shared_ptr<bnscup::SceneData> pSceneData;
	{
		pSceneData.reset(new bnscup::SceneData());
		pSceneData->pAssetRegister = pAssetRegister.get();
		pSceneData->nextScene = bnscup::SceneKey::Load;
	}

	bnscup::GameApp gameApp{ pSceneData };
	gameApp
		.add<bnscup::LoadScene>(bnscup::SceneKey::Load)
		.init(bnscup::SceneKey::Load);

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
