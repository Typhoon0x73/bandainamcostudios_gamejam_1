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

		RectF noneSrcRect{ 8 * chipSize, 7 * chipSize, chipSize, chipSize };
		m_tileSet(noneSrcRect).fitted(Scene::Size() * 2).draw(-Scene::Size());

		for (int32 y : step(mapSize.y))
		{
			for (int32 x : step(mapSize.x))
			{
				const int32 roomIndex = x + y * mapSize.x;
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
				// 通路の表示
				if (room.canPassable(RoomData::Route::Up))
				{
					RectF srcRect0{ 9 * chipSize, 7 * chipSize, chipSize, chipSize };
					RectF srcRect1{ 7 * chipSize, 0 * chipSize, chipSize, chipSize };
					m_tileSet(srcRect0).draw((x * 5 + 2) * chipSize, (y * 5 + 0) * chipSize);
					m_tileSet(srcRect1).draw((x * 5 + 2) * chipSize, (y * 5 + 1) * chipSize);
				}
				if (room.canPassable(RoomData::Route::Down))
				{
					RectF srcRect0{ 9 * chipSize, 7 * chipSize, chipSize, chipSize };
					RectF srcRect1{ 7 * chipSize, 2 * chipSize, chipSize, chipSize };
					RectF srcRect2{ 5 * chipSize, 5 * chipSize, chipSize, chipSize };
					RectF srcRect3{ 4 * chipSize, 5 * chipSize, chipSize, chipSize };
					m_tileSet(srcRect0).draw((x * 5 + 2) * chipSize, (y * 5 + 4) * chipSize);
					m_tileSet(srcRect1).draw((x * 5 + 2) * chipSize, (y * 5 + 3) * chipSize);
					m_tileSet(srcRect2).draw((x * 5 + 1) * chipSize, (y * 5 + 4) * chipSize);
					m_tileSet(srcRect3).draw((x * 5 + 3) * chipSize, (y * 5 + 4) * chipSize);
				}
				if (room.canPassable(RoomData::Route::Left))
				{
					RectF srcRect0{ 9 * chipSize, 7 * chipSize, chipSize, chipSize };
					RectF srcRect1{ 6 * chipSize, 1 * chipSize, chipSize, chipSize };
					RectF srcRect2{ 1 * chipSize, 0 * chipSize, chipSize, chipSize };
					RectF srcRect3{ 3 * chipSize, 5 * chipSize, chipSize, chipSize };
					m_tileSet(srcRect0).draw((x * 5 + 0) * chipSize, (y * 5 + 2) * chipSize);
					m_tileSet(srcRect1).draw((x * 5 + 1) * chipSize, (y * 5 + 2) * chipSize);
					m_tileSet(srcRect2).draw((x * 5 + 0) * chipSize, (y * 5 + 1) * chipSize);
					m_tileSet(srcRect3).draw((x * 5 + 0) * chipSize, (y * 5 + 3) * chipSize);
				}
				if (room.canPassable(RoomData::Route::Right))
				{
					RectF srcRect0{ 9 * chipSize, 7 * chipSize, chipSize, chipSize };
					RectF srcRect1{ 8 * chipSize, 1 * chipSize, chipSize, chipSize };
					RectF srcRect2{ 4 * chipSize, 0 * chipSize, chipSize, chipSize };
					RectF srcRect3{ 0 * chipSize, 5 * chipSize, chipSize, chipSize };
					m_tileSet(srcRect0).draw((x * 5 + 4) * chipSize, (y * 5 + 2) * chipSize);
					m_tileSet(srcRect1).draw((x * 5 + 3) * chipSize, (y * 5 + 2) * chipSize);
					m_tileSet(srcRect2).draw((x * 5 + 4) * chipSize, (y * 5 + 1) * chipSize);
					m_tileSet(srcRect3).draw((x * 5 + 4) * chipSize, (y * 5 + 3) * chipSize);
				}
				// 扉の表示
				if (room.isLocked(RoomData::Route::Up))
				{
					RectF srcRect{ 7 * chipSize, 3 * chipSize, chipSize, chipSize };
					m_tileSet(srcRect).draw((x * 5 + 2) * chipSize, (y * 5 + 0) * chipSize);
				}
				if (room.isLocked(RoomData::Route::Down))
				{
					RectF srcRect{ 7 * chipSize, 3 * chipSize, chipSize, chipSize };
					m_tileSet(srcRect).draw((x * 5 + 2) * chipSize, (y * 5 + 4) * chipSize);
				}
				if (room.isLocked(RoomData::Route::Left))
				{
					RectF srcRect{ 7 * chipSize, 4 * chipSize, chipSize, chipSize };
					m_tileSet(srcRect).draw((x * 5 + 0) * chipSize, (y * 5 + 2) * chipSize);
				}
				if (room.isLocked(RoomData::Route::Right))
				{
					RectF srcRect{ 8 * chipSize, 5 * chipSize, chipSize, chipSize };
					m_tileSet(srcRect).draw((x * 5 + 4) * chipSize, (y * 5 + 2) * chipSize);
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
