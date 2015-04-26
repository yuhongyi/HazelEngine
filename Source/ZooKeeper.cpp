//--------------------------------------------------------------------------------------
// File: ZooKeeper.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#include "../resource.h"
#include "ZooKeeperGame.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SoundManager.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device

//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                      bool bWindowed, void* pUserContext )
{
    // Typically want to skip back buffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
	g_pd3dDevice = pd3dDevice;

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	ParticleManager::GetInstance()->Tick(fElapsedTime);
	ZooKeeperGame::GetInstance()->Tick(fElapsedTime);
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		ShaderResource* shader = dynamic_cast<ShaderResource*>(ResourceManager::GetInstance()->GetResourceById(10));
		if(shader)
		{
			ID3DXEffect* shaderEffect = shader->GetEffect();
			if(shaderEffect)
			{
				ZooKeeperGame::GetInstance()->Render(pd3dDevice, shaderEffect);
			}
		}
        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
	switch( uMsg )
	{
		case WM_MOUSEMOVE:
			InputManager::GetInstance()->OnMouseMove(hWnd, LOWORD(lParam), HIWORD(lParam), (int)wParam);
			break;
		case WM_LBUTTONDOWN:
			InputManager::GetInstance()->OnMouseLButtonDown(hWnd, LOWORD(lParam), HIWORD(lParam), (int)wParam);
			break;
		case WM_LBUTTONUP:
			InputManager::GetInstance()->OnMouseLButtonUp(hWnd, LOWORD(lParam), HIWORD(lParam), (int)wParam);
			break;
	}

    return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
}

bool InitializeGame()
{
	// Initialize Resource
	if(!ResourceManager::GetInstance()->InitResource(g_pd3dDevice))
	{
		return false;
	}

	if(!SoundManager::GetInstance()->InitResource(g_pd3dDevice))
	{
		return false;
	}
	
	// Initialize game object
	if(!ParticleManager::GetInstance()->Initialize(g_pd3dDevice))
	{
		return false;
	}

	if(!ZooKeeperGame::GetInstance()->Initialize(g_pd3dDevice))
	{
		return false;
	}

	return true;
}

void CleanUp()
{
	SoundManager::GetInstance()->CleanUpResource();
	ResourceManager::GetInstance()->CleanUpResource();
	ParticleManager::GetInstance()->Deinitialize();
	ZooKeeperGame::GetInstance()->Deinitialize();

	InputManager::GetInstance()->Destroy();
	SoundManager::GetInstance()->Destroy();
	ResourceManager::GetInstance()->Destroy();
	ParticleManager::GetInstance()->Destroy();
	ZooKeeperGame::GetInstance()->Destroy();
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions
    DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameMove( OnFrameMove );

    // TODO: Perform any application-level initialization here

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"ZooKeeper" );
	DXUTCreateDevice( true, gGridWidth * gCellSize, gGridHeight * gCellSize );

	// Initialize D3D resources
	if(!InitializeGame())
	{
		return DXUTGetExitCode();
	}

    // Start the render loop
    DXUTMainLoop();
	
	// Clean up 
	CleanUp();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}
