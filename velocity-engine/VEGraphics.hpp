/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

// D3D9
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment( lib, "d3d9.lib" )
#ifdef _DEBUG
#pragma comment( lib, "d3dx9d.lib" )
#else
#pragma comment( lib, "d3dx9.lib" )
#endif

// DXERR
#include <DxErr.h>
#pragma comment( lib, "dxerr.lib" )

// Scene Graph
#include "VESceneGraph.hpp"

VE_BEGIN

// Graphics Properties
struct GraphicsProperties
{
	HWND hWnd;
	D3DFORMAT backbufferFormat;
	D3DFORMAT depthFormat;
	bool waitForVSync;
	bool fullscreen;
};

// D3D Vertex Declarations
struct D3DVert_XYZ_UV
{
	float x, y, z;
	float u, v;
	static DWORD FVF;
};

struct D3DVert_XYZW
{
	float x, y, z, rhw;
	static DWORD FVF;
};

struct D3DVert_XYZW_UV
{
	float x, y, z, rhw;
	float u, v;
	static DWORD FVF;
};

// Font
class Font
{
public:
						Font();
						~Font();

	// Setup
	void				create( int size, const char *typeface );

	// Accessors
	ID3DXFont			*getFont();

private:
	// Font Interface
	ID3DXFont			*mFont;

};

// Scene Graph
class Graphics : public EventListener
{
public:
						Graphics( GraphicsProperties &props );
						~Graphics();

	// Effect Manipulation
	void				passSemanticValues( ID3DXEffect *effect );

	// Rendering
	void				beginSceneRender();
	void				endSceneRender();
	void				beginRender( bool postProcessing );
	void				endRender();
	void				present();
	void				renderPostProcessQuad();

	// Text
	void				setFont( Font *font );
	void				drawText( int x, int y, const char *text, DWORD colour );
	void				drawTextEx( RECT position, int style, const char *text, DWORD colour );

	// Matrix Operations
	D3DXMATRIX			getTop();
	void				setWorld( D3DXMATRIX *matrix );
	D3DXMATRIX			&getWorld();
	void				setView( D3DXMATRIX *matrix );
	D3DXMATRIX			&getView();
	void				setProjection( D3DXMATRIX *matrix );
	D3DXMATRIX			&getProjection();

	// Events
	void				handleEvent( Event &ev );

	// Accessors
	IDirect3DDevice9	*getDevice();
	ID3DXEffect			*getPPEffect();
	SceneGraph			*getSceneGraph();

private:
	// Properties
	GraphicsProperties	mProps;

	// D3D9
	IDirect3D9				*mD3D;
	IDirect3DDevice9		*mDevice;
	D3DPRESENT_PARAMETERS	mPP;
	IDirect3DSurface9		*mMainRTV;

	// Scene Graph
	SceneGraph				*mSceneGraph;

	// Post Process Framework
	IDirect3DTexture9		*mPPRT0, *mPPRT1;
	IDirect3DSurface9		*mPPRT0Surf, *mPPRT1Surf;
	IDirect3DVertexBuffer9	*mPPQuad;
	ID3DXEffect				*mPPEffect;

	// Matrices
	D3DXMATRIX				mWorld, mView, mProjection;

	// Other
	Font					*mCurFont;
	D3DXVECTOR3				mSunDirection;

};

VE_END
