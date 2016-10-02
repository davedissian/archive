/*
	Window Support Library
	By SerialVelocity
*/
#include "CWindow.hpp"

// Window Class
CWindow::CWindow( int flags, PCTSTR wndTitle, PCTSTR className, UINT width, UINT height, HICON icon, WNDPROC wndProc )
{
	// Setup the window class
	mClass.cbSize = sizeof( WNDCLASSEX );
	mClass.lpfnWndProc = wndProc;
	mClass.hInstance = GetModuleHandle( 0 ); 
	mClass.hCursor = LoadCursor( 0, IDC_ARROW );
	mClass.hbrBackground = ( flags & CWnd_Fullscreen ) ? 0 : (HBRUSH)( COLOR_WINDOW + 1 );
	mClass.lpszClassName = className;
	mClass.hIcon = icon;
	mClass.hIconSm = icon;
	mClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx( &mClass );

	// Create the window
	mHandle = CreateWindowEx( 0, mClass.lpszClassName, wndTitle, ( flags & CWnd_Fullscreen ) ? WS_EX_TOPMOST | WS_POPUP : WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		( flags & CWnd_Fullscreen ) ? 0 : 100, ( flags & CWnd_Fullscreen ) ? 0 : 100, width, height, 0, 0, mClass.hInstance, 0 );
	ShowWindow( mHandle, SW_SHOWDEFAULT );
	UpdateWindow( mHandle );
}

CWindow::~CWindow()
{
	// Destroy the window
	DestroyWindow( mHandle );
	UnregisterClass( mClass.lpszClassName, GetModuleHandle( 0 ) );
}

WNDCLASSEX &CWindow::getClass()
{
	// Return the class
	return mClass;
}

HWND CWindow::getHandle()
{
	// Return the handle
	return mHandle;
}
