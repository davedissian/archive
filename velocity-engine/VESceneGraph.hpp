/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

VE_BEGIN

// Forward Declarations
class Texture;
class Font;
class SceneNode;
class SceneGraph;

// Common Typedefs
typedef std::list<SceneNode*> NodeList;

// Scene Node Types
enum SceneNodeType
{
	SNT_SOLID,
	SNT_ALPHA
};

// Base Scene Node
class SceneNode
{
public:
						SceneNode( bool addToSceneGraph, int nodeType );

	// Modifiers
	virtual void		setLocalTransform( D3DXMATRIX *transform );
	void				computeGlobalTransform();

	// Child Nodes
	virtual void		addChild( SceneNode *node );
	virtual void		removeChild( SceneNode *node );

	// Rendering
	virtual void		onPreRender();
	virtual void		onRender();
	virtual void		onRenderChildren();
	virtual void		onPostRender();

	// Accessors
	bool				isReady();
	SceneNode			*getParent();
	D3DXMATRIX			&getLocalTransform();
	D3DXMATRIX			&getGlobalTransform();
	int					getType();

protected:
	// Flags
	bool				mReady;

	// Scene Graph
	SceneNode			*mParent;
	SceneGraph			*mSceneGraph;

	// Children
	NodeList			mChildren;

	// Node Properties
	D3DXMATRIX			mLocalTransform, mGlobalTransform;
	int					mNodeType;

};

// Mesh Node
class MeshNode : public SceneNode
{
public:
						MeshNode( bool addToSceneGraph, int nodeType );
						~MeshNode();

	// Loading
	void				load( const char *meshFilename, const char *effectFilename );

	// Updating
	void				onRender();

	// Accessors
	ID3DXMesh			*getMesh();
	ID3DXEffect			*getEffect();

private:
	// Mesh
	ID3DXMesh			*mMesh;

	// Effect
	ID3DXEffect			*mEffect;
	D3DXMATRIX			mCurrentWVP, mPrevWVP;

};

// Sprite Node

// Camera Node
class Camera : public SceneNode
{
public:
						Camera();
						~Camera();

	// Creation
	void				setup( float FOV, float aspect, float zNear, float zFar );

	// Rendering
	void				onRender();

	// Updating
	void				updateView();
	void				updateProjection();

private:
	// Scene Manager
	SceneGraph			*mSceneGraph;

	// Camera Properties
	D3DXVECTOR3			mAxisRight, mAxisUp, mAxisLook;
	float				mFOV, mAspect, mZNear, mZFar;
	D3DXMATRIX			mProjection;

};

// Scene Graph Base
class SceneGraph
{
	friend class		Graphics;

public:
						SceneGraph();
						~SceneGraph();

	// Main Nodes
	void				addSceneNode( SceneNode *node );
	void				removeSceneNode( SceneNode *node );

	// Camera
	void				setCameras( Camera *mainCamera, Camera *debugCamera );
	void				toggleDebugCamera();
	Camera				*getCamera(); // Return the rendering camera (debug if in debug mode)

	// Sprites

	// Operations
	void				pushMatrix( D3DXMATRIX *matrix );
	void				popMatrix();
	D3DXMATRIX			getTop();
	void				computeGlobalTransforms();
	void				renderScene();

	// Accessors

private:
	// Scene Graph
	NodeList			mNodes;
	ID3DXMatrixStack	*mMatStack;
	Camera				*mMainCamera, *mDebugCamera;

};

VE_END
