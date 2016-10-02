/*
	Velocity Engine
	By SerialVelocity
*/
#include "VE.hpp"
using namespace VE;

// Fighter Actor
Actor::Actor()
{
	// Set all members to default values
	mMesh = 0;
	mRotationSpeed = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	mAxisRight = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	mAxisUp = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	mAxisLook = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	D3DXMatrixIdentity( &mPosition );
	D3DXMatrixIdentity( &mRotation );
	mVelocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	mCtrlState = ACS_NONE;

	// Setup default control scheme
	ZeroMemory( &mCtrlScheme, sizeof( ActorControlScheme ) );
	mCtrlScheme.yawRight = DIK_RIGHT;
	mCtrlScheme.yawLeft = DIK_LEFT;
	mCtrlScheme.pitchUp = DIK_UP;
	mCtrlScheme.pitchDown = DIK_DOWN;
	mCtrlScheme.rollRight = DIK_PERIOD;
	mCtrlScheme.rollLeft = DIK_COMMA;
	mCtrlScheme.engine = DIK_SPACE;
	mCtrlScheme.mouse = true;

	// Add self to the actor lists
	gEngine->postEvent( ET_NEW_ACTOR, EP_IMMEDIATE, (void*)this );
}

Actor::~Actor()
{
	// Delete the mesh node
	SAFE_DELETE( mMesh );
}

void Actor::loadDefault( D3DXVECTOR3 position )
{
	// Load the default ship
	mMesh = new MeshNode( true, SNT_SOLID );
	mMesh->load( "fighter.x", "basicObject.fx" );

	// Set the position
	D3DXMatrixTranslation( &mPosition, position.x, position.y, position.z );
	mMesh->setLocalTransform( &mPosition );
}

void Actor::onUpdate()
{
	// Control the actor based on the control state
	switch( mCtrlState )
	{
		// Human Actor Control
		case ACS_HUMAN:
		{
			InputManager *i = Engine::getInstance()->getInputManager();

			// Pitch/Yaw
			if( mCtrlScheme.mouse )
			{
				// Use the movement of the mouse for pitch and yaw
				D3DXVECTOR2 MM( (float)i->mouseMove().x / 100.0f, (float)i->mouseMove().y / 100.0f );
				mRotationSpeed.x += MM.x;
				mRotationSpeed.y += MM.y;
			}
			else
			{
				if( i->keyDown( mCtrlScheme.yawRight ) )
				{
					// Yaw Right
					mRotationSpeed.x += 0.05f;
				}
				if( i->keyDown( mCtrlScheme.yawLeft ) )
				{
					// Yaw Left
					mRotationSpeed.x -= 0.05f;
				}
				if( i->keyDown( mCtrlScheme.pitchUp ) )
				{
					// Pitch Up
					mRotationSpeed.y -= 0.05f;
				}
				if( i->keyDown( mCtrlScheme.pitchDown ) )
				{
					// Pitch Down
					mRotationSpeed.y += 0.05f;
				}
			}

			// Roll
			if( i->keyDown( mCtrlScheme.rollRight ) )
			{
				// Roll Right
				mRotationSpeed.z -= 0.05f;
			}
			if( i->keyDown( mCtrlScheme.rollLeft ) )
			{
				// Roll Left
				mRotationSpeed.z += 0.05f;
			}

			// Use the mapped controls to move the ship too
			if( i->keyDown( mCtrlScheme.engine ) )
			{
				// Move the engine forward
				mVelocity += mAxisLook * 0.05f;
			}
		}
		break;

		// AI Actor Control
		case ACS_AI:
		{
			// AI CODE HERE
		}
		break;

		// Remote (Multiplayer) Actor Control
		case ACS_REMOTE:
		{
			// PACKET PARSING CODE
		}
		break;

		// This code is can never executed, used to suppress warning from compiler
		default:
			break;
	}

	// Update rotation
	D3DXMATRIX rotation; D3DXQUATERNION upRot, rightRot, lookRot;
	D3DXQuaternionRotationAxis( &upRot, &mAxisUp, D3DXToRadian( mRotationSpeed.x ) );
	D3DXQuaternionRotationAxis( &rightRot, &mAxisRight, D3DXToRadian( mRotationSpeed.y ) );
	D3DXQuaternionRotationAxis( &lookRot, &mAxisLook, D3DXToRadian( mRotationSpeed.z ) );
	D3DXMatrixRotationQuaternion( &rotation, &( upRot * rightRot * lookRot ) );
	mRotation *= rotation;

	// Slow down rotation speed
	mRotationSpeed *= 0.95f;

	// Transform each axis
	D3DXVECTOR4 newAxisRight, newAxisUp, newAxisLook;
	D3DXVec3Transform( &newAxisRight, &mAxisRight, &rotation );
	mAxisRight.x = newAxisRight.x; mAxisRight.y = newAxisRight.y; mAxisRight.z = newAxisRight.z;
	D3DXVec3Transform( &newAxisUp, &mAxisUp, &rotation );
	mAxisUp.x = newAxisUp.x; mAxisUp.y = newAxisUp.y; mAxisUp.z = newAxisUp.z;
	D3DXVec3Transform( &newAxisLook, &mAxisLook, &rotation );
	mAxisLook.x = newAxisLook.x; mAxisLook.y = newAxisLook.y; mAxisLook.z = newAxisLook.z;

	// Update the position
	D3DXMATRIX velocity;
	D3DXMatrixTranslation( &velocity, mVelocity.x, mVelocity.y, mVelocity.z );
	mPosition *= velocity;

	// Update the transform matrix
	mMesh->setLocalTransform( &( mRotation * mPosition ) );
}

void Actor::setControlState( int ctrlState )
{
	// Update the control state
	mCtrlState = ctrlState;
}

void Actor::setHumanControlScheme( ActorControlScheme &ctrlScheme )
{
	// Update the human actor key settings
	mCtrlScheme = ctrlScheme;
}

ActorControlScheme &Actor::getHumanControlScheme()
{
	// Return the control scheme
	return mCtrlScheme;
}

D3DXMATRIX &Actor::getPosition()
{
	// Return the position transform
	return mPosition;
}

D3DXMATRIX &Actor::getRotation()
{
	// Return the rotation transform
	return mRotation;
}

D3DXMATRIX Actor::getWorld()
{
	// Generate world transform
	return mRotation * mPosition;
}