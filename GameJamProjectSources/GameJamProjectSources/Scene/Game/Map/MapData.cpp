#include "MapData.h"
#include "../../../Common/Common.h"
#include "RoomData.h"

namespace bnscup
{

	MapData::MapData(const Array<RoomData>& rooms, AssetNameView name, int32 mapSizeW, int32 mapSizeH, int32 chipSize)
		: m_tilesetTexture{ name }
		, m_rooms{ rooms }
		, m_mapSize{ mapSizeW, mapSizeH }
		, m_chipSize{ chipSize }
	{
	}

	MapData::~MapData()
	{
	}

	void MapData::setTilesetTextureName(AssetNameView name)
	{
		m_tilesetTexture = name;
	}

	AssetNameView MapData::getTilesetTextureName() const
	{
		return m_tilesetTexture;
	}

	const Array<RoomData>& MapData::getRooms() const
	{
		return m_rooms;
	}

	const Size& MapData::getMapSize() const
	{
		return m_mapSize;
	}

	int32 MapData::getChipSize() const
	{
		return m_chipSize;
	}

}
