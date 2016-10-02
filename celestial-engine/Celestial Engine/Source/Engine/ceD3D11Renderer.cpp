/*
	Celestial Engine
	Created by SerialVelocity
*/
#include "celestialBase.h"
#include "ceD3D11Renderer.h"
#define ceSUBSYSTEM "Renderer (D3D11)"

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )
#ifdef ceDEBUG
#pragma comment( lib, "d3dx11d.lib" )
#pragma comment( lib, "d3dx10d.lib" )
#pragma comment( lib, "d3dx11fxd.lib" )
#else
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3dx10.lib" )
#pragma comment( lib, "d3dx11fx.lib" )
#endif
#pragma comment( lib, "DxErr.lib" )

HRESULT celestial::D3D11Renderer::currentHR = S_OK;

// Window Proc
LRESULT CALLBACK WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// Act based on the message type
	switch( msg ) 
	{
		// Pass any messages we care about to the MessageHandler object

		// On Close
		case WM_CLOSE:
			DestroyWindow( hWnd );
		break;

		// On Destroy
		case WM_DESTROY:
			PostQuitMessage( 0 );
		break;

		default:
			return DefWindowProc( hWnd, msg, wParam, lParam );
	} 
	return FALSE;
}

ceBEGIN

D3D11Renderer::D3D11Renderer( Game *parent ) : Renderer( parent )
{
	// Windows
	mWindowHandle = 0;

	// Device and Context
	mSwapChain = 0;
	mD3DDevice = 0;
	mD3DDevContext = 0;

	// Backbuffer
	mBackbufferRTV = 0;
	mBackbufferDSV = 0;
}

D3D11Renderer::~D3D11Renderer()
{
	// Release D3D stuff
	if( mD3DDevContext )
	{
		mD3DDevContext->ClearState();
		mD3DDevContext->Flush();
	}
	ceSAFE_RELEASE( mSwapChain );
	ceSAFE_RELEASE( mBackbufferDSV );
	ceSAFE_RELEASE( mBackbufferRTV );
	ceSAFE_RELEASE( mD3DDevContext );
	ceSAFE_RELEASE( mD3DDevice );

	// Destroy the window
	DestroyWindow( mWindowHandle );
}

void D3D11Renderer::startup( SceneGraph *sg )
{
	Renderer::startup( sg );

	// Register the window class
	WNDCLASSEX wndClass;
	memset( &wndClass, 0, sizeof( WNDCLASSEX ) );
	wndClass.cbSize = sizeof( WNDCLASSEX );
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = GetModuleHandle( 0 ); 
	wndClass.hIcon = LoadIcon( 0, IDI_APPLICATION );
	wndClass.hIconSm = LoadIcon( 0, IDI_APPLICATION );
	wndClass.hCursor = LoadCursor( 0, IDC_ARROW );
	wndClass.hbrBackground = 0;
	wndClass.lpszClassName = "CE_WINDOW_CLASS";
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx( &wndClass );

	// Create the window
	mWindowHandle = CreateWindow( wndClass.lpszClassName, mGame->getProperties().windowTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
								  mGame->getProperties().windowSize.x, mGame->getProperties().windowSize.y, 0, 0, wndClass.hInstance, 0 );
	ceASSERT( mWindowHandle );
	ShowWindow( mWindowHandle, SW_SHOWDEFAULT );
	UpdateWindow( mWindowHandle );
	ceSSLOGWRITE( "Created a window at " << mGame->getProperties().windowSize.x << "x" << mGame->getProperties().windowSize.y );

	// Get the window size
	RECT clientRect;
	u32 screenWidth, screenHeight;
	GetClientRect( mWindowHandle, &clientRect );
	screenWidth = clientRect.right - clientRect.left;
	screenHeight = clientRect.bottom - clientRect.top;

	// Create the Swap Chain Description
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = screenWidth;
	sd.BufferDesc.Height = screenHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = mWindowHandle;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Create a device, device context and swap chain using the information in the sc description
	u32 flags = 0;
#ifdef CE_DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3DCALL( D3D11CreateDeviceAndSwapChain( 0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, 0, 0, D3D11_SDK_VERSION, &sd, &mSwapChain, &mD3DDevice, 0, &mD3DDevContext ) );
	ceSSLOGWRITE( "Created the Direct3D 11 Device and Swap Chain" );

	// Setup the backbuffers render target view
	ID3D11Texture2D *pBackBuffer;
	D3DCALL( mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&pBackBuffer ) );
	D3DCALL( mD3DDevice->CreateRenderTargetView( pBackBuffer, 0, &mBackbufferRTV ) );
	pBackBuffer->Release();
	ceSSLOGWRITE( "Setup the backbuffer RTV" );

	// Create backbuffers depth stencil texture
	ID3D11Texture2D *pDepthStencil;
    D3D11_TEXTURE2D_DESC descDepth;
    memset( &descDepth, 0, sizeof( descDepth ) );
	descDepth.Width = screenWidth;
	descDepth.Height = screenHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc = sd.SampleDesc;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	D3DCALL( mD3DDevice->CreateTexture2D( &descDepth, 0, &pDepthStencil ) );

    // Create the backbuffers depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    memset( &descDSV, 0, sizeof( descDSV ) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
	D3DCALL( mD3DDevice->CreateDepthStencilView( pDepthStencil, &descDSV, &mBackbufferDSV ) );
	pDepthStencil->Release();
	ceSSLOGWRITE( "Setup the backbuffer DSV" );

	// Set the viewport
	memset( &mViewport, 0, sizeof( D3D11_VIEWPORT ) );
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = (f32)screenWidth;
	mViewport.Height = (f32)screenHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
}

bool D3D11Renderer::update()
{
	// Update the message pump
	MSG msg;
	while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) == TRUE )
	{
		// If the message is WM_QUIT, return false
		if( msg.message == WM_QUIT )
			return false;

		// Process Messages
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	// Continue the loop
	return true;
}

void D3D11Renderer::rBegin( Vector3f colour )
{
	// Set the render targets to the backbuffers RTV and DSV
	mD3DDevContext->OMSetRenderTargets( 1, &mBackbufferRTV, mBackbufferDSV );
	mD3DDevContext->RSSetViewports( 1, &mViewport );

	// Clear the back buffer
	f32 colArray[4] = { colour.x, colour.y, colour.z, 1.0f };
	mD3DDevContext->ClearRenderTargetView( mBackbufferRTV, colArray );
	mD3DDevContext->ClearDepthStencilView( mBackbufferDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
}

void D3D11Renderer::rBegin( Vector3f colour, RenderTarget rt )
{
	rBegin( colour );
}

void D3D11Renderer::rEnd()
{
}

void D3D11Renderer::rPresent()
{
	// Switch the back buffer and the front buffer
	mSwapChain->Present( 0, 0 );
}

ceEND
