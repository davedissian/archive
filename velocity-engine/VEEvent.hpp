/*
	Transcendent
	By SerialVelocity
*/
#pragma once

VE_BEGIN

// Event Types
enum EventType
{
	// Core						Data:
	ET_TICK,					// NONE

	// Message Boxes
	ET_MESSAGEBOX_QUIT,			// NONE

	// Engine Events
	ET_WINDOW_RESIZE,			// NONE
	ET_D3D_DEVICE_RESET,		// Ptr to device

	// Scene Graph
	ET_NEW_SCENE_NODE,			// Ptr to new node

	// Actors
	ET_NEW_ACTOR,				// Ptr to new actor
	ET_DEAD_ACTOR,				// Ptr to dead actor

	// Bullets
	ET_SPAWN_BULLET,			// Ptr to parent actor
	ET_SPAWN_MISSILE,			// Ptr to parent actor
};

// Event Data to String
std::string EventTypeToString( int eventType );

// Event Data
class Event
{
public:
						Event( int eventType );

	// Data Managment
	void				setData( void *data );

	// Accessors
	int					getType();
	void				*getData();

private:
	// Event type
	int					mType;

	// Data Pointer
	void				*mData;

};

// Event Listener
class EventListener
{
public:
	// Event Handling
	virtual void		handleEvent( Event & ) = 0;

};

// Event Snooper
class EventSnooper : public EventListener
{
public:
						EventSnooper();

	// Event Handling
	void				handleEvent( Event &ev );

};

VE_END
