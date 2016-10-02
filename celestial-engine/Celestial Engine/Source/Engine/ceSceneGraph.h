/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once
#include "ceSceneNode.h"

ceBEGIN

class SceneGraph
{
public:
	SceneGraph( Game *parent ) { mGame = parent; }

	// Attach/Detach
	virtual void		attach( SceneNode &node ) = 0;
	virtual void		detach( SceneNode &node ) = 0;

	// Drawing
	virtual void		draw() = 0;

	// Matrix Stack
	virtual MatrixStack	&getMatStack() { return mMatStack; }

protected:
	Game				*mGame;

	// Matrix Stack
	MatrixStack			mMatStack;

};

ceEND
