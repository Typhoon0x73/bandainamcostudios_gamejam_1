#pragma once
#ifndef BNSCUP_SCENE_DEFINE_H_
#define BNSCUP_SCENE_DEFINE_H_

#include <Siv3D.hpp>

namespace bnscup
{
	enum class SceneKey
	{
		Load,
		Title,
		StageSelect,
	};

	class AssetRegister;
	struct SceneData
	{
		SceneKey nextScene;
		AssetRegister* pAssetRegister;
	};

	using GameApp = SceneManager<SceneKey, SceneData>;
}

#endif // !BNSCUP_SCENE_DEFINE_H_
