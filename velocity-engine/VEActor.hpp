/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

VE_BEGIN

// Actor Control States
enum ActorControlStates
{
	ACS_NONE,
	ACS_HUMAN,
	ACS_AI,
	ACS_REMOTE
};

// Human Actor Control Scheme
struct ActorControlScheme
{
	int yawRight;
	int yawLeft;
	int pitchUp;
	int pitchDown;
	int rollRight;
	int rollLeft;
	int engine;
	bool mouse;
};

// Actor
class Actor
{
public:
						Actor();
						~Actor();

	// Loading
	void				loadDefault( D3DXVECTOR3 position );

	// Updating
	void				onUpdate();

	// Control
	void				setControlState( int ctrlState );

	// Human
	void				setHumanControlScheme( ActorControlScheme &ctrlScheme );
	ActorControlScheme	&getHumanControlScheme();

	// AI

	// Remote

	// Accessors
	D3DXMATRIX			&getPosition();
	D3DXMATRIX			&getRotation();
	D3DXMATRIX			getWorld();

private:
	// Mesh Node
	MeshNode			*mMesh;

	// Orientation and Position
	D3DXVECTOR3			mRotationSpeed;
	D3DXVECTOR3			mAxisRight;
	D3DXVECTOR3			mAxisUp;
	D3DXVECTOR3			mAxisLook;
	D3DXMATRIX			mPosition;
	D3DXMATRIX			mRotation;
	D3DXVECTOR3			mVelocity;

	// Control
	int					mCtrlState;

	// Human
	ActorControlScheme	mCtrlScheme;

	// AI

	// Remote

};

// Actor List
typedef std::list<Actor*> ActorList;

VE_END
