/*
	Celestial Engine
	Created by SerialVelocity
*/
#include "celestialBase.h"

ceBEGIN

// Game Member Functions
Game::Game()
{
	mAudio = 0;
	mInput = 0;
	mPhysics = 0;
	mRenderer = 0;
}

Game::~Game()
{
}

void Game::init( GameProperties &properties )
{
	mProperties = properties;
}

void Game::forceExit()
{
	// Release engine resources
	release();

	// Terminate the process
	exit( 1 );
}

void Game::release()
{
	ceSAFE_DELETE( mRenderer );
	ceSAFE_DELETE( mPhysics );
	ceSAFE_DELETE( mInput );
	ceSAFE_DELETE( mAudio );
}

void Game::setMenuState( MenuState *menuState )
{
}

void Game::setSPGameState( SPGameState *gameState )
{
}

void Game::setMPGameState( MPGameState *gameState )
{
}

GameProperties &Game::getProperties()
{
	return mProperties;
}

ceEND
