#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Game/Bullet.hpp"

#include <math.h>

const float SECONDS_BEFORE_DISAPPEARING = 1.f;
const float BULLET_SIZE_KNOB = 5.f;
const float BULLET_SPEED_KNOB = 400.f;

//-----------------------------------------------------------------------------
Bullet::Bullet()
	: Entity2D( "Data/Images/Disc.png", new Rgba( .4f, .6f, 1.f ) )
{
	m_timeToLive = SECONDS_BEFORE_DISAPPEARING;
	m_cosmeticRadius = BULLET_SIZE_KNOB;
	m_physicsRadius = m_cosmeticRadius;
	m_health = 1;
}


//-----------------------------------------------------------------------------
Bullet::~Bullet()
{
	delete m_color;
}


//-----------------------------------------------------------------------------
Bullet::Bullet( const Vector2& position, const float m_orientation )
: Bullet()
{
	m_position = position;
	m_velocity.x = BULLET_SPEED_KNOB * cos( m_orientation );
	m_velocity.y = BULLET_SPEED_KNOB * sin( m_orientation );
}


//-----------------------------------------------------------------------------
// void Bullet::Render()
// {
//	if ( !m_isAlive ) return;

// 	Vector2 startPos = Vector2( m_cosmeticRadius + m_position.x, m_cosmeticRadius + m_position.y );
// 	Vector2 endPos = Vector2( startPos.x + BULLET_SIZE_KNOB, startPos.y + BULLET_SIZE_KNOB );
// 	g_theRenderer->DrawLine( startPos, endPos, Rgba(), Rgba() );


// }
void Bullet::Update( float deltaSeconds )
{
	Entity2D::Update( deltaSeconds );

	m_timeToLive -= deltaSeconds;
	if ( m_timeToLive <= 0.f ) m_isAlive = false;
}