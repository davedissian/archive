/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

// FMOD
#include <fmod.hpp>
#pragma comment( lib, "fmodex_vc.lib" )

VE_BEGIN

// Sound
class Sound
{
public:
						Sound();
						~Sound();

	// Loading
	void				load( FMOD::System *system, const char *filename );

	// Manipulation
	void				play( bool looped );
	void				togglePause();
	void				stop();
	void				setVolume( float scalar );

private:
	// Flags
	bool				mPaused;

	// System
	FMOD::System		*mSystem;

	// Handle
	FMOD::Sound			*mSound;
	FMOD::Channel		*mChannel;

};

// Audio Manager
class AudioManager : public EventListener
{
public:
						AudioManager();
						~AudioManager();

	// Setup
	void				setup();

	// Updating
	void				tick();
	void				handleEvent( Event &ev );

	// Accessors
	FMOD::System		*getSystem();

private:
	// FMOD
	FMOD::System		*mSystem;

};

VE_END
