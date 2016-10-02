/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

VE_BEGIN

// Base Screen
class Screen
{
public:
	// Setup
	virtual void		onPush() = 0; // Resource Loading

	// Updating
	virtual void		onUpdate() = 0; // Update (Movement/Physics/Collision)
	virtual void		onRender() = 0; // Render
	virtual void		onReset() = 0; // Reset (lost device)

	// Shutdown
	virtual void		onPop() = 0; // Resource Releasing

private:

};

VE_END
