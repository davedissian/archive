/*
	Velocity Engine
	By SerialVelocity
*/
#include "VE.hpp"
using namespace VE;

// Scene Node
SceneNode::SceneNode( bool addToSceneGraph, int nodeType )
{
	// Set all members to default values
	mReady = false;
	mSceneGraph = gEngine->getGraphics()->getSceneGraph();
	mNodeType = nodeType;

	// Trigger an event to add this to the scene graph
	if( addToSceneGraph )
		gEngine->postEvent( ET_NEW_SCENE_NODE, EP_IMMEDIATE, (void*)this );
}

void SceneNode::setLocalTransform( D3DXMATRIX *transform )
{
	// Update the node transformation
	mLocalTransform = *transform;
}

void SceneNode::computeGlobalTransform()
{
	// Add transform to the stack
	mSceneGraph->pushMatrix( &mLocalTransform );

	// Set the global transform
	mGlobalTransform = mSceneGraph->getTop();

	// Iterate through each child
	for( NodeList::iterator i = mChildren.begin(); i != mChildren.end(); i++ )
	{
		// Compute the global transform
		SceneNode *node = *i;
		node->computeGlobalTransform();
	}
	
	// Remove transform from the stack
	mSceneGraph->popMatrix();
}

void SceneNode::addChild( SceneNode *node )
{
	// Add a child
	mChildren.push_back( node );
}

void SceneNode::removeChild( SceneNode *node )
{
	// Remove the child specified
	mChildren.remove( node );
}

void SceneNode::onPreRender()
{
	// Update world matrix
	gEngine->getGraphics()->setWorld( &mGlobalTransform );
}

void SceneNode::onRender()
{
	// Render code here
}

void SceneNode::onRenderChildren()
{
	// Iterate through each child
	for( NodeList::iterator i = mChildren.begin(); i != mChildren.end(); i++ )
	{
		// Render the node
		SceneNode *node = *i;
		node->onPreRender();
		node->onRender();
		node->onRenderChildren();
		node->onPostRender();
	}
}

void SceneNode::onPostRender()
{
}

bool SceneNode::isReady()
{
	// Return the flag stating whether the node is ready
	return mReady;
}

SceneNode *SceneNode::getParent()
{
	// Return the parent
	return mParent;
}

D3DXMATRIX &SceneNode::getLocalTransform()
{
	// Return the local transform matrix
	return mLocalTransform;
}

D3DXMATRIX &SceneNode::getGlobalTransform()
{
	// Return the local transform matrix
	return mGlobalTransform;
}

int SceneNode::getType()
{
	// Return the type
	return mNodeType;
}

// Mesh Node
MeshNode::MeshNode( bool addToSceneGraph, int nodeType ) : SceneNode( addToSceneGraph, nodeType )
{
	// Set all members to default values
	mMesh = 0;
	mEffect = 0;
	D3DXMatrixIdentity( &mPrevWVP );
}

MeshNode::~MeshNode()
{
	// Release the effect
	SAFE_RELEASE( mEffect );

	// Release the mesh
	SAFE_RELEASE( mMesh );
}

void MeshNode::load( const char *meshFilename, const char *effectFilename )
{
	// Load the mesh
	std::string temp;
	temp = VE_DIR_MODELS;
	temp += meshFilename;
	Resource *meshResource = gResCache->load( temp.c_str() );
	HRESULT hr = D3DXLoadMeshFromXInMemory( meshResource->data(), meshResource->size(), D3DXMESH_SYSTEMMEM, gEngine->getGraphics()->getDevice(), 0, 0, 0, 0, &mMesh );
	if( FAILED( hr ) )
	{
		LOGWRITE( "Failed to load " << meshFilename );
		return;
	}

	// Load and compile the effect
	ID3DXBuffer *errors;
	temp = VE_DIR_SHADERS;
	temp += effectFilename;
	Resource *effectResource = gResCache->load( temp.c_str() );
	D3DXCreateEffect( gEngine->getGraphics()->getDevice(), effectResource->data(), effectResource->size(), 0, 0, 0, 0, &mEffect, &errors );
	if( errors )
	{
		LOGWRITE( "Shader " << effectFilename << " failed to compile." );
		LOGWRITE( static_cast<char*>( errors->GetBufferPointer() ) );
		SAFE_RELEASE( mMesh );
		return;
	}

	// Find the best technique
	D3DXHANDLE hTech;
	mEffect->FindNextValidTechnique( 0, &hTech );
	mEffect->SetTechnique( hTech );

	// Ready
	mReady = true;
}

void MeshNode::onRender()
{
	// If the mesh isnt ready, return
	if( mReady == false ) return;
	
	// Update parameters
	gEngine->getGraphics()->passSemanticValues( mEffect );
	D3DXHANDLE handle = mEffect->GetParameterBySemantic( 0, "PREVWORLDVIEWPROJECTION" );
	if( handle ) mEffect->SetMatrix( handle, &mPrevWVP );

	// Render each pass
	UINT numPasses;
	mEffect->Begin( &numPasses, 0 );
	for( unsigned int i = 0; i < numPasses; i++ ) 
	{
		mEffect->BeginPass( i );
			mMesh->DrawSubset( 0 );
		mEffect->EndPass();
	}
	mEffect->End();

	// Update Prev WVP
	mPrevWVP = gEngine->getGraphics()->getWorld() * gEngine->getGraphics()->getView() * gEngine->getGraphics()->getProjection();
}

ID3DXMesh *MeshNode::getMesh()
{
	// Return the mesh
	return mMesh;
}

ID3DXEffect *MeshNode::getEffect()
{
	// Return the effect
	return mEffect;
}

// Sprite Node

// Camera Node
Camera::Camera() : SceneNode( false, SNT_SOLID )
{
	// Set all members to default values
	mSceneGraph = gEngine->getGraphics()->getSceneGraph();
	mFOV = 60.0f;
	mAspect = 1.0f;
	mZNear = 1.0f;
	mZFar = 1000.0f;
}

Camera::~Camera()
{
}

void Camera::setup( float FOV, float aspect, float zNear, float zFar )
{
	// Setup camera properties
	mFOV = FOV;
	mAspect = aspect;
	mZNear = zNear;
	mZFar = zFar;
}

void Camera::onRender()
{
}

void Camera::updateView()
{
	// Generate view matrix
	D3DXMATRIX view;
	D3DXMatrixInverse( &view, 0, &mLocalTransform );
	gEngine->getGraphics()->setView( &view );
}

void Camera::updateProjection()
{
	// Generate projection matrix
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH( &projection, D3DXToRadian( mFOV ), mAspect, mZNear, mZFar );
	gEngine->getGraphics()->setProjection( &projection );
}

// Scene Graph Base
SceneGraph::SceneGraph()
{
	// Create the matrix stack
	D3DXCreateMatrixStack( 0, &mMatStack );
	mMatStack->LoadIdentity();

	// Set all members to default values
	mMainCamera = mDebugCamera = 0;
}

SceneGraph::~SceneGraph()
{
	// Release the matrix stack
	SAFE_RELEASE( mMatStack );
}

void SceneGraph::addSceneNode( SceneNode *node )
{
	// Add the node
	mNodes.push_back( node );
}

void SceneGraph::removeSceneNode( SceneNode *node )
{
	// Remove the node to the scene graph
	mNodes.remove( node );
}

void SceneGraph::setCameras( Camera *mainCamera, Camera *debugCamera )
{
	// Update the cameras
	mainCamera->updateProjection();
	debugCamera->updateProjection();
	mMainCamera = mainCamera;
	mDebugCamera = debugCamera;
}

void SceneGraph::toggleDebugCamera()
{
}

Camera *SceneGraph::getCamera()
{
	// Return the current main camera
	return mMainCamera;
}

void SceneGraph::pushMatrix( D3DXMATRIX *matrix )
{
	// Push a new matrix on the stack and multiply by the argument
	mMatStack->Push();
	mMatStack->MultMatrixLocal( matrix );
}

void SceneGraph::popMatrix()
{
	// Pop the top matrix from the stack
	mMatStack->Pop();
}

D3DXMATRIX SceneGraph::getTop()
{
	// Return the current top matrix
	return *( mMatStack->GetTop() );
}

void SceneGraph::computeGlobalTransforms()
{
	// Iterate through each child
	for( NodeList::iterator i = mNodes.begin(); i != mNodes.end(); i++ )
	{
		// Compute the global transform
		SceneNode *node = *i;
		node->computeGlobalTransform();
	}
}

void SceneGraph::renderScene()
{
	// If the node list is empty then return
	if( mNodes.empty() )
		return;

	// Iterate and render each node
	for( NodeList::iterator i = mNodes.begin(); i != mNodes.end(); i++ )
	{
		// Render the node
		SceneNode *node = *i;
		node->onPreRender();
		node->onRender();
		node->onRenderChildren();
		node->onPostRender();
	}
}