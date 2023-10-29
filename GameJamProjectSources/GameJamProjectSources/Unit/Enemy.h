#pragma once
#ifndef BNSCUP_ENEMY_H_
#define BNSCUP_ENEMY_H_

#include "Unit.h"
#include "../Scene/Game/Map/RoomData.h"

namespace bnscup
{
	class Enemy : public Unit
	{
	public:

		enum class MoveType
		{
			UpDown,
			LeftRight,
		};

		explicit Enemy();
		virtual ~Enemy();

		void setMoveType(MoveType type);
		Enemy::MoveType getMoveType() const;
		void setMoveDirection(RoomData::Route dir);
		RoomData::Route getMoveDirection() const;

	private:

		MoveType m_moveType;
		RoomData::Route m_moveDirection;
	};
}

#endif // !BNSCUP_ENEMY_H_
