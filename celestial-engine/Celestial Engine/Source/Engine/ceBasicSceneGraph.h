/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

ceBEGIN

class BasicSceneGraph : public SceneGraph
{
public:
						BasicSceneGraph( Game *parent );
						~BasicSceneGraph();

	// Attach/Detach
	void				attach( SceneNode &node );
	void				detach( SceneNode &node );

	// Drawing
	void				draw();

private:
	// Node List
	SceneNodeList		mNodes;

};

ceEND
