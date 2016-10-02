/*
	Velocity Engine
	By SerialVelocity
*/
#include "VEBase.hpp"
using namespace VE;

// Event Dispatcher
EventDispatcher::EventDispatcher()
{
}

EventDispatcher::~EventDispatcher()
{
	// Clear both lists
	mQueue.clear();
	mListenerList.clear();
}

EventDispatcher *EventDispatcher::getInstance()
{
	// Create an object which is automatically deleted on exit and return
	static EventDispatcher instance;
	return &instance;
}

void EventDispatcher::trigger( int eventType, void *eventData )
{
	// Dont trigger if the event listener list is empty or the event queue is empty
	if( mListenerList.empty() ) return;

	// Create the event
	Event ev( eventType );
	if( eventData != 0 )
		ev.setData( eventData );

	// Trigger the event immediately
	for( EventListenerList::iterator i = mListenerList.begin(); i != mListenerList.end(); i++ )
	{
		(*i)->handleEvent( ev );
	}
}

void EventDispatcher::queue( int eventType, void *eventData )
{
	// Create the event
	Event ev( eventType );
	if( eventData != 0 )
		ev.setData( eventData );

	// Add the event to the list
	mQueue.push_back( ev );
}

void EventDispatcher::addListener( EventListener *listener )
{
	// Add a listener to the list
	mListenerList.push_back( listener );
}

void EventDispatcher::tick()
{
	// Dont update if the event listener list is empty or the event queue is empty
	if( mListenerList.empty() || mQueue.empty() ) return;

	// Iterate through the list and trigger each event
	for( EventListenerList::iterator i = mListenerList.begin(); i != mListenerList.end(); i++ )
	{
		for( EventQueue::iterator j = mQueue.begin(); j != mQueue.end(); j++ )
		{
			(*i)->handleEvent( (*j) );
		}
	}

	// Clear the event queue
	mQueue.clear();
}
