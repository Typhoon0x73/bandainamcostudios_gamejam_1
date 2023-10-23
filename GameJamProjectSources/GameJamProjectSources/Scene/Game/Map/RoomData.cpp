#include "RoomData.h"

namespace bnscup
{
	RoomData::RoomData(uint8 route, uint8 lock)
		: m_route{ route }
		, m_routeLock{ lock }
	{
	}

	RoomData::~RoomData()
	{
	}

	void RoomData::unlock(Route route)
	{
		m_routeLock &= ~FromEnum(route);
	}

	bool RoomData::canPassable(Route route) const
	{
		return m_route & FromEnum(route);
	}

	bool RoomData::isLocked(Route route) const
	{
		return m_routeLock & FromEnum(route);
	}

	bool RoomData::isEmpty() const
	{
		return m_route == FromEnum(Route::None);
	}

}

