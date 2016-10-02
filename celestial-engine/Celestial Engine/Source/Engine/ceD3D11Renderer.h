/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <d3dx11effect.h>
#include <DxErr.h>
#ifdef ceDEBUG
#define D3DCALL( x ) if( FAILED( D3D11Renderer::currentHR = x ) ) { ceLOGWRITE( "Error: D3D Call failed at line " << __LINE__ << " in file " << __FILE__ ); ceLOGWRITE( "- HRESULT = " << DXGetErrorStringA ( D3D11Renderer::currentHR ) ); ceLOGWRITE( "- Description: " << DXGetErrorDescriptionA( D3D11Renderer::currentHR ) ); }
#else
#define D3DCALL( x ) x
#endif

ceBEGIN

// D3D11 Renderer
class D3D11Renderer : public Renderer
{
public:
						D3D11Renderer( Game *parent );
						~D3D11Renderer();

	// Startup
	void				startup( SceneGraph *sg );
	
	// Updating
	bool				update();
	
	// Rendering
	void				rBegin( Vector3f colour );
	void				rBegin( Vector3f colour, RenderTarget rt );
	void				rEnd();
	void				rPresent();

	// Static HR
	static HRESULT		currentHR;

private:
	// Windows
	HWND				mWindowHandle;

	// Device and Context
	IDXGISwapChain		*mSwapChain;
	ID3D11Device		*mD3DDevice;
	ID3D11DeviceContext	*mD3DDevContext;
	D3D11_VIEWPORT		mViewport;

	// Backbuffer
	ID3D11RenderTargetView *mBackbufferRTV;
	ID3D11DepthStencilView *mBackbufferDSV;

};

ceEND
