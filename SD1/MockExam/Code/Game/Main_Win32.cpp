#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <cassert>
#include <crtdbg.h>
#include <stdlib.h>
#include <time.h>
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Time/Time.hpp"
#include "Engine/Audio/TheAudio.hpp"
#include "Game/TheApp.hpp"
#include "Game/TheGame.hpp"

//-----------------------------------------------------------------------------------------------
#define UNUSED(x) (void)(x);


//-----------------------------------------------------------------------------------------------
//Noting that this sets (0,0) is in the bottom-left until SetOrtho is called later on.
const int OFFSET_FROM_WINDOWS_DESKTOP = 50;
const int WINDOW_PHYSICAL_WIDTH = 1600;
const int WINDOW_PHYSICAL_HEIGHT = 900;
const float VIEW_LEFT = 0.0;
const float VIEW_RIGHT = 1600.0;
const float VIEW_BOTTOM = 0.0;
const float VIEW_TOP = VIEW_RIGHT * static_cast<float>( WINDOW_PHYSICAL_HEIGHT ) / static_cast<float>( WINDOW_PHYSICAL_WIDTH );

float g_spinCounter;
float g_bKeyStopwatch;
bool g_isQuitting = false;
HWND g_hWnd = nullptr;
HDC g_displayDeviceContext = nullptr;
HGLRC g_openGLRenderingContext = nullptr;
const char* APP_NAME = "Assault (Milestone 01) by Benjamin Gibson";


//-----------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowsMessageHandlingProcedure( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	unsigned char asKey = (unsigned char)wParam;
	switch ( wmMessageCode )
	{
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
		g_isQuitting = true;
		return 0;

	case WM_KEYDOWN:
		g_theApp->SetKeyDownStatus( asKey, true );
		if ( asKey == VK_ESCAPE )
		{
			g_isQuitting = true;
			return 0;
		}
		//		g_keyDownArray[ asKey ] = true;
		break;

	case WM_KEYUP:
		g_theApp->SetKeyDownStatus( asKey, false );
		//		g_keyDownArray[ asKey ] = false;
		break;
	}

	return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );
}


//-----------------------------------------------------------------------------------------------
void CreateOpenGLWindow( HINSTANCE applicationInstanceHandle )
{
	// Define a window class
	WNDCLASSEX windowClassDescription;
	memset( &windowClassDescription, 0, sizeof( windowClassDescription ) );
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>( WindowsMessageHandlingProcedure ); // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle( NULL );
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT( "Simple Window Class" );
	RegisterClassEx( &windowClassDescription );

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow( );
	GetClientRect( desktopWindowHandle, &desktopRect );

	RECT windowRect = { OFFSET_FROM_WINDOWS_DESKTOP, OFFSET_FROM_WINDOWS_DESKTOP, OFFSET_FROM_WINDOWS_DESKTOP + WINDOW_PHYSICAL_WIDTH, OFFSET_FROM_WINDOWS_DESKTOP + WINDOW_PHYSICAL_HEIGHT };
	AdjustWindowRectEx( &windowRect, windowStyleFlags, FALSE, windowStyleExFlags );

	WCHAR windowTitle[ 1024 ];
	MultiByteToWideChar( GetACP( ), 0, APP_NAME, -1, windowTitle, sizeof( windowTitle ) / sizeof( windowTitle[ 0 ] ) );
	g_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL );

	ShowWindow( g_hWnd, SW_SHOW );
	SetForegroundWindow( g_hWnd );
	SetFocus( g_hWnd );

	g_displayDeviceContext = GetDC( g_hWnd );

	HCURSOR cursor = LoadCursor( NULL, IDC_ARROW );
	SetCursor( cursor );

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset( &pixelFormatDescriptor, 0, sizeof( pixelFormatDescriptor ) );
	pixelFormatDescriptor.nSize = sizeof( pixelFormatDescriptor );
	pixelFormatDescriptor.nVersion = 1;
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits = 24;
	pixelFormatDescriptor.cDepthBits = 24;
	pixelFormatDescriptor.cAccumBits = 0;
	pixelFormatDescriptor.cStencilBits = 8;

	int pixelFormatCode = ChoosePixelFormat( g_displayDeviceContext, &pixelFormatDescriptor );
	SetPixelFormat( g_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor );
	g_openGLRenderingContext = wglCreateContext( g_displayDeviceContext );
	wglMakeCurrent( g_displayDeviceContext, g_openGLRenderingContext );

	g_theRenderer->SetOrtho( Vector2( VIEW_LEFT, VIEW_BOTTOM ), Vector2( VIEW_RIGHT, VIEW_TOP ) );
	g_theRenderer->SetRenderFlag( GL_BLEND );
	g_theRenderer->SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	g_theRenderer->SetLineWidth( 2.f );
	g_theRenderer->SetRenderFlag( GL_LINE_SMOOTH );
}


//-----------------------------------------------------------------------------------------------
void RunMessagePump( )
{
	MSG queuedMessage;
	for ( ;; )
	{
		const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
		if ( !wasMessagePresent )
		{
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage );
	}
}


//-----------------------------------------------------------------------------------------------
void Update( )
{
	static double s_timeLastFrameBegan = GetCurrentTimeSeconds( ); //Called only once due static.
	double timeThisFrameBegan = GetCurrentTimeSeconds( );
	float deltaSeconds = static_cast<float>( timeThisFrameBegan - s_timeLastFrameBegan );
	s_timeLastFrameBegan = timeThisFrameBegan;

	g_theGame->Update( deltaSeconds ); //Can pass in 0 to freeze, or others to rewind/slow/etc.
}


//-----------------------------------------------------------------------------------------------
void Render( )
{
	g_theRenderer->ClearScreenToColor( 0.f, 0.f, 0.f );
	g_theGame->Render( );
	SwapBuffers( g_displayDeviceContext );
}

//-----------------------------------------------------------------------------------------------
AudioChannelHandle g_bgMusicChannel;
void RunFrame( )
{
	g_theApp->Update(); //Resets keys, putting this here validates WasKeyJustChanged.
	
	g_theAudio->Update();

	RunMessagePump( ); //Can rename this to just doInput, because it handles updating key callback.
	Update();
	if ( g_theApp->isKeyDown( 'X' ) && g_theApp->WasKeyJustPressed( 'X' ) ) g_theAudio->StopChannel( g_bgMusicChannel );
	Render( );
}


//-----------------------------------------------------------------------------------------------
void Initialize( HINSTANCE applicationInstanceHandle )
{
	srand( static_cast<unsigned int>( time( NULL ) ) );
	CreateOpenGLWindow( applicationInstanceHandle );
	g_theApp = new TheApp( VIEW_RIGHT, VIEW_TOP );
	g_theGame = new TheGame();
	g_theRenderer = new TheRenderer();

	g_theAudio = new AudioSystem();
	SoundID musicID;
	musicID = g_theAudio->CreateOrGetSound( "Data/Audio/Yume Nikki mega mix (SD).mp3" );
	g_bgMusicChannel = g_theAudio->PlaySound( musicID );
}


//-----------------------------------------------------------------------------------------------
void Shutdown( )
{
	delete g_theApp;
	delete g_theGame;
	delete g_theRenderer;

	g_theApp = nullptr;
	g_theGame = nullptr;
	g_theRenderer = nullptr;
}


//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int )
{
	UNUSED( commandLineString );
	Initialize( applicationInstanceHandle );

	while ( !g_isQuitting )
	{
		if ( g_theGame->m_shouldExitGame ) break;
		RunFrame( );
	}

	Shutdown( );
	return 0;
}