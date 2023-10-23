#pragma once
#ifndef BNSCUP_ROOMDATA_H_
#define BNSCUP_ROOMDATA_H_

#include <Siv3D.hpp>

namespace bnscup
{
	class RoomData
	{
	public:

		enum class Route : uint8
		{
			None  = 0,
			Up    = 1 << 0,
			Right = 1 << 1,
			Down  = 1 << 2,
			Left  = 1 << 3,

			All = Up | Right | Down | Left,
			UpRight = Up | Right,
			UpDown  = Up | Down,
			UpLeft  = Up | Left,
			RightDown = Right | Down,
			RightLeft = Right | Left,
			DownLeft  = Down | Left,
			UpRightDown = Up | Right | Down,
			UpRightLeft = Up | Right | Left,
			UpDownLeft  = Up | Down  | Left,
			RightDownLeft = Right | Down | Left,
		};

	public:

		explicit RoomData(uint8 route, uint8 lock);
		virtual ~RoomData();

		void unlock(Route route);
		bool canPassable(Route route) const;
		bool isLocked(Route route) const;
		bool isEmpty() const;

	private:

		uint8 m_route;
		uint8 m_routeLock;
	};
}

#endif // !BNSCUP_ROOMDATA_H_
