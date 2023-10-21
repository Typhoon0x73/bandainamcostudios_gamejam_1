#include "MapData.h"
#include "../../../Common/Common.h"

namespace
{
	bool LoadMapData(FilePath path, std::array<Grid<int32>, bnscup::MapData::LAYER_MAX>& out)
	{
		CSV csv;
		if (not(csv.load(path)))
		{
			DEBUG_BREAK(true);
			return false;
		}
		const auto& row0 = csv.getRow(0);
		const uint32 rowCount = ParseOr<uint32>(row0[0], 0);
		const uint32 columnCount = ParseOr<uint32>(row0[1], 0);

		for (auto& grid : out)
		{
			grid.resize(columnCount, rowCount);
		}

		for (uint32 layer : step(bnscup::MapData::LAYER_MAX))
		{
			auto& outGrid = out[layer];
			for (uint32 row = 0; row < rowCount; row++)
			{
				for (uint32 column : step(columnCount))
				{
					const auto& rowData = csv.getRow(row + 1 + layer * rowCount);
					if (rowData.size() <= column)
					{
						DEBUG_BREAK(true);
						return false;
					}
					const auto& cell = rowData[column];
					const int32 cellData = ParseOr<int32>(cell, 0);
					outGrid[Point(column, row)] = cellData;
				}
			}
		}
		return true;
	}
}

namespace bnscup
{

	MapData::MapData()
		: m_asyncTask{ none }
		, m_layers{}
	{
	}

	MapData::~MapData()
	{
	}

	void MapData::loadAsync(FilePath path)
	{
		m_asyncTask.emplace(Async(LoadMapData, path, std::ref(m_layers)));
	}

	bool MapData::isReady() const
	{
		if (not(m_asyncTask.has_value()))
		{
			return false;
		}
		return m_asyncTask->isReady();
	}

	const std::array<Grid<int32>, MapData::LAYER_MAX>& MapData::getLayers() const
	{
		return m_layers;
	}

	const Grid<int32>& MapData::getGrid(Layer layer) const
	{
		uint32 index = FromEnum(layer);
		DEBUG_BREAK(LAYER_MAX <= index);
		return m_layers[index];
	}

	int32 MapData::getCell(Layer layer, uint32 x, uint32 y) const
	{
		const auto& grid = getGrid(layer);
		if (x >= grid.width() or y >= grid.height())
		{
			DEBUG_BREAK(true);
			return -1;
		}
		return grid.at(y, x);
	}


}
