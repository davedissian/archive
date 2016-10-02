/*
	Test Game
	Created by SerialVelocity
*/
#include "test.h"

int main()
{
	// Create a game object
	Game *game = new tGame;
	if( !game )
		return 1;

	// Initialise the game
	GameProperties props;
	props.windowTitle = "Test";
	props.windowSize.x = 1280;
	props.windowSize.y = 800;
	props.fullscreen = false;
	game->init( props );

	// Run the game
	s32 exitCode = game->run();

	// Release the game and exit
	game->release();
	delete game;
	return exitCode;
}