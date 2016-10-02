/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

VE_BEGIN

// Screen Manager
class ScreenManager
{
						ScreenManager();
						~ScreenManager();

public:
	// Instance
	static ScreenManager *getInstance();

	// Modifiers
	void				set( Screen *screen );

	// Accessors
	Screen				*getScreen();
	bool				screenExist();

private:
	// Current Screen
	Screen				*mScreen;

};

VE_END
