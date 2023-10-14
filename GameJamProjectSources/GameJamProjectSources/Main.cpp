# include <Siv3D.hpp> // Siv3D v0.6.12

# ifdef _DEBUG
# include "DebugPlayer/DebugPlayer.h"

namespace
{
	bncup::DebugPlayer g_debugPlayer;
}
# endif //_DEBUG

# include "Scene/SceneDefine.h"

void Main()
{

	bncup::GameApp gameApp;

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
