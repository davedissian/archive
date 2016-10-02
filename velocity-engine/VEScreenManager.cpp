/*
	Velocity Engine
	By SerialVelocity
*/
#include "VEBase.hpp"
#include "VEScreen.hpp"
#include "VEScreenManager.hpp"
using namespace VE;

// Screen Manager
ScreenManager::ScreenManager()
{
	// Set all members to default values
	mScreen = 0;
}

ScreenManager::~ScreenManager()
{
}

ScreenManager *ScreenManager::getInstance()
{
	// Create an object which is automatically deleted on exit and return
	static ScreenManager instance;
	return &instance;
}

void ScreenManager::set( Screen *screen )
{
	// Remove the current screen
	if( mScreen )
		mScreen->onPop();

	// Update the screen
	mScreen = screen;
	screen->onPush();
}

Screen *ScreenManager::getScreen()
{
	// Return the current screen
	return mScreen;
}

bool ScreenManager::screenExist()
{
	// If the screen is 0, it doesnt exist and return false
	return ( mScreen ) ? true : false;
}
