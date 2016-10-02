/*
	Velocity Engine
	By SerialVelocity
*/
#include "VE.hpp"
using namespace VE;

// Static Definitions
DWORD D3DVert_XYZ_UV::FVF = D3DFVF_XYZ | D3DFVF_TEX1;
DWORD D3DVert_XYZW::FVF = D3DFVF_XYZRHW;
DWORD D3DVert_XYZW_UV::FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

// Font
Font::Font()
{
	// Set all members to default values
	mFont = 0;
}

Font::~Font()
{
	// Release font
	SAFE_RELEASE( mFont );
}

void Font::create( int size, const char *typeface )
{
	// Create the font
	HDC hdc = GetDC( 0 );
	int newSize = -MulDiv( size, GetDeviceCaps( hdc, LOGPIXELSY ), 72 );
	ReleaseDC( 0, hdc );
	D3DXCreateFontA( gEngine->getGraphics()->getDevice(), newSize, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS, DEFAULT_PITCH | FF_DONTCARE, typeface, &mFont );
}

ID3DXFont *Font::getFont()
{
	// Return the font interface
	return mFont;
}

// Graphics Subsystem
Graphics::Graphics( GraphicsProperties &props )
{
	// Determine the size of the drawable area of the window
	RECT drawArea;
	GetClientRect( props.hWnd, &drawArea );
	UINT width = drawArea.right - drawArea.left;
	UINT height = drawArea.bottom - drawArea.top;

	// Construct Presentation Parameters
	ZeroMemory( &mPP, sizeof( mPP ) );
	mPP.BackBufferFormat = props.backbufferFormat;
	mPP.BackBufferCount = 1;
	mPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	mPP.EnableAutoDepthStencil = TRUE;
    mPP.AutoDepthStencilFormat = props.depthFormat;
	mPP.PresentationInterval = props.waitForVSync == true ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
	mPP.Windowed = props.fullscreen == true ? FALSE : TRUE;
	mPP.BackBufferWidth = width;
	mPP.BackBufferHeight = height;

	// Create the device
	mD3D = Direct3DCreate9( D3D_SDK_VERSION );
	mD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, props.hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &mPP, &mDevice );

	// Grab the backbuffer
	mDevice->GetRenderTarget( 0, &mMainRTV );

	// Configure the device for first time rendering
	mDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	mDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	// Set the default material
	D3DMATERIAL9 mat;
	ZeroMemory( &mat, sizeof( mat ) );
	mat.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	mat.Ambient = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	mDevice->SetMaterial( &mat );

	// Create the fullscreen quad with pretransformed vertices
	mDevice->CreateVertexBuffer( 6 * sizeof( D3DVert_XYZW_UV ), D3DUSAGE_WRITEONLY, D3DVert_XYZW_UV::FVF, D3DPOOL_MANAGED, &mPPQuad, 0 );
	D3DVert_XYZW_UV *vData;
	mPPQuad->Lock( 0, 0, (void**)&vData, D3DLOCK_DISCARD );
	{
		// Create each vertex
		D3DVert_XYZW_UV vertices[] =
		{
			{ 0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f },
			{ (float)width, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f },
			{ (float)width, (float)height, 0.5f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f },
			{ (float)width, (float)height, 0.5f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, (float)height, 0.5f, 1.0f, 0.0f, 1.0f }
		};

		// Translate by half a pixel
		for( int i = 0; i < 6; i++ )
		{
			vertices[i].x -= 0.5f;
			vertices[i].y -= 0.5f;
		}

		// Copy data to graphics card
		memcpy( vData, vertices, sizeof( vertices ) );
	}
	mPPQuad->Unlock();

	// Setup render targets
	mDevice->CreateTexture( width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &mPPRT0, 0 );
	mPPRT0->GetSurfaceLevel( 0, &mPPRT0Surf );
	mDevice->CreateTexture( width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &mPPRT1, 0 );
	mPPRT1->GetSurfaceLevel( 0, &mPPRT1Surf );

	// Load the post process shader
	D3DXCreateEffectFromFile( mDevice, VE_WDIR_SHADERS L"postProcess.fx", 0, 0, 0, 0, &mPPEffect, 0 );
	D3DXHANDLE hTech;
	mPPEffect->FindNextValidTechnique( 0, &hTech );
	mPPEffect->SetTechnique( hTech );
	mPPEffect->SetTexture( "tBuf0", mPPRT0 );
	mPPEffect->SetTexture( "tBuf1", mPPRT1 );

	// Setup the scene graph
	mSceneGraph = new SceneGraph;

	// Pass the properties
	mProps = props;

	// Set all members to default values
	mCurFont = 0;
	mSunDirection = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
}

Graphics::~Graphics()
{
	// Release the scene graph
	SAFE_DELETE( mSceneGraph );

	// Release Resources
	SAFE_RELEASE( mPPRT0 );
	SAFE_RELEASE( mPPRT1 );
	SAFE_RELEASE( mPPRT0Surf );
	SAFE_RELEASE( mPPRT1Surf );
	SAFE_RELEASE( mPPQuad );
	SAFE_RELEASE( mPPEffect );

	// Release D3D
	SAFE_RELEASE( mMainRTV );
	SAFE_RELEASE( mDevice );
	SAFE_RELEASE( mD3D );
}

void Graphics::passSemanticValues( ID3DXEffect *effect )
{
	// Check each semantic and fill if necessary
	D3DXHANDLE handle;
	handle = effect->GetParameterBySemantic( 0, "WORLDVIEWPROJECTION" );
	if( handle ) effect->SetMatrix( handle, &( mWorld * mView * mProjection ) );
	handle = effect->GetParameterBySemantic( 0, "WORLD" );
	if( handle ) effect->SetMatrix( handle, &mWorld );
	handle = effect->GetParameterBySemantic( 0, "SUNDIRECTION" );
	if( handle ) effect->SetVector( handle, &D3DXVECTOR4( mSunDirection.x, mSunDirection.y, mSunDirection.z, 1.0f ) );
}

void Graphics::beginSceneRender()
{
	// Set render target to the scene buffer
	mDevice->SetRenderTarget( 0, mPPRT0Surf );
	mDevice->SetRenderTarget( 1, mPPRT1Surf );

	// Clear the screen
	mDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
	mDevice->BeginScene();

	// Update the view matrix
	mSceneGraph->getCamera()->updateView();
}

void Graphics::endSceneRender()
{
	// Set render target to the backbuffer
	mDevice->SetRenderTarget( 0, mMainRTV );
	mDevice->SetRenderTarget( 1, 0 );

	// End scene
	mDevice->EndScene();
}

void Graphics::beginRender( bool postProcessing )
{
	// Clear the screen
	mDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
	mDevice->BeginScene();

	// Update the view matrix
	if( !postProcessing )
		mSceneGraph->getCamera()->updateView();
}

void Graphics::endRender()
{
	// End scene
	mDevice->EndScene();
}

void Graphics::present()
{
	// Present the scene
	mDevice->Present( 0, 0, 0, 0 );
}

void Graphics::renderPostProcessQuad()
{
	// Draw the fullscreen quad with the shader
	UINT passCount = 0;
	mPPEffect->Begin( &passCount, 0 );
	for( UINT cPass = 0; cPass < passCount; cPass++ )
	{
		mPPEffect->BeginPass( cPass );
			mDevice->SetFVF( D3DVert_XYZW_UV::FVF );
			mDevice->SetStreamSource( 0, mPPQuad, 0, sizeof( D3DVert_XYZW_UV ) );
			mDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
		mPPEffect->EndPass();
	}
	mPPEffect->End();
}

void Graphics::setFont( Font *font )
{
	// Update the current font
	mCurFont = font;
}

void Graphics::drawText( int x, int y, const char *text, DWORD colour )
{
	// Call the parent function
	RECT pos = { x, y, 0, 0 };
	drawTextEx( pos, DT_LEFT | DT_NOCLIP, text, colour );
}

void Graphics::drawTextEx( RECT position, int style, const char *text, DWORD colour )
{
	// If the current font is 0, return
	if( mCurFont == 0 ) return;

	// Draw some text 
	mCurFont->getFont()->DrawTextA( 0, text, -1, &position, style, colour );
}

void Graphics::setWorld( D3DXMATRIX *matrix )
{
	// Update the world matrix
	mWorld = *matrix;
	mDevice->SetTransform( D3DTS_WORLD, matrix );
}

D3DXMATRIX &Graphics::getWorld()
{
	// Return the world matrix
	return mWorld;
}

void Graphics::setView( D3DXMATRIX *matrix )
{
	// Update the view matrix
	mView = *matrix;
	mDevice->SetTransform( D3DTS_VIEW, matrix );
}

D3DXMATRIX &Graphics::getView()
{
	// Return the view matrix
	return mView;
}

void Graphics::setProjection( D3DXMATRIX *matrix )
{
	// Set the projection matrix
	mProjection = *matrix;
	mDevice->SetTransform( D3DTS_PROJECTION, matrix );
}

D3DXMATRIX &Graphics::getProjection()
{
	// Return the projection matrix
	return mProjection;
}

void Graphics::handleEvent( Event &ev )
{
	// Act on the event type
	int type = ev.getType();
	
	// When a new scene node is created
	if( type == ET_NEW_SCENE_NODE )
	{
		// Add the scene node to the graph
		mSceneGraph->addSceneNode( (SceneNode*)ev.getData() );
	}
}

IDirect3DDevice9 *Graphics::getDevice()
{
	// Return the Direct3D Device
	return mDevice;
}

ID3DXEffect *Graphics::getPPEffect()
{
	// Return the post process effect
	return mPPEffect;
}

SceneGraph *Graphics::getSceneGraph()
{
	// Return the scene graph
	return mSceneGraph;
}