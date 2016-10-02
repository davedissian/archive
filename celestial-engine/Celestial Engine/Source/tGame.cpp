/*
	Test Game
	Created by SerialVelocity
*/
#include "test.h"

tGame::tGame() : Game()
{
}

tGame::~tGame()
{
}

void tGame::init( GameProperties &properties )
{
	// Call the base function
	Game::init( properties );

	// Create the renderer subsystem
	mRenderer = new D3D11Renderer( this );
	mRenderer->startup( new BasicSceneGraph( this ) );
}

s32 tGame::run()
{
	while( mRenderer->update() )
	{
		mRenderer->rBegin( Vector3f( 0.0f, 40.0f / 255.0f, 100.0f / 255.0f ) );
		mRenderer->rEnd();
		mRenderer->rPresent();
	}

	return 0;
}

void tGame::release()
{
	// Release subsystems
	Game::release();
}

void tGame::setMenuState( MenuState *menuState )
{
}

void tGame::setSPGameState( SPGameState *gameState )
{
}

void tGame::setMPGameState( MPGameState *gameState )
{
}
