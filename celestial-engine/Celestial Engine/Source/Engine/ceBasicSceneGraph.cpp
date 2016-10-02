/*
	Celestial Engine
	Created by SerialVelocity
*/
#include "celestialBase.h"
#include "ceBasicSceneGraph.h"

ceBEGIN

BasicSceneGraph::BasicSceneGraph( Game *parent ) : SceneGraph( parent )
{
}

BasicSceneGraph::~BasicSceneGraph()
{
	mNodes.clear();
}

void BasicSceneGraph::attach( SceneNode &node )
{
	mNodes.push_back( &node );
}

void BasicSceneGraph::detach( SceneNode &node )
{
	mNodes.remove( &node );
}

void BasicSceneGraph::draw()
{
	for( SceneNodeList::iterator i = mNodes.begin(); i != mNodes.end(); i++ )
	{
		SceneNode *sn = (*i);
		sn->preRender();
		sn->render();
		sn->renderChildren();
		sn->postRender();
	}
}

ceEND
