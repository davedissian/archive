/*
	Velocity Engine
	By SerialVelocity
*/
#include "VEBase.hpp"
using namespace VE;

// Macros
#define CHECK_TYPE( x ) case x: return #x; break

// Event Type to String
std::string VE::EventTypeToString( int eventType )
{
	switch( eventType )
	{
		// Core
		CHECK_TYPE( ET_TICK );

		// Message Boxes
		CHECK_TYPE( ET_MESSAGEBOX_QUIT );

		// Engine States
		CHECK_TYPE( ET_WINDOW_RESIZE );
		CHECK_TYPE( ET_D3D_DEVICE_RESET );

		// Scene Graph
		CHECK_TYPE( ET_NEW_SCENE_NODE );

		// Actors
		CHECK_TYPE( ET_NEW_ACTOR );
		CHECK_TYPE( ET_DEAD_ACTOR );

		// Bullets
		CHECK_TYPE( ET_SPAWN_BULLET );
		CHECK_TYPE( ET_SPAWN_MISSILE );

		// Unknown type
		default:
			return "UNKNOWN";
	}
}

// Event Data
Event::Event( int eventType )
{
	// Set all members to default values
	mType = eventType;
	mData = 0;
}

void Event::setData( void *data )
{
	// Set the internal data pointer
	mData = data;
}

int Event::getType()
{
	// Return the event type
	return mType;
}

void *Event::getData()
{
	// Return the internal data
	return mData;
}

// Snooper
EventSnooper::EventSnooper()
{
	LOGWRITE( "Event Snooper: Ignoring ET_TICK." );
}

void EventSnooper::handleEvent( Event &ev )
{
	// Output event data to cout
	if( ev.getType() == ET_TICK ) return;
	LOGWRITE( "Event Snooper: Received " << EventTypeToString( ev.getType() ) << "." );
}
