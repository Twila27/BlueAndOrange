#pragma once

#include <string>

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/AABB2.hpp"



//-----------------------------------------------------------------------------
class Texture;
class string;
struct Rgba;


//-----------------------------------------------------------------------------
class Entity2D
{
public:
	Entity2D( const std::string& imageFilePath, const Rgba* color );
	virtual ~Entity2D();

	virtual void Update( float deltaSeconds );// = 0; //For mock exam.
	virtual void Render();

	bool DoesOverlap( const Entity2D& ent ) const;
	void DecreaseHealth( int numToLowerBy = 1 );
	void SetHealth( int newVal ) { m_health = newVal; }
	Vector2 GetPosition() const { return m_position; }
	Vector2 GetLocalTipPosition() const;
	Vector2 GetVelocity() const { return m_velocity; }
	Vector2 GetDirection() const;
	float GetOrientation() const { return m_orientation; }
	float GetPhysicsRadius() const { return m_physicsRadius; }
	float GetAgeAliveInSeconds() const { return m_ageAliveInSeconds; }
	void SetAgeDeadInSeconds( float newVal ) { m_ageDeadInSeconds = newVal; }
	float GetAgeDeadInSeconds() const { return m_ageDeadInSeconds; }
	void SetPosition( float newX, float newY );
	void SetPosition( Vector2 newPos ) { m_position = newPos; }
	void SetPositionX( float newX ) { m_position.x = newX; }
	void SetPositionY( float newY ) { m_position.y = newY; }
	void SetVelocity( float newX, float newY );
	void SetVelocity( Vector2 newVel ) { m_velocity = newVel; }
	void SetOrientation( float newOrientation );
	void SetAngularVelocity( float newAngularVelocity );
	virtual void Die() { SetIsAlive( false ); }
	void SetIsAlive( bool newStatus );
	virtual bool IsAlive() const { return m_isAlive; }


public:
	AABB2 m_renderBounds;
	Texture* m_diffuseTexture;
	const Rgba* m_color;

	Vector2 m_position;
	Vector2 m_velocity;
	float m_angularVelocity;
	float m_orientation;
	Vector2 m_acceleration;

	float m_physicsRadius;
	float m_cosmeticRadius;

	float m_ageAliveInSeconds;
	float m_ageDeadInSeconds;
	int m_health;
	bool m_isAlive;

	static const float DEFAULT_ENTITY_RADIUS_SIZE;
	static const float DEFAULT_ENTITY_NUM_SIDES;
};