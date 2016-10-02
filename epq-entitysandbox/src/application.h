// EPQ Project
#pragma once

// Forward declarations
class Agent;
class ItemManager;

enum MouseButtonState
{
	MBS_LEFT,
	MBS_RIGHT,
	MBS_MIDDLE
};

class App : public IEventReceiver
{
public:
	App();
	~App();

	s32 go( s32 argc, char** argv );

	void setup();
	void mainLoop();
	void shutdown();

	bool isKeyDown( EKEY_CODE keyCode ) const;
	bool isMouseButtonDown( MouseButtonState mouseButton ) const;
	const vector2df& getMousePosition() const;

	virtual bool OnEvent( const SEvent& e );

private:
	bool mIsRunning;

	IGUIStaticText* mDiagnostic;

	ICameraSceneNode* mCamera;
	vector2df mCameraPos;
	f32 mCameraHeight;
	f32 mCameraTargetHeight;
	f32 mCameraOffset;

	Agent* mAgent;
	ItemManager* mItemMgr;

	// For camera dragging
	bool mIsDragging;
	vector2df mStartMousePosition;
	vector2df mStartCameraPos;

	bool mKeyStates[KEY_KEY_CODES_COUNT];
	bool mMouseButtonStates[3];
	vector2df mMousePosition;

};
