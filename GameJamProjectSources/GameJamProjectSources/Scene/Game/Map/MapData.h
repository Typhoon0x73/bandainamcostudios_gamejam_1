#pragma once
#ifndef BNSCUP_MAPDATA_H_
#define BNSCUP_MAPDATA_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class RoomData;
	class MapData
	{
	public:

		explicit MapData(const Array<RoomData>& rooms, AssetNameView name, int32 mapSizeW, int32 mapSizeH, int32 chipSize);
		virtual ~MapData();

		void setTilesetTextureName(AssetNameView name);
		AssetNameView getTilesetTextureName() const;

		const Array<RoomData>& getRooms() const;
		const Size& getMapSize() const;
		int32 getChipSize() const;

	private:

		AssetName m_tilesetTexture;
		Array<RoomData> m_rooms;
		Size m_mapSize;
		int32 m_chipSize;
	};
}

#endif // !BNSCUP_MAPDATA_H_
