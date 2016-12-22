#pragma once


#include "Game/Entity2D.hpp"


//-----------------------------------------------------------------------------
class Bullet : public Entity2D
{
public:
	Bullet( const Vector2& position, const float m_orientation );
	~Bullet();
	float GetTimeToLive() const { return m_timeToLive; }
	void Update( float deltaSeconds ) override;
private:
	Bullet();
	float m_timeToLive;
};