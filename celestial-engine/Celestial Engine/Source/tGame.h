/*
	Test Game
	Created by SerialVelocity
*/
#pragma once

// Test Project Game
class tGame : public Game
{
public:
						tGame();
						~tGame();

	// Initialisation
	void				init( GameProperties &properties );

	// Run
	s32					run();

	// Shutdown
	void				release();

	// States
	void				setMenuState( MenuState *menuState );
	void				setSPGameState( SPGameState *gameState );
	void				setMPGameState( MPGameState *gameState );

};
