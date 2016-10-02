/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

ceBEGIN

// Physics Subsystem Base
class Physics
{
public:
	Physics( Game *parent ) { mGame = parent; }

	// Startup
	virtual void		startup() = 0;
	
	// Updating
	virtual bool		update() = 0;

protected:
	Game				*mGame;

};

ceEND
