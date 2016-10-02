/*
	Velocity Engine
	By SerialVelocity
*/
#include "VE.hpp"
using namespace VE;

// Basic Window Proceedure
LRESULT CALLBACK WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg ) 
	{
		case WM_CLOSE:
			DestroyWindow( hWnd );
		break;

		case WM_DESTROY:
			PostQuitMessage( 0 );
		break;

		default:
			return DefWindowProc( hWnd, msg, wParam, lParam );
	} 
	return FALSE;
}

// Engine
Engine::Engine()
{
	// Set all members to default values
	mQuitFlag = 0;
	mWnd = 0;
	ZeroMemory( &mWndClass, sizeof( mWndClass ) );
	mClientWidth = 0;
	mClientHeight = 0;
	mNewScreen = 0;
	mEventSnooper = 0;
	mGraphics = 0;
	mAudioManager = 0;
	mInputManager = 0;
	mLuaState = 0;
	mMainCamera = 0;
	mDebugCamera = 0;
}

Engine::~Engine()
{
	LOGWRITE( "Engine released" );
}

Engine *Engine::getInstance()
{
	// Create an object which is automatically deleted on exit and return
	static Engine instance;
	return &instance;
}

void Engine::setup()
{
	// Output debug information
	std::cout << "Velocity Engine Version " VE_VERSION << std::endl << std::endl;

	// Flags
	bool fullscreen = false;

	// Get client width/height
	mClientWidth = fullscreen ? (UINT)GetSystemMetrics( 0 ) : 1280;
	mClientHeight = fullscreen ? (UINT)GetSystemMetrics( 1 ) : 800;

	// Register window class and create the window
	mWndClass.cbSize = sizeof( WNDCLASSEX );
	mWndClass.lpfnWndProc = WindowProc;
	mWndClass.hInstance = GetModuleHandle( 0 ); 
	mWndClass.hIcon = LoadIcon( 0, IDI_APPLICATION );
	mWndClass.hCursor = LoadCursor( 0, IDC_ARROW );
	mWndClass.hbrBackground = fullscreen ? 0 : (HBRUSH)( COLOR_WINDOW + 1 );
	mWndClass.lpszClassName = L"TRANSCENDENT_WND_CLASS";
	mWndClass.hIconSm = LoadIcon( 0, IDI_APPLICATION );
	mWndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx( &mWndClass );
	mWnd = CreateWindowEx( 0, mWndClass.lpszClassName, L"Transcendent", fullscreen ? WS_EX_TOPMOST | WS_POPUP : WS_OVERLAPPEDWINDOW | WS_VISIBLE, fullscreen ? 0 : 100, fullscreen ? 0 : 100, mClientWidth, mClientHeight, 0, 0, mWndClass.hInstance, 0 );
	ShowWindow( mWnd, SW_SHOWDEFAULT );
	UpdateWindow( mWnd );
	std::string windowType = "windowed";
	if( fullscreen == true )
		windowType = "fullscreen";
	LOGWRITE( "Window created at " << mClientWidth << "x" << mClientHeight << " in " << windowType << " mode." );

	// Create graphics
	GraphicsProperties gProperties;
	gProperties.backbufferFormat = D3DFMT_X8R8G8B8;
	gProperties.depthFormat = D3DFMT_D16;
	gProperties.hWnd = mWnd;
	gProperties.waitForVSync = true;
	gProperties.fullscreen = fullscreen;
	mGraphics = new Graphics( gProperties );
	LOGWRITE( "Graphics subsystem setup using Direct3D 9.0." );

	// Create audio manager
	mAudioManager = new AudioManager;
	mAudioManager->setup();
	LOGWRITE( "Audio subsystem setup using FMOD 2.0." );

	// Create input manager
	mInputManager = new InputManager;
	mInputManager->setup( mWnd, false, false );
	LOGWRITE( "Input subsystem setup using DireectInput 8.0." );

	// Create lua state manager
	mLuaState = new LuaState;
	LOGWRITE( "Lua subsystem setup using Lua 5.1." );

	// Setup Cameras
	mMainCamera = new Camera;
	mMainCamera->setup( 60, (float)mClientWidth / (float)mClientHeight, 1.0f, 100000.0f );
	mDebugCamera = new Camera;
	mDebugCamera->setup( 60, (float)mClientWidth / (float)mClientHeight, 1.0f, 100000.0f );

	// Add event listeners
	gEventDispatcher->addListener( this );
	gEventDispatcher->addListener( mGraphics );
	gEventDispatcher->addListener( mAudioManager );
	gEventDispatcher->addListener( mInputManager );

	// Create event snooper
	mEventSnooper = new EventSnooper;
	gEventDispatcher->addListener( mEventSnooper );
}

void Engine::setScreen( Screen *screen )
{
	// Update the new screen
	mNewScreen = screen;
}

bool Engine::run()
{
	// If the engine has set the quit flag, quit the loop
	if( mQuitFlag == true )
		return false;

	// If a new screen has been set, update
	if( mNewScreen != 0 )
	{
		gScreenManager->set( mNewScreen );
		mNewScreen = 0;
	}

	// Update the message pump
	MSG msg;
	while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == TRUE )
	{
		// If the message is WM_QUIT, quit
		if( msg.message == WM_QUIT )
			return false;

		// Process Messages
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	// If there is no screen active, quit
	if( !gScreenManager->screenExist() )
		return false;

	// Update subsystems
	gEventDispatcher->trigger( ET_TICK );
	gEventDispatcher->tick();

	// Update and render the current screen
	gScreenManager->getScreen()->onUpdate();
	gScreenManager->getScreen()->onRender();

	// Return true to continue running
	return true;
}

void Engine::addActor( Actor *actor )
{
	// Add an actor to the list
	mActorList.push_back( actor );
}

void Engine::removeActor( Actor *actor )
{
	// Remove an actor from the list
	mActorList.remove( actor );
}

void Engine::updateActors()
{
	// Update the actors in the list
	for( ActorList::iterator i = mActorList.begin(); i != mActorList.end(); i++ )
	{
		(*i)->onUpdate();
	}
}

void Engine::handleEvent( Event &ev )
{
	// Act on the event type
	int type = ev.getType();
	
	// A yes/no message box asking the user to quit
	if( type == ET_MESSAGEBOX_QUIT )
	{
		// If the GUI subsystem is not initialised, fall back to Win32
		/*
		if( mGUISubsystemReady )
		{
		}
		else
		*/
		{
			int result = MessageBoxA( 0, "Would you like to quit?", "Message", MB_ICONINFORMATION | MB_YESNO );
			if( result == IDYES )
			{
				// Set the quit flag to true
				mQuitFlag = true;
			}
		}
	}

	// Add a new actor when one is created
	if( type == ET_NEW_ACTOR )
	{
		// Add the actor to the list
		addActor( (Actor*)ev.getData() );
	}

	// Remove an actor when one dies
	if( type == ET_DEAD_ACTOR )
	{
		// Add the actor to the list
		removeActor( (Actor*)ev.getData() );
	}
}

void Engine::postEvent( int eventType, int eventDispatchFlag, void *eventData )
{
	// Trigger the event immediately if the flag is immediate
	if( eventDispatchFlag == EP_IMMEDIATE )
		gEventDispatcher->trigger( eventType, eventData );
	else
		gEventDispatcher->queue( eventType, eventData );
}

void Engine::shutdown()
{
	// Cameras
	SAFE_DELETE( mDebugCamera );
	SAFE_DELETE( mMainCamera );

	// Event snooper
	SAFE_DELETE( mEventSnooper );

	// Lua
	SAFE_DELETE( mLuaState );
	LOGWRITE( "Lua subsystem released." );

	// Input
	SAFE_DELETE( mInputManager );
	LOGWRITE( "Input subsystem released." );

	// Audio
	SAFE_DELETE( mAudioManager );
	LOGWRITE( "Audio subsystem released." );

	// Graphics
	SAFE_DELETE( mGraphics );
	LOGWRITE( "Graphics subsystem released." );

	// Win32
	DestroyWindow( mWnd );
	UnregisterClass( mWndClass.lpszClassName, mWndClass.hInstance );
	LOGWRITE( "Window released." );
}

unsigned int Engine::getClientWidth()
{
	// Return the client width
	return mClientWidth;
}

unsigned int Engine::getClientHeight()
{
	// Return the client height
	return mClientHeight;
}

HWND Engine::getWnd()
{
	// Return the current window
	return mWnd;
}

WNDCLASSEX &Engine::getWndClass()
{
	// Return the current window class
	return mWndClass;
}

Graphics *Engine::getGraphics()
{
	// Return the scene manager
	return mGraphics;
}

AudioManager *Engine::getAudioManager()
{
	// Return the scene manager
	return mAudioManager;
}

InputManager *Engine::getInputManager()
{
	// Return the input manager
	return mInputManager;
}

LuaState *Engine::getLuaState()
{
	// Return the lua state manager
	return mLuaState;
}

Camera *Engine::getCamera()
{
	// Return the camera
	return mGraphics->getSceneGraph()->getCamera();
}