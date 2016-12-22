#pragma once


#include <vector>


//-----------------------------------------------------------------------------
class TheGame;
class Entity2D;
class Bullet;


//-----------------------------------------------------------------------------
//Add other forward declarations for classes whose objects tracked below:
//class Bullet; //For Bullet* m_bullets[#] below, etc.

//-----------------------------------------------------------------------------
//Add constants required by header file, e.g. sizes of arrays on stack memory.
//const int MAX_NUMBER_OF_BULLETS = 400; //For Bullet* m_bullets[MNOB], etc.

//-----------------------------------------------------------------------------
extern TheGame* g_theGame;


//-----------------------------------------------------------------------------
class TheGame
{
private:
	//int m_numBulletsAllocated; //Check vs to not exceed array size max/min.
public:
	//Ship* m_ship; //Whatever represents the player, array if multiplayer.
	//Bullet* m_bullets[ MAX_NUMBER_OF_BULLETS ];

	Entity2D* m_player;

	std::vector< Entity2D* > m_enemies;
	std::vector< Bullet* > m_bullets;

	TheGame( );
	~TheGame( );
	void Update( float deltaSeconds );
	void Render( );

	void SpawnEnemies( int numEnemiesToSpawn );

	static const float PLAYER_SPEED_KNOB;
	static const float ENEMY_SPEED_KNOB;
	static const int ENEMIES_PER_SPAWNING;
	static const float PLAYER_SHOTS_PER_SECOND;
	static const float PLAYER_SECONDS_PER_SHOT;
	float m_shotCooldownTimerInSeconds;

	static const Rgba ENEMY_COLOR;
	static const Rgba PLAYER_COLOR;

	static const float SECONDS_BETWEEN_DEATH_AND_EXIT;
	float m_gameOverTimeInSeconds;
	bool m_shouldExitGame;

	bool m_shouldSpawnEnemy;

	void FlashRedScreen();
	bool m_shouldFlashRed;
	float m_currentPointAlongTransition;
	float m_redtoBlackTransitionFullLengthInSeconds;

};
