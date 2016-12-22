#pragma once
#include "Engine/Input/XboxController.hpp"

//-----------------------------------------------------------------------------
class TheApp;


//-----------------------------------------------------------------------------
const int NUM_KEYS = 250;
const int NUM_CONTROLLERS = 4;
extern TheApp* g_theApp;


//-----------------------------------------------------------------------------
struct KeyButtonState
{
	bool m_isKeyDown;
	bool m_didKeyJustChange;
};


//-----------------------------------------------------------------------------
class TheApp
{
public:
	double m_screenWidth;
	double m_screenHeight;
	XboxController* m_controllers[ NUM_CONTROLLERS ];
	enum Controllers { CONTROLLER_ONE, CONTROLLER_TWO, CONTROLLER_THREE, CONTROLLER_FOUR };
	TheApp( );
	TheApp( const double screenWidth, const double screenHeight );
	void SetKeyDownStatus( unsigned char asKey, bool isNowDown );
	bool isKeyDown( unsigned char keyID );
	bool WasKeyJustPressed( unsigned char keyID );
	void Update( );
private:
	KeyButtonState m_keys[ NUM_KEYS ];
}; 
