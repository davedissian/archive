/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once
#include "ceSceneGraph.h"

ceBEGIN

class RenderTarget {};

// Renderer Subsystem Base
class Renderer
{
public:
	Renderer( Game *parent ) { mGame = parent; mSG = 0; }

	// Startup
	virtual void		startup( SceneGraph *sg ) { mSG = sg; }
	
	// Updating
	virtual bool		update() = 0;
	
	// Rendering
	virtual void		rBegin( Vector3f colour ) = 0;
	virtual void		rBegin( Vector3f colour, RenderTarget rt ) = 0;
	virtual void		rEnd() = 0;
	virtual void		rPresent() = 0;

	// Scene graph
	virtual SceneGraph	*getSceneGraph() { return 0; }

protected:
	Game				*mGame;

	// Scene Graph
	SceneGraph			*mSG;

};

ceEND
