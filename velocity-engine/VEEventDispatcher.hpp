/*
	Transcendent
	By SerialVelocity
*/
#pragma once

#include "VEEvent.hpp"

VE_BEGIN

// Event Dispatcher
class EventDispatcher
{
						EventDispatcher();
						~EventDispatcher();
public:
	// Instance
	static EventDispatcher *getInstance();

	// Dispatching
	void				trigger( int eventType, void *eventData = 0 );
	void				queue( int eventType, void *eventData = 0 );

	// Listeners
	void				addListener( EventListener *listener );

	// Updating
	void				tick();

private:
	// Event List
	typedef std::list<Event> EventQueue;
	EventQueue			mQueue;

	// Listeners
	typedef std::list<EventListener*> EventListenerList;
	EventListenerList	mListenerList;

};

VE_END
