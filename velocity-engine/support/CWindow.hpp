/*
	Window Support Library
	By SerialVelocity
*/
#pragma once

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Window creation flags
enum CWindow_CreationFlags
{
	CWnd_Windowed = 0x01,
	CWnd_Fullscreen = 0x02,
};

// Window class
class CWindow
{
public:
						CWindow( int flags, PCTSTR wndTitle, PCTSTR className, UINT width, UINT height, HICON icon, WNDPROC wndProc );
						~CWindow();

	// Accessors
	WNDCLASSEX			&getClass();
	HWND				getHandle();

private:
	// Class
	WNDCLASSEX			mClass;

	// Handle
	HWND				mHandle;

};
