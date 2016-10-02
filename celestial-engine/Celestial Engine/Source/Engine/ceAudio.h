/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

ceBEGIN

// Audio Subsystem Base
class Audio
{
public:
	Audio( Game *parent ) { mGame = parent; }

	// Startup
	virtual void		startup() = 0;
	
	// Updating
	virtual bool		update() = 0;

protected:
	Game				*mGame;

};

ceEND
