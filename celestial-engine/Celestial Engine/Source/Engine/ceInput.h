/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

ceBEGIN

// Input Subsystem Base
class Input
{
public:
	Input( Game *parent ) { mGame = parent; }

	// Startup
	virtual void		startup() = 0;
	
	// Updating
	virtual bool		update() = 0;

protected:
	Game				*mGame;

};

ceEND
