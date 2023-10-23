#pragma once
#ifndef BNSCUP_MAPVIEW_H_
#define BNSCUP_MAPVIEW_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class MapData;
	class MapView
	{
	public:

		explicit MapView(MapData* pMapData);
		virtual ~MapView();

		void draw() const;

	private:

		void createDisp();

	private:

		Texture m_tileSet;
		MapData* m_pMapData;
	};
}

#endif // !BNSCUP_MAPVIEW_H_
