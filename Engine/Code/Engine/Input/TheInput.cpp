#include "Engine/Input/TheInput.hpp"
#include "Engine/Error/ErrorWarningAssert.hpp"


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include "Engine/Input/XboxController.hpp"


//--------------------------------------------------------------------------------------------------------------
TheInput* g_theInput = nullptr;


//--------------------------------------------------------------------------------------------------------------
TheInput::TheInput()
{
	for ( int keyIndex = 0; keyIndex < NUM_KEYS; ++keyIndex )
	{
		m_keys[ keyIndex ].m_isKeyDown = false;
		m_keys[ keyIndex ].m_didKeyJustChange = false;
	}
	for ( int controllerSlot = 0; controllerSlot < NUM_CONTROLLERS; ++controllerSlot )
	{
		m_controllers[ controllerSlot ] = nullptr;
	}
}


//--------------------------------------------------------------------------------------------------------------
void TheInput::SetKeyDownStatus( unsigned char asKey, bool isNowDown )
{
	m_keys[ asKey ].m_didKeyJustChange = ( m_keys[ asKey ].m_isKeyDown != isNowDown ? true : false );
	m_keys[ asKey ].m_isKeyDown = isNowDown;
}


//--------------------------------------------------------------------------------------------------------------
bool TheInput::IsKeyDown( unsigned char keyID ) const
{
	return m_keys[ keyID ].m_isKeyDown;
}


//--------------------------------------------------------------------------------------------------------------
bool TheInput::WasKeyJustPressed( unsigned char keyID ) const
{
	return m_keys[ keyID ].m_didKeyJustChange;
}


//--------------------------------------------------------------------------------------------------------------
bool TheInput::WasKeyPressedOnce( unsigned char keyID ) const
{
	return m_keys[ keyID ].m_didKeyJustChange && m_keys[ keyID ].m_isKeyDown;
}


//--------------------------------------------------------------------------------------------------------------
bool TheInput::IsButtonDown( ControllerButtons buttonID, Controllers controllerID ) const
{
	switch ( buttonID )
	{
	case ControllerButtons::A_BUTTON: return m_controllers[ controllerID ]->IsAButtonDown();
	case ControllerButtons::B_BUTTON: return m_controllers[ controllerID ]->IsBButtonDown();
	case ControllerButtons::X_BUTTON: return m_controllers[ controllerID ]->IsXButtonDown();
	case ControllerButtons::Y_BUTTON: return m_controllers[ controllerID ]->IsYButtonDown();
	case ControllerButtons::LEFT_SHOULDER_BUTTON: return m_controllers[ controllerID ]->IsLeftShoulderButtonDown();
	case ControllerButtons::RIGHT_SHOULDER_BUTTON: return m_controllers[ controllerID ]->IsRightShoulderButtonDown();
	case ControllerButtons::BACK_BUTTON: return m_controllers[ controllerID ]->IsBackButtonDown();
	case ControllerButtons::START_BUTTON: return m_controllers[ controllerID ]->IsStartButtonDown();
	case ControllerButtons::LEFT_STICK_BUTTON: return m_controllers[ controllerID ]->IsLeftStickClickedIn();
	case ControllerButtons::RIGHT_STICK_BUTTON: return m_controllers[ controllerID ]->IsRightStickClickedIn();
	default: ERROR_AND_DIE( "Invalid ButtonID in TheInput::IsButtonDown()!" );
	}
}


//--------------------------------------------------------------------------------------------------------------
bool TheInput::WasButtonJustPressed( ControllerButtons buttonID, Controllers controllerID ) const
{
	switch ( buttonID )
	{
		case ControllerButtons::A_BUTTON: return m_controllers[ controllerID ]->WasAButtonJustPressed();
		case ControllerButtons::B_BUTTON: return m_controllers[ controllerID ]->WasBButtonJustPressed();
		case ControllerButtons::X_BUTTON: return m_controllers[ controllerID ]->WasXButtonJustPressed();
		case ControllerButtons::Y_BUTTON: return m_controllers[ controllerID ]->WasYButtonJustPressed();
		case ControllerButtons::LEFT_SHOULDER_BUTTON: return m_controllers[ controllerID ]->WasLeftShoulderButtonJustPressed();
		case ControllerButtons::RIGHT_SHOULDER_BUTTON: return m_controllers[ controllerID ]->WasRigtShoulderButtonJustPressed();
		case ControllerButtons::BACK_BUTTON: return m_controllers[ controllerID ]->WasBackButtonJustPressed();
		case ControllerButtons::START_BUTTON: return m_controllers[ controllerID ]->WasStartButtonJustPressed();
		case ControllerButtons::LEFT_STICK_BUTTON: return m_controllers[ controllerID ]->WasLeftStickClickJustPressed();
		case ControllerButtons::RIGHT_STICK_BUTTON: return m_controllers[ controllerID ]->WasRightStickClickJustPressed();
		default: ERROR_AND_DIE( "Invalid ButtonID in TheInput::WasButtonJustPressed()!" );
	}
}


//--------------------------------------------------------------------------------------------------------------
bool TheInput::WasButtonPressedOnce( ControllerButtons buttonID, Controllers controllerID ) const
{
	return IsButtonDown( buttonID, controllerID ) && WasButtonPressedOnce( buttonID, controllerID );
}


//--------------------------------------------------------------------------------------------------------------
Vector2 TheInput::GetLeftStickPosition( Controllers controllerID ) const
{
	return m_controllers[ controllerID ]->GetLeftStickPosition();
}


//--------------------------------------------------------------------------------------------------------------
float TheInput::GetLeftStickPositionAsRadius( Controllers controllerID ) const
{
	return m_controllers[ controllerID ]->GetLeftStickPositionAsRadius();
}


//--------------------------------------------------------------------------------------------------------------
float TheInput::GetLeftStickPositionAsAngleInRadians( Controllers controllerID ) const
{
	return m_controllers[ controllerID ]->GetLeftStickPositionAsAngleInRadians();
}


//--------------------------------------------------------------------------------------------------------------
Vector2 TheInput::GetRightStickPosition( Controllers controllerID ) const
{
	return m_controllers[ controllerID ]->GetRightStickPosition();
}


//--------------------------------------------------------------------------------------------------------------
float TheInput::GetRightStickPositionAsRadius( Controllers controllerID ) const
{
	return m_controllers[ controllerID ]->GetRightStickPositionAsRadius();
}


//--------------------------------------------------------------------------------------------------------------
float TheInput::GetRightStickPositionAsAngleInRadians( Controllers controllerID ) const
{
	return m_controllers[ controllerID ]->GetRightStickPositionAsAngleInRadians();
}


//--------------------------------------------------------------------------------------------------------------
float TheInput::GetLeftTrigger( Controllers controllerID ) const
{
	return m_controllers[ controllerID ]->GetLeftTrigger( );
}


//--------------------------------------------------------------------------------------------------------------
float TheInput::GetRightTrigger( Controllers controllerID ) const
{
	return m_controllers[ controllerID ]->GetRightTrigger();
}


//--------------------------------------------------------------------------------------------------------------
void TheInput::Update( )
{
	for ( int keyIndex = 0; keyIndex < NUM_KEYS; ++keyIndex )
		m_keys[ keyIndex ].m_didKeyJustChange = false;

	//Check for controller updates, drop-ins/outs.
	XINPUT_STATE xboxControllerState;
	memset( &xboxControllerState, 0, sizeof( xboxControllerState ) );
	for ( int controllerSlot = 0; controllerSlot < 3; controllerSlot++ )
	{
		DWORD errorStatus = XInputGetState( controllerSlot, &xboxControllerState );
		if ( errorStatus == ERROR_SUCCESS )
		{
			if ( m_controllers[ controllerSlot ] == nullptr )
			{
				m_controllers[ controllerSlot ] = new XboxController( controllerSlot );
			}
			m_controllers[ controllerSlot ]->Update();
		}
		else if ( errorStatus == ERROR_DEVICE_NOT_CONNECTED )
		{

			if ( m_controllers[ controllerSlot ] != nullptr )
			{
				delete m_controllers[ controllerSlot ];
				m_controllers[ controllerSlot ] = nullptr;
			}
		}
	}
}
