#include "Game/Entity2D.hpp"


#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Error/ErrorWarningAssert.hpp"
#include "Game/TheApp.hpp"
#include "Game/GameCommon.hpp"


//--------------------------------------------------------------------------------------------------------------
STATIC const float Entity2D::DEFAULT_ENTITY_RADIUS_SIZE = 20.f; //Note this gets shown relative to SetOrtho call.
STATIC const float Entity2D::DEFAULT_ENTITY_NUM_SIDES = 20.f; //Note this gets shown relative to SetOrtho call.


//--------------------------------------------------------------------------------------------------------------
Entity2D::Entity2D( const std::string& imageFilePath, const Rgba* color )
	: m_position( Vector2( 0.f, 0.f ) )
	, m_velocity( Vector2( 0.f, 0.f ) )
	, m_orientation( 0.f ) //Treated as degrees by other functions.
	, m_angularVelocity( 0.f )
	, m_acceleration( Vector2( 0.f, 0.f ) )
	, m_cosmeticRadius( DEFAULT_ENTITY_RADIUS_SIZE )
	, m_physicsRadius( DEFAULT_ENTITY_RADIUS_SIZE )
	, m_color( color )
	, m_isAlive( true )
	, m_ageAliveInSeconds( 0.f )
	, m_ageDeadInSeconds( 0.f )
	, m_health( 0 )
	, m_renderBounds(
	Vector2( 0.f, 0.f ),
	Vector2( 0.f, 0.f )
	)
{
	Vector2 mins = Vector2( m_position.x - m_cosmeticRadius, m_position.y - m_cosmeticRadius );
	Vector2 maxs = Vector2( m_position.x + m_cosmeticRadius, m_position.y + m_cosmeticRadius );
	m_renderBounds = AABB2( mins, maxs );
	if (imageFilePath != "") m_diffuseTexture = Texture::CreateOrGetTexture( imageFilePath );
}

Entity2D::~Entity2D()
{
	//Note texture deletion handled later on, as per Squirrel saying so in class.
}

//--------------------------------------------------------------------------------------------------------------
void Entity2D::Update( float deltaSeconds )
{
	if ( m_isAlive ) m_ageAliveInSeconds += deltaSeconds;
	else m_ageDeadInSeconds += deltaSeconds;

	m_position += m_velocity * deltaSeconds;

	m_renderBounds.mins = Vector2( m_position.x - m_cosmeticRadius, m_position.y - m_cosmeticRadius );
	m_renderBounds.maxs = Vector2( m_position.x + m_cosmeticRadius, m_position.y + m_cosmeticRadius );
	m_orientation += m_angularVelocity * deltaSeconds;
}


//--------------------------------------------------------------------------------------------------------------
void Entity2D::Render()
{
	if ( !m_isAlive ) return;

	if ( m_diffuseTexture != nullptr ) g_theRenderer->DrawTexturedAABB( m_renderBounds, *m_diffuseTexture, AABB2(0.f, 0.f, 1.f, 1.f), *m_color );
	else g_theRenderer->DrawPolygon( m_position, m_cosmeticRadius, DEFAULT_ENTITY_NUM_SIDES, 0.f );
// 
// 	g_theRenderer->PushView();
// 
// 	//Change of Basis (detour) Theorem's triple product, from camera (world-to-cam happens before this is called!) to model space.
// 	g_theRenderer->TranslateView( m_position * 1.f );
// 	g_theRenderer->RotateViewByDegrees( m_orientation );
// 	g_theRenderer->TranslateView( m_position * -1.f );
// 
// 	g_theRenderer->DrawTexturedAABB( m_renderBounds, *m_diffuseTexture );
// 
// 	g_theRenderer->PopView();
}


//--------------------------------------------------------------------------------------------------------------
bool Entity2D::DoesOverlap( const Entity2D& ent ) const
{
	return DoDiscsOverlap( m_position, m_physicsRadius, ent.m_position, ent.m_physicsRadius );
}


//--------------------------------------------------------------------------------------------------------------
void Entity2D::DecreaseHealth( int numToLowerBy /*=1*/ )
{
	ASSERT_OR_DIE( numToLowerBy > 0, "DecreaseHealth Does Not Heal!" );


	m_health -= numToLowerBy;
	if ( m_health <= 0 ) Die();
}

//--------------------------------------------------------------------------------------------------------------
Vector2 Entity2D::GetLocalTipPosition() const
{
	return GetDirection() * m_cosmeticRadius;
}


//--------------------------------------------------------------------------------------------------------------
Vector2 Entity2D::GetDirection() const
{
	Vector2 directionVector = Vector2( CosDegrees( m_orientation ), SinDegrees( m_orientation ) );

	directionVector.Normalize();

	return directionVector;
}


//--------------------------------------------------------------------------------------------------------------
void Entity2D::SetPosition( float newX, float newY )
{
	m_position.x = newX;
	m_position.y = newY;
}


//--------------------------------------------------------------------------------------------------------------
void Entity2D::SetVelocity( float newX, float newY )
{
	m_velocity.x = newX;
	m_velocity.y = newY;
}


//--------------------------------------------------------------------------------------------------------------
void Entity2D::SetOrientation( float newOrientation )
{
	m_orientation = newOrientation;
}


//--------------------------------------------------------------------------------------------------------------
void Entity2D::SetAngularVelocity( float newAngularVelocity )
{
	m_angularVelocity = newAngularVelocity;
}


//--------------------------------------------------------------------------------------------------------------
void Entity2D::SetIsAlive( bool newStatus )
{
	m_isAlive = newStatus;
	if ( newStatus == true ) m_ageDeadInSeconds = 0.f;
}