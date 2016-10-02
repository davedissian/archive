// EPQ Project
#include "main.h"
#include "application.h"
#include "agent.h"
#include "itemManager.h"

App::App() :
	mCameraPos( 0.0f, 0.0f ),
	mDiagnostic( nullptr ),
	mCamera( nullptr ),
	mCameraHeight( 120.0f ),
	mCameraTargetHeight( mCameraHeight ),
	mCameraOffset( 0.0f ),
	mAgent( nullptr ),
	mItemMgr( nullptr ),
	mIsDragging( false )
{
	mIsRunning = true;
	for( s32 i = 0; i < KEY_KEY_CODES_COUNT; i++ )
		mKeyStates[i] = false;
	for( s32 i = 0; i < 3; i++ )
		mMouseButtonStates[i] = false;
}

App::~App()
{
	shutdown();
}

s32 App::go( s32 argc, char** argv )
{
	setup();
	mainLoop();
	return EXIT_SUCCESS;
}

void App::setup()
{
	// Create the device and initialise irrlicht
	cxt.device = createDevice( EDT_OPENGL, dimension2d<u32>( 800, 600 ), 32U, false, false, true, this );
	cxt.driver = cxt.device->getVideoDriver();
	cxt.sceneMgr = cxt.device->getSceneManager();
	cxt.guiEnv = cxt.device->getGUIEnvironment();
	cxt.device->setWindowCaption( L"What are the building blocks of modelling intelligent behaviour in computer programs? - David Avedissian" );
	cxt.sceneMgr->setAmbientLight( SColorf( 0.5f, 0.5f, 0.5f ) );

	// Set the font and create the diagnostic display
	IGUIFont* font = cxt.guiEnv->getFont( "default.xml" );
	cxt.guiEnv->getSkin()->setFont( font );
	mDiagnostic = cxt.guiEnv->addStaticText( L"", rect<s32>( 5, 5, 1024, 768 ) );
	mDiagnostic->setOverrideColor( SColor( 255, 255, 255, 255 ) );

	// Set up the camera
	mCamera = cxt.sceneMgr->addCameraSceneNode();
	mCamera->setPosition( vector3df( mCameraPos.X, mCameraHeight, mCameraPos.Y ) );
	mCamera->setTarget( mCamera->getPosition() + vector3df( 0.0f, -1.0f, 0.0f ) );
	mCamera->setUpVector( vector3df( 0.0f, 0.0f, 1.0f ) );

	// Set up the world
	mAgent = new Agent( vector2df( 0.0f, 0.0f ) );
	mItemMgr = new ItemManager( mAgent );
}

void App::mainLoop()
{
	u32 then = cxt.device->getTimer()->getTime();
	while( cxt.device->run() && cxt.driver && mIsRunning )
	{
		// Calculate the dt, for frame independent movement
		const u32 now = cxt.device->getTimer()->getTime();
		const f32 dt = ( f32 )( now - then ) / 1000.0f;
		then = now;

		// Update entity
		mAgent->update( dt );

		// Update items
		mItemMgr->update( dt );

		// Camera Scrolling
		const vector2df mousePos = getMousePosition();
		if( isMouseButtonDown( MBS_RIGHT ) )
		{
			// Start dragging
			if( !mIsDragging )
			{
				mIsDragging = true;
				mStartCameraPos = mCameraPos;
				mStartMousePosition = mMousePosition;
			}
			vector2df mouseOffset = mStartMousePosition - mousePos;

			// Compute the change in position, and constrain it
			mCameraPos = mStartCameraPos + vector2df( mouseOffset.X, -mouseOffset.Y ) * mCameraHeight *  0.002f;
			if( mCameraPos.X > HALF_PLAYFIELD_SIZE )
				mCameraPos.X = HALF_PLAYFIELD_SIZE;
			if( mCameraPos.X < -HALF_PLAYFIELD_SIZE )
				mCameraPos.X = -HALF_PLAYFIELD_SIZE;
			if( mCameraPos.Y > HALF_PLAYFIELD_SIZE )
				mCameraPos.Y = HALF_PLAYFIELD_SIZE;
			if( mCameraPos.Y < -HALF_PLAYFIELD_SIZE )
				mCameraPos.Y = -HALF_PLAYFIELD_SIZE;
		}
		else if( mIsDragging )
		{
			mIsDragging = false;
		}

		// Slowly change the camera position
		mCameraHeight = lerp( mCameraHeight, mCameraTargetHeight, clamp( dt * 8.0f, 0.0f, 1.0f ) );
		mCameraOffset = 25.0f - 25.0f * clamp( ( mCameraHeight - MIN_CAMERA_HEIGHT ) / ( MAX_CAMERA_HEIGHT - MIN_CAMERA_HEIGHT ), 0.0f, 1.0f );
		mCamera->setPosition( conv2DTo3D( mCameraPos + vector2df( 0.0f, -mCameraOffset ), mCameraHeight ) );
		mCamera->setTarget( conv2DTo3D( mCameraPos, 0.0f ) );

		// Only update the visible screen if the program isnt minimised
		if( cxt.device->isWindowActive() )
		{
			// Update the GUI
			std::wstringstream diag;
			diag << mAgent->debugOutput() << std::endl;
			mDiagnostic->setText( diag.str().c_str() );

			// Render the scene
			cxt.driver->beginScene( true, true, SColor( 255, 160, 200, 250 ) );
			cxt.sceneMgr->drawAll();
			cxt.guiEnv->drawAll();
			cxt.driver->endScene();
		}
		else
		{
			cxt.device->yield();
		}
	}
}

void App::shutdown()
{
	delete mAgent;
	mAgent = nullptr;

	cxt.device->drop();
}

bool App::isKeyDown( EKEY_CODE keyCode ) const
{
	return mKeyStates[keyCode];
}

bool App::isMouseButtonDown( MouseButtonState mouseButton ) const
{
	assert( mouseButton < 3 );
	return mMouseButtonStates[mouseButton];
}

const vector2df& App::getMousePosition() const
{
	return mMousePosition;
}

bool App::OnEvent( const SEvent& e )
{
	if( e.EventType == EET_KEY_INPUT_EVENT )
	{
		// Write the key state to the cache
		mKeyStates[e.KeyInput.Key] = e.KeyInput.PressedDown;

		// Pressing escape to exit
		if( e.KeyInput.Key == KEY_ESCAPE )
		{
			mIsRunning = false;
			return true;
		}
	}

	if( e.EventType == EET_MOUSE_INPUT_EVENT )
	{
		// Update the mouse position
		mMousePosition = vector2df( ( f32 )e.MouseInput.X, ( f32 )e.MouseInput.Y );

		// Left click to set the spawn position
		if( e.MouseInput.isLeftPressed() && mMouseButtonStates[MBS_LEFT] == false
				&& mItemMgr->getToolboxWindow()->getAbsoluteClippingRect().isPointInside( vector2d<s32>( e.MouseInput.X, e.MouseInput.Y ) ) == false )
		{
			line3df ray = cxt.sceneMgr->getSceneCollisionManager()->getRayFromScreenCoordinates( vector2di( ( s32 )mMousePosition.X, ( s32 )mMousePosition.Y ), mCamera );
			plane3df plane( vector3df( 0.0f, 0.0f, 0.0f ), vector3df( 0.0f, 1.0f, 0.0f ) );
			vector3df intersection;
			plane.getIntersectionWithLine( ray.start, ray.getVector(), intersection );
			mItemMgr->setSpawnPosition( conv3DTo2D( intersection ) );
			return true;
		}

		// Scroll Wheel to zoom in/out
		f32 moveZ = e.MouseInput.Wheel;
		if( abs( moveZ ) > 1e-6f )
		{
			mCameraTargetHeight *= 1.0f - 0.2f * moveZ;
			mCameraTargetHeight = clamp( mCameraTargetHeight, MIN_CAMERA_HEIGHT, MAX_CAMERA_HEIGHT );
		}

		// Update the mouse button states
		mMouseButtonStates[MBS_LEFT] = e.MouseInput.isLeftPressed();
		mMouseButtonStates[MBS_RIGHT] = e.MouseInput.isRightPressed();
		mMouseButtonStates[MBS_MIDDLE] = e.MouseInput.isMiddlePressed();
	}

	return mItemMgr == nullptr ? false : mItemMgr->OnEvent( e );
}