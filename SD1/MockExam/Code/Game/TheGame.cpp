#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Audio/TheAudio.hpp"

#include "Game/TheGame.hpp"
#include "Game/TheApp.hpp" //For input polling.
#include "Game/GameCommon.hpp"
//Be sure to #include all forward declared classes' .hpp's.
#include "Game/Entity2D.hpp"
#include "Game/Bullet.hpp"

#include <cstdlib>
#include <math.h>

//Can add virtual key macros here.
//#define VK_SPACE 0x20 //Etc. F1 on macro name to pull up full MSDN list.

//-----------------------------------------------------------------------------
TheGame* g_theGame = nullptr; 


//-----------------------------------------------------------------------------
//Add knob constants that aren't needed in .hpp, can still add there as extern.
//const float ASTEROID_SPEED_CHANGE_ON_BREAKUP_SCALING_KNOB = 1.25f; //Etc.
STATIC const float TheGame::PLAYER_SPEED_KNOB = 270.f;
STATIC const float TheGame::ENEMY_SPEED_KNOB = PLAYER_SPEED_KNOB * .25f;
STATIC const int TheGame::ENEMIES_PER_SPAWNING = 2;
STATIC const float TheGame::PLAYER_SHOTS_PER_SECOND = 10.f;
STATIC const float TheGame::PLAYER_SECONDS_PER_SHOT = 1.f / PLAYER_SHOTS_PER_SECOND;
STATIC const Rgba TheGame::PLAYER_COLOR = Rgba( .2f, .3f, .5f );
STATIC const Rgba TheGame::ENEMY_COLOR = Rgba( 1.f, .5f, .2f );
STATIC const float TheGame::SECONDS_BETWEEN_DEATH_AND_EXIT = 3.f;


//-----------------------------------------------------------------------------
TheGame::TheGame( )
	: m_shotCooldownTimerInSeconds( 0.f )
	, m_gameOverTimeInSeconds( 0.f )
	, m_redtoBlackTransitionFullLengthInSeconds( 2.f )
	, m_currentPointAlongTransition( 0.f )
	, m_shouldSpawnEnemy( false )
	, m_shouldExitGame( false )
	, m_shouldFlashRed( false )
{
	//Remember to initialize any pointers to initial values or to nullptr in here.
	// for ( int i = 0; i < INITIAL_NUMBER_OF_ASTEROIDS; i++ ) m_asteroids[ i ] = new Asteroid(...);
	// for ( int i = 0; i < MAX_NUMBER_OF_BULLETS; i++ ) m_bullets[ i ] = nullptr; //Etc.

	m_player = new Entity2D( "Data/Images/Disc.png", &PLAYER_COLOR );
	m_player->SetPosition( Vector2( 500.f, 500.f ) );
	m_player->SetHealth( 3 );

	SpawnEnemies( 1 );

	//Position player representation if camera isn't translated to player (i.e. if camera is fixed in world).
	//m_ship = new Ship( static_cast<float>( screenWidth ) / 2.f, static_cast<float>( screenHeight ) / 2.f ); 
}


//-----------------------------------------------------------------------------
TheGame::~TheGame( )
{
	delete m_player;
	for ( int i = 0; i < (int)m_enemies.size(); i++ ) delete m_enemies[ i ];
	for ( int i = 0; i < (int)m_bullets.size(); i++ ) delete m_bullets[ i ];
}


//-----------------------------------------------------------------------------
float g_counter = 0.f;
void TheGame::Update( float deltaSeconds )
{

	//if ( g_theApp->isKeyDown( 'A' ) && g_theApp->WasKeyJustPressed( 'A' ) ) g_counter += 10.f;

//----------------------------------- Input Handling

	if ( m_player->IsAlive() && g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ] != nullptr )
	{
		Vector2 leftStickPos = g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ]->GetLeftStickPosition();
		if ( leftStickPos.x != 0.f && leftStickPos.y != 0.f )
		{
			m_player->m_position += leftStickPos * PLAYER_SPEED_KNOB * deltaSeconds;	
			if ( m_player->m_position.x - m_player->m_cosmeticRadius < 0 ) m_player->m_position.x = m_player->m_cosmeticRadius;
			if ( m_player->m_position.x + m_player->m_cosmeticRadius > g_theApp->m_screenWidth ) m_player->m_position.x = g_theApp->m_screenWidth - m_player->m_cosmeticRadius;
			if ( m_player->m_position.y - m_player->m_cosmeticRadius < 0 ) m_player->m_position.y = m_player->m_cosmeticRadius;
			if ( m_player->m_position.y + m_player->m_cosmeticRadius > g_theApp->m_screenHeight ) m_player->m_position.y = g_theApp->m_screenHeight - m_player->m_cosmeticRadius;
		}
	}

	if ( m_player->IsAlive() && g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ] != nullptr )
	{
		Vector2 rightStickPos = g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ]->GetRightStickPosition();
		if ( rightStickPos.x != 0.f && rightStickPos.y != 0.f && m_shotCooldownTimerInSeconds >= TheGame::PLAYER_SECONDS_PER_SHOT )
		{
			m_shotCooldownTimerInSeconds = 0.f;
			m_player->m_orientation = RadiansToDegrees( g_theApp->m_controllers[ TheApp::CONTROLLER_ONE ]->GetRightStickPositionAsAngleInRadians() );
			m_bullets.push_back( new Bullet( m_player->GetLocalTipPosition() + m_player->GetPosition(), DegreesToRadians(m_player->GetOrientation()) ) );
		}
		else m_shotCooldownTimerInSeconds += deltaSeconds;
	}

//------------------------------------ Update Chains

	if ( m_player->IsAlive() ) m_player->Update( deltaSeconds ); //Update player representation.

	for ( auto enemyIter = m_enemies.begin( ); enemyIter != m_enemies.end( ); )
	{
		Entity2D* currentEnemy = *enemyIter;
		Vector2 dirToPlayer = m_player->m_position - currentEnemy->m_position;
		if (dirToPlayer.CalcLength() != 0) dirToPlayer.Normalize();
		currentEnemy->m_position += dirToPlayer * ENEMY_SPEED_KNOB * deltaSeconds;
		currentEnemy->Update( deltaSeconds );

		if ( m_player->IsAlive() && currentEnemy->DoesOverlap( *m_player ) )
		{
			currentEnemy->DecreaseHealth();
			m_player->DecreaseHealth();

			FlashRedScreen();
		}

		if ( currentEnemy->IsAlive() == false )
		{
			delete currentEnemy;
			enemyIter = m_enemies.erase( enemyIter );

			m_shouldSpawnEnemy = true;
		}
		else ++enemyIter;
	}
	//Can't handle enemy spawn INSIDE the loop because the vector seems to  wind up reallocating itself due to pushing back two new enemies, wrecking iterator integrity.
	if ( m_shouldSpawnEnemy )
	{
		m_shouldSpawnEnemy = false;
		SpawnEnemies( ENEMIES_PER_SPAWNING );
	}


	for ( auto bulletIter = m_bullets.begin( ); bulletIter != m_bullets.end( ); )
	{
		Bullet* currentBullet = *bulletIter;
		currentBullet->Update( deltaSeconds );

		for ( auto enemyIter = m_enemies.cbegin(); enemyIter != m_enemies.cend(); ++enemyIter )
		{
			Entity2D* currentEnemy = *enemyIter;
			if ( currentBullet->DoesOverlap( *currentEnemy ) )
			{
				currentBullet->DecreaseHealth();
				currentEnemy->DecreaseHealth();
			}
		}

		if ( currentBullet->IsAlive() == false )
		{
			delete currentBullet;
			bulletIter = m_bullets.erase( bulletIter );
		}
		else ++bulletIter;
	}

	if ( !m_player->IsAlive() )
	{
		if ( m_gameOverTimeInSeconds > TheGame::SECONDS_BETWEEN_DEATH_AND_EXIT )
		{
			m_shouldExitGame = true;
		}
		else m_gameOverTimeInSeconds += deltaSeconds;
	}

	if ( m_currentPointAlongTransition > 0.f )
	{
		m_currentPointAlongTransition -= deltaSeconds;
	}
	else if ( m_currentPointAlongTransition < 0.f )
	{
		m_shouldFlashRed = false;
		m_currentPointAlongTransition = 0.f;
	}
}


//-----------------------------------------------------------------------------
void TheGame::Render( )
{
	g_theRenderer->SetOrtho( Vector2( 0.f, 0.f ), Vector2( 1600.f, 900.f ) );

	if ( m_shouldFlashRed )
	{
		float amtAlpha = RangeMap( m_currentPointAlongTransition, 0.f, m_redtoBlackTransitionFullLengthInSeconds, 0.f, 1.f );
		g_theRenderer->DrawAABB( AABB2( 0.f, 0.f, 1600.f, 900.f ), 
			Rgba( 1.f, 0.f, 0.f, amtAlpha ) );

	}

	m_player->Render( ); //For player representation.

	//Drawing player oriented tip for debug to ensure bullets fire from right origin.
// 	Vector2 startPos = Vector2( m_player->GetPosition() + m_player->GetLocalTipPosition() );
// 	Vector2 endPos = Vector2( startPos.x + 3.f, startPos.y + 3.f );
// 	g_theRenderer->DrawLine( startPos, endPos, Rgba(), Rgba() );

	for ( auto enemyIter = m_enemies.begin(); enemyIter != m_enemies.end(); ++enemyIter )
	{
		Entity2D* currentEnemy = *enemyIter;
		currentEnemy->Render();
	}

	for ( auto bulletIter = m_bullets.begin(); bulletIter != m_bullets.end(); ++bulletIter )
	{
		Bullet* currentBullet = *bulletIter;
		currentBullet->Render();
	}

	//g_theRenderer->DrawTexturedAABB( AABB2( 1000.f, 200.f, 1400.f, 600.f ), *Texture::CreateOrGetTexture( "Data/Images/Paused.png" ) );
	//g_theRenderer->DrawPolygon( Vector2( 100.f, 100.f ), 50.f, 20.f, 90.f );
	
	//g_theRenderer->SetOrtho( Vector2( 0.f, 0.f ), Vector2( 800.f, 450.f ) );

	//g_theRenderer->DrawAABB( AABB2( 100.f + g_counter, 100.f, 400.f + g_counter, 400.f ), Rgba( 0.1f, 0.3f, 0.9f, 0.5f ) ); //Takes up half the height due to ortho call.

}


//-----------------------
void TheGame::SpawnEnemies( int numEnemiesToSpawn )
{
	static enum Side { TOP, LEFT, RIGHT, BOTTOM };
	for ( int i = 0; i < numEnemiesToSpawn; i++ )
	{
		//First, pick a side.
		Side side = (Side)GetRandomIntInRange( 0, 4 );
		m_enemies.push_back( new Entity2D( "Data/Images/Enemy.png", &ENEMY_COLOR ) );
		m_enemies.back()->SetHealth( 1 );

		float posX, posY;
		switch ( side )
		{
		case LEFT:
			posY = GetRandomFloatInRange( 0.f, g_theApp->m_screenHeight );
			m_enemies.back()->SetPosition( -m_enemies.back()->m_cosmeticRadius, posY );
			break;
		case RIGHT:
			posY = GetRandomFloatInRange( 0.f, g_theApp->m_screenHeight );
			m_enemies.back()->SetPosition( g_theApp->m_screenWidth + m_enemies.back()->m_cosmeticRadius, posY );
			break;
		case TOP:
			posX = GetRandomFloatInRange( 0.f, g_theApp->m_screenWidth );
			m_enemies.back()->SetPosition( posX, g_theApp->m_screenHeight + m_enemies.back()->m_cosmeticRadius );
			break;
		case BOTTOM:
			posX = GetRandomFloatInRange( 0.f, g_theApp->m_screenWidth );
			m_enemies.back()->SetPosition( posX, -m_enemies.back()->m_cosmeticRadius );
			break;
		}
	}
}


//--------------------------------------
void TheGame::FlashRedScreen()
{
	m_shouldFlashRed = true;
	switch ( m_player->m_health )
	{
	case 2:
	case 1:
		m_redtoBlackTransitionFullLengthInSeconds = 0.5f;
		break;
	case 0:
		m_redtoBlackTransitionFullLengthInSeconds = 2.0f;
		break;
	}
	m_currentPointAlongTransition = m_redtoBlackTransitionFullLengthInSeconds;
}