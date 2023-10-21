#pragma once
#ifndef BNSCUP_MAPDATA_H_
#define BNSCUP_MAPDATA_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class MapData
	{
	public:

		enum class Layer : uint32
		{
			Grounds = 0,
			Wall,
			Ornament,
		};
		static const uint32 LAYER_MAX = 3;

		explicit MapData();
		virtual ~MapData();

		void loadAsync(FilePath path);
		bool isReady() const;

		const std::array<Grid<int32>, LAYER_MAX>& getLayers() const;
		const Grid<int32>& getGrid(Layer layer) const;
		int32 getCell(Layer layer, uint32 x, uint32 y) const;

	private:

		Optional<AsyncTask<bool>> m_asyncTask;
		std::array<Grid<int32>, LAYER_MAX> m_layers;
	};
}

#endif // !BNSCUP_MAPDATA_H_
