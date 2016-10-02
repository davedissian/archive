/*
	Velocity Engine
	By SerialVelocity
*/
#include "VEBase.hpp"
#include "VEInput.hpp"
using namespace VE;

// Input Manager
InputManager::InputManager()
{
	// Set the DirectInput interfaces to 0
	mDI = 0;
	mDIKeyboard = 0;
	mDIMouse = 0;
	
	// Set the variable defaults
	mCursorVisible = true;
	mFocusState = true;

	// Empty the Mouse and Key States
	ZeroMemory( mKeystateList, sizeof( mKeystateList ) );
	ZeroMemory( &mMouseState, sizeof( mMouseState ) );
}

InputManager::~InputManager()
{
	SAFE_UNACQUIRE( mDIMouse );
	SAFE_UNACQUIRE( mDIKeyboard );
	SAFE_RELEASE( mDI );
}

void InputManager::setup( HWND hWnd, bool KeyboardExclusive, bool MouseExclusive )
{
	// Setup the devices
	DirectInput8Create( GetModuleHandle( 0 ), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDI, 0 );
	mDI->CreateDevice( GUID_SysKeyboard, &mDIKeyboard, 0 );
	mDIKeyboard->SetDataFormat( &c_dfDIKeyboard );
	mDIKeyboard->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | ( ( KeyboardExclusive == true ) ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE ) );
	mDIKeyboard->Acquire();
	mDI->CreateDevice( GUID_SysMouse, &mDIMouse, 0 );
	mDIMouse->SetDataFormat( &c_dfDIMouse );
	mDIMouse->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | ( ( MouseExclusive == true ) ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE ) );
	mDIMouse->Acquire();

	// Update flags
	mWnd = hWnd;
}

void InputManager::showCursor()
{
	// If the cursor is invisible then show
	if( !mCursorVisible )
	{
		ShowCursor( 1 );
		mCursorVisible = true;
	}
}

void InputManager::hideCursor()
{
	// If the cursor is visible then hide
	if( mCursorVisible )
	{
		ShowCursor( 0 );
		mCursorVisible = false;
	}
}

void InputManager::setCursorPosition( IntVec2 newPos )
{
	// Set the cursor position
	SetCursorPos( newPos.x, newPos.y );
}

IntVec2 InputManager::getLocalCursorPosition()
{
	// Get the Screen Cursor Position
	IntVec2 cPos = this->getCursorPosition();

	// Transform it to Client Position
	POINT MP; MP.x = cPos.x; MP.y = cPos.y;
	ScreenToClient( mWnd, &MP );
	return IntVec2( MP.x, MP.y );
}

IntVec2 InputManager::getCursorPosition()
{
	// Get the cursor position
	POINT Result;
	GetCursorPos( &Result );
	return IntVec2( (int)Result.x, (int)Result.y );
}

bool InputManager::keyDown( int key )
{
	if( !mFocusState )
		return false;

	// Check the state of the key at the keycode
	if( mKeystateList[key] & 0x80 )
		return true;
	return false;
}

bool InputManager::keyUp( int key )
{
	// Return the reverse of the other function
	return !this->keyDown( key );
}

bool InputManager::mouseButtonDown( int button )
{
	if( !mFocusState )
		return false;

	// Return the state of the mouse button
	if( mMouseState.rgbButtons[button] & 0x80 )
		return true;
	return false;
}

bool InputManager::mouseButtonUp( int button )
{
	// Return the reverse of the sister function
	return !this->mouseButtonDown( button );
}

IntVec3 InputManager::mouseMove()
{
	// Convert the MouseState XYZ values into an 3D integer vector
	if( !mFocusState )
		return IntVec3( 0, 0, 0 );
	else
		return IntVec3( (int)mMouseState.lX, (int)mMouseState.lY, (int)mMouseState.lZ );
}

void InputManager::tick()
{
	if( mDI )
	{
		// Fill the Keystate List array with the keystates on the keyboard
		if( mDIKeyboard )
		{
			ZeroMemory( mKeystateList, sizeof( mKeystateList ) );
			HRESULT hr = mDIKeyboard->GetDeviceState( sizeof( mKeystateList ), mKeystateList );
			if( FAILED( hr ) )
			{
				// If input is lost then acquire and keep trying until we get it back 
				hr = mDIKeyboard->Acquire();
				while( hr == DIERR_INPUTLOST ) 
				{          
					hr = mDIKeyboard->Acquire();
				}

				// Could be we failed for some other reason
				if( FAILED( hr ) )
					return;

				// Now read the state again
				HRESULT hr = mDIKeyboard->GetDeviceState( sizeof( mKeystateList ), mKeystateList );
			}
		}

		// Fill the state of the Mouse and the Mouse Buttons
		if( mDIMouse )
		{
			ZeroMemory( &mMouseState, sizeof( mMouseState ) );
			HRESULT hr = mDIMouse->GetDeviceState( sizeof( mMouseState ), (void*)&mMouseState );
			if( FAILED( hr ) )
			{
				// If input is lost then acquire and keep trying until we get it back 
				hr = mDIMouse->Acquire();
				while( hr == DIERR_INPUTLOST ) 
				{          
					hr = mDIMouse->Acquire();
				}

				// Could be we failed for some other reason
				if( FAILED( hr ) )
					return;

				// Now read the state again
				HRESULT hr = mDIMouse->GetDeviceState( sizeof( mMouseState ), (void*)&mMouseState );
			}
		}
	}
}

void InputManager::zeroInput()
{
	// Clear any input
	ZeroMemory( mKeystateList, sizeof( mKeystateList ) );
	ZeroMemory( (void*)&mMouseState, sizeof( mMouseState ) );
}

void InputManager::handleEvent( Event &ev )
{
	// Act on the event type
	int type = ev.getType();
	
	// Tick
	if( type == ET_TICK )
		tick();
}

void InputManager::setFocus( bool state )
{
	// Update the state
	mFocusState = state;
}