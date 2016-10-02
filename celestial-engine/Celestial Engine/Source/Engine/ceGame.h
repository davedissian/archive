/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

ceBEGIN
	class Game;
ceEND

#include "ceAudio.h"
#include "ceInput.h"
#include "cePhysics.h"
#include "ceRenderer.h"

ceBEGIN

// Current State
enum CurrentGameState
{
	GS_MENU,
	GS_SPGAME,
	GS_MPGAME
};

// The Main Menu would be implemented in this class
class MenuState
{
public:

};

// The single-player game would be implemented in this class
class SPGameState
{
public:

};

// The multi-player game would be implemented in this class
class MPGameState
{
public:

};

// Properties Structure
struct GameProperties
{
	string windowTitle;
	Vector2<u16> windowSize;
	bool fullscreen;
};

// A game made using celestial will be implemented mainly through this class
class Game
{
public:
						Game();
	virtual				~Game();

	// Initialisation
	virtual void		init( GameProperties &properties );

	// Run
	virtual s32			run() = 0;

	// Shutdown
	virtual void		forceExit();
	virtual void		release();

	// States
	virtual void		setMenuState( MenuState *menuState );
	virtual void		setSPGameState( SPGameState *gameState );
	virtual void		setMPGameState( MPGameState *gameState );

	// Properties
	GameProperties		&getProperties();

protected:
	GameProperties		mProperties;

	// Subsystems
	Audio				*mAudio;
	Input				*mInput;
	Physics				*mPhysics;
	Renderer			*mRenderer;

	// States
	MenuState			*mMenuState;
	SPGameState			*mSPGameState;
	MPGameState			*mMPGameState;
	s32					mCurrentState;

};

ceEND
