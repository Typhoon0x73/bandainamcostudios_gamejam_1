#pragma once
#ifndef BNCUP_SCENE_DEFINE_H_
#define BNCUP_SCENE_DEFINE_H_

#include <Siv3D.hpp>

namespace bncup
{
	enum class SceneKey
	{
		Load,
	};

	class AssetRegister;
	struct SceneData
	{
		SceneKey nextScene;
		AssetRegister* pAssetRegister;
	};

	using GameApp = SceneManager<SceneKey, SceneData>;
}

#endif // !BNCUP_SCENE_DEFINE_H_
