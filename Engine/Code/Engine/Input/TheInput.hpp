#pragma once


#include "Engine/Math/Vector2.hpp"


//-----------------------------------------------------------------------------
class TheInput;
class XboxController;


//-----------------------------------------------------------------------------
extern TheInput* g_theInput;
enum Controllers { CONTROLLER_ONE, CONTROLLER_TWO, CONTROLLER_THREE, CONTROLLER_FOUR, NUM_CONTROLLERS };
enum ControllerButtons { A_BUTTON, B_BUTTON, X_BUTTON, Y_BUTTON, LEFT_SHOULDER_BUTTON, RIGHT_SHOULDER_BUTTON, 
	BACK_BUTTON, START_BUTTON, LEFT_STICK_BUTTON, RIGHT_STICK_BUTTON, NUM_BUTTONS };


//-----------------------------------------------------------------------------
struct KeyButtonState
{
	bool m_isKeyDown;
	bool m_didKeyJustChange;
};


//-----------------------------------------------------------------------------
class TheInput
{
public:
	TheInput();

	void SetKeyDownStatus( unsigned char asKey, bool isNowDown ); //Used by WinMain key callback.
	bool IsKeyDown( unsigned char keyID ) const;
	bool WasKeyJustPressed( unsigned char keyID ) const;
	bool WasKeyPressedOnce( unsigned char keyID ) const;

	bool IsButtonDown( ControllerButtons buttonID, Controllers controllerID ) const;
	bool WasButtonJustPressed( ControllerButtons buttonID, Controllers controllerID ) const;
	bool WasButtonPressedOnce( ControllerButtons buttonID, Controllers controllerID ) const;

	Vector2 GetLeftStickPosition( Controllers controllerID ) const;
	float GetLeftStickPositionAsRadius( Controllers controllerID ) const;
	float GetLeftStickPositionAsAngleInRadians( Controllers controllerID ) const;

	Vector2 GetRightStickPosition( Controllers controllerID ) const;
	float GetRightStickPositionAsRadius( Controllers controllerID ) const;
	float GetRightStickPositionAsAngleInRadians( Controllers controllerID ) const;

	float GetLeftTrigger( Controllers controllerID ) const;
	float GetRightTrigger( Controllers controllerID ) const;

	void Update();

private:
	static const int NUM_KEYS = 250;
	KeyButtonState m_keys[ NUM_KEYS ];
	XboxController* m_controllers[ Controllers::NUM_CONTROLLERS ];
};
