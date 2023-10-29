#include "Enemy.h"

namespace bnscup
{
	Enemy::Enemy()
		: m_moveType{ MoveType::UpDown }
		, m_moveDirection{ RoomData::Route::Up }
	{
	}

	Enemy::~Enemy()
	{
	}

	void Enemy::setMoveType(MoveType type)
	{
		m_moveType = type;
	}

	Enemy::MoveType Enemy::getMoveType() const
	{
		return m_moveType;
	}

	void Enemy::setMoveDirection(RoomData::Route dir)
	{
		m_moveDirection = dir;
	}

	RoomData::Route Enemy::getMoveDirection() const
	{
		return m_moveDirection;
	}

}
