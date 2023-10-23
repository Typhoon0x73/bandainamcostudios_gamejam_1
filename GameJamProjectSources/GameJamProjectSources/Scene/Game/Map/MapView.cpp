#include "MapView.h"
#include "MapData.h"
#include "../../../Common/Common.h"
#include "RoomData.h"

namespace
{
	static const Grid<Point> ROOM_TEMPLATE_CHIP 
	{
		5, 5,
		{
			{ 0, 0 }, { 1, 0 }, { 2, 0 }, { 4, 0 }, { 5, 0 },
			{ 0, 1 }, { 1, 1 }, { 2, 1 }, { 4, 1 }, { 5, 1 },
			{ 0, 2 }, { 1, 2 }, { 2, 2 }, { 4, 2 }, { 5, 2 },
			{ 0, 3 }, { 1, 3 }, { 2, 3 }, { 4, 3 }, { 5, 3 },
			{ 0, 4 }, { 1, 4 }, { 2, 4 }, { 4, 4 }, { 5, 4 },
		}
	};
}

namespace bnscup
{
	MapView::MapView(MapData* pMapData)
		: m_tileSet{}
		, m_pMapData{ pMapData }
	{
		createDisp();
	}

	MapView::~MapView()
	{
	}

	void MapView::draw() const
	{
		if (m_pMapData == nullptr)
		{
			return;
		}

		const auto& rooms = m_pMapData->getRooms();
		const auto& mapSize = m_pMapData->getMapSize();
		const auto& chipSize = m_pMapData->getChipSize();
		for (int32 y : step(mapSize.y))
		{
			for (int32 x : step(mapSize.x))
			{
				const int32 roomIndex = x * mapSize.x + y;
				const auto& room = rooms[roomIndex];
				if (room.isEmpty())
				{
					continue;
				}
				//5x5
				for(int32 ry : step(5))
				{
					for (int32 rx : step(5))
					{
						const auto& chip = ROOM_TEMPLATE_CHIP[Point(rx, ry)];
						RectF srcRect{ chip.x * chipSize, chip.y * chipSize, chipSize, chipSize };
						m_tileSet(srcRect).draw((x * 5 + rx) * chipSize, (y * 5 + ry) * chipSize);
					}
				}
			}
		}

	}

	void MapView::createDisp()
	{
		if (m_pMapData == nullptr)
		{
			DEBUG_BREAK(true);
			return;
		}

		// タイルセットの設定
		{
			m_tileSet = TextureAsset(m_pMapData->getTilesetTextureName());
		}

	}


}
