/*
	Celestial Engine
	Created by SerialVelocity
*/
#include "celestialBase.h"

ceBEGIN

SceneNode::SceneNode()
{
	mParent = 0;

	mPosition = Vector3f();
	mRotation = QuatIdentity();
	mScale = Vector3f();
	mWorld = MatIdentity();
	mComputedAbsTransform = false;
}

SceneNode::~SceneNode()
{
}

void SceneNode::setParent( SceneNode *node )
{
	// Detach this from the current parent
	if( mParent )
		mParent->detach( this );

	// Attach this to the new parent
	if( node )
		node->attach( this );
	else
		mParent = 0;
}

SceneNode *SceneNode::getParent()
{
	return mParent;
}

void SceneNode::attach( SceneNode *node )
{
	ceASSERT( node );

	// Add the node to the list
	mChildren.push_back( node );
	node->mParent = this;
}

void SceneNode::detach( SceneNode *node )
{
	ceASSERT( node );
	ceASSERT( !mChildren.empty() );

	// Remoce the node to the list
	node->mParent = 0;
	mChildren.remove( node );
}

 void SceneNode::setPosition( Vector3f position )
{
	mPosition = position;
}

Vector3f &SceneNode::getPosition()
{
	return mPosition;
}

void SceneNode::setRotation( Quaternion rotation )
{
	mRotation = rotation;
}

Quaternion &SceneNode::getRotation()
{
	return mRotation;
}

void SceneNode::setScale( Vector3f scale )
{
	mScale = scale;
}

Vector3f &SceneNode::getScale()
{
	return mScale;
}

Vector3f SceneNode::getAbsPosition()
{
	return Vector3f( mWorld.m30, mWorld.m31, mWorld.m32 );
}

void SceneNode::updateAbsPosition()
{
	if( mComputedAbsTransform )
		return;
	mComputedAbsTransform = true;

	// Multiply local matrix by parents world matrix
	if( mParent )
		mWorld = ( MatScale( mScale ) * MatRotate( mRotation ) * MatTranslate( mPosition ) ) * mParent->mWorld;
	else
		mWorld = MatScale( mScale ) * MatRotate( mRotation ) * MatTranslate( mPosition );

	// Compute child absolute positions
	for( SceneNodeList::iterator i = mChildren.begin(); i != mChildren.end(); i++ )
		(*i)->updateAbsPosition();
}

void SceneNode::setMaterial( Material &material )
{
	mMaterial = material;
}

Material &SceneNode::getMaterial()
{
	return mMaterial;
}

void SceneNode::preRender()
{
}

void SceneNode::render()
{
}

void SceneNode::renderChildren()
{
	for( SceneNodeList::iterator i = mChildren.begin(); i != mChildren.end(); i++ )
	{
		SceneNode *sn = (*i);
		sn->preRender();
		sn->render();
		sn->renderChildren();
		sn->postRender();
	}
}

void SceneNode::postRender()
{
	mComputedAbsTransform = false;
}

ceEND
