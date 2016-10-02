/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

VE_BEGIN

// Event Dispatch Flags
enum EventDispatchFlags
{
	EP_IMMEDIATE,
	EP_QUEUE,
};

// Engine Layer
class Engine : public EventListener
{
						Engine();
						~Engine();

public:
	// Instance
	static Engine		*getInstance();

	// Setup
	void				setup();

	// Updating
	void				setScreen( Screen *screen );
	bool				run();

	// Actors
	void				addActor( Actor *actor );
	void				removeActor( Actor *actor );
	void				updateActors();

	// Events
	void				handleEvent( Event &ev );
	void				postEvent( int eventType, int eventDispatchFlag = EP_QUEUE, void *eventData = 0 );

	// Shutdown
	void				shutdown();

	// Accessors
	unsigned int		getClientWidth();
	unsigned int		getClientHeight();
	HWND				getWnd();
	WNDCLASSEX			&getWndClass();
	Graphics			*getGraphics();
	AudioManager		*getAudioManager();
	InputManager		*getInputManager();
	LuaState			*getLuaState();
	Camera				*getCamera();

private:
	// Flags
	bool				mQuitFlag;

	// Win32
	HWND				mWnd;
	WNDCLASSEX			mWndClass;
	unsigned int		mClientWidth;
	unsigned int		mClientHeight;

	// Actor List
	ActorList			mActorList;

	// Subsystems
	Screen				*mNewScreen;
	EventSnooper		*mEventSnooper;
	Graphics			*mGraphics;
	AudioManager		*mAudioManager;
	InputManager		*mInputManager;
	LuaState			*mLuaState;

	// Cameras
	Camera				*mMainCamera;
	Camera				*mDebugCamera;

};

VE_END
