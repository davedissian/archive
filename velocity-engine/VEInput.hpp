/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

// DI8
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>
#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dxguid.lib" )
#define SAFE_UNACQUIRE( x ) if( ( x ) ) { (x)->Unacquire(); (x)->Release(); (x) = 0; }

VE_BEGIN

// Input Manager
class InputManager : public EventListener
{
public:
						InputManager();
						~InputManager();

	// Initialise
	void				setup( HWND hWnd, bool KeyboardExclusive, bool MouseExclusive );

	// Modifiers
	void				showCursor();
	void				hideCursor();
	void				setCursorPosition( IntVec2 NewPosition );
	IntVec2				getLocalCursorPosition();
	IntVec2				getCursorPosition();

	// Queries
	bool				keyDown( int key );
	bool				keyUp( int key );
	bool				mouseButtonDown( int button );
	bool				mouseButtonUp( int button );
	IntVec3				mouseMove();

	// Updating
	void				tick();
	void				zeroInput();
	void				handleEvent( Event &ev );
	void				setFocus( bool state );

private:
	// DirectInput
	IDirectInput8		*mDI;
	IDirectInputDevice8 *mDIKeyboard;
	IDirectInputDevice8 *mDIMouse;
	HWND				mWnd;
		
	// Keyboard State List
	BYTE				mKeystateList[256];
		
	// Mouse State
	DIMOUSESTATE		mMouseState;
	bool				mCursorVisible;
	bool				mFocusState;
};

VE_END
