/*
	Velocity Engine
	By SerialVelocity
*/
#include "VEBase.hpp"
#include "VEAudio.hpp"
using namespace VE;

// Sound
Sound::Sound()
{
	// Set all members to default values
	mPaused = false;

	mSystem = 0;

	mSound = 0;
	mChannel = 0;
}

Sound::~Sound()
{
	// Release the sound
	if( mSound )
		mSound->release();
}

void Sound::load( FMOD::System *system, const char *filename )
{
	// Update the internal system pointer
	mSystem = system;

	// Load the sound into memory
	mSystem->createSound( filename, FMOD_SOFTWARE, 0, &mSound );
	LOGWRITE( "Loaded sound " << filename );
}

void Sound::play( bool looped )
{
	// Update the channel
	mSystem->playSound( FMOD_CHANNEL_FREE, mSound, false, &mChannel );
	mChannel->setMode( looped ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF );
}

void Sound::togglePause()
{
	// Toggle the pause state
	mPaused = !mPaused;
	mChannel->setPaused( mPaused );
}

void Sound::stop()
{
	// Stop playing
	mChannel->stop();
}

void Sound::setVolume( float scalar )
{
	// Adjust the volume
	mChannel->setVolume( scalar );
}

// Audio Manager
AudioManager::AudioManager()
{
	// Set all members to default values
	mSystem = 0;
}

AudioManager::~AudioManager()
{
	// Shutdown FMOD
	if( mSystem )
	{
		mSystem->close();
		mSystem->release();
	}
}

void AudioManager::setup()
{
	// Initialise FMOD
	FMOD::System_Create( &mSystem );
	mSystem->init( 100, FMOD_INIT_NORMAL, 0 );
}

void AudioManager::tick()
{
	// Update FMOD
	mSystem->update();
}

void AudioManager::handleEvent( Event &ev )
{
	// Act on the event type
	int type = ev.getType();
	
	// Tick
	if( type == ET_TICK )
		tick();
}

FMOD::System *AudioManager::getSystem()
{
	// Return the FMOD system
	return mSystem;
}
