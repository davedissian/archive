/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

ceBEGIN

struct Material
{
	/*
	Texture				*texture[8];
	Shader				*shader;
	*/
};

typedef	list<class SceneNode *> SceneNodeList;
class SceneNode
{
public:
						SceneNode();
						~SceneNode();

	// Relationships
	virtual void		setParent( SceneNode *node );
	SceneNode			*getParent();
	virtual void		attach( SceneNode *node );
	virtual void		detach( SceneNode *node );

	// Transforms
	virtual void		setPosition( Vector3f position );
	Vector3f			&getPosition();
	virtual void		setRotation( Quaternion rotation );
	Quaternion			&getRotation();
	virtual void		setScale( Vector3f scale );
	Vector3f			&getScale();
	
	Vector3f			getAbsPosition();
	virtual void		updateAbsPosition();

	// Material
	void				setMaterial( Material &material );
	Material			&getMaterial();

	// Rendering
	virtual void		preRender();
	virtual void		render();
	virtual void		renderChildren();
	virtual void		postRender();

	// Accessors
	virtual const char	*getType() { return "SceneNode"; }

protected:
	Material			mMaterial;

	// Node relationships
	SceneNode			*mParent;
	SceneNodeList		mChildren;

	// Transforms
	Vector3f			mPosition;
	Quaternion			mRotation;
	Vector3f			mScale;
	Matrix				mWorld;
	bool				mComputedAbsTransform;
};

ceEND
