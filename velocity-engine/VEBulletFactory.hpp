/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

// Bullets
#include "VEBullet.hpp"

VE_BEGIN

// Bullet Factory
// The bullet factory is basically a variant of a particle emitter
class BulletFactory : public EventListener
{
public:
						BulletFactory();
						~BulletFactory();

	// Updating
	void				tick();
	void				render();
	void				handleEvent( Event &ev );

private:
	// Bullet Lists

	// Laser Bolt Point List
	IDirect3DVertexBuffer9 *mLaserBoltList;

};

VE_END
