//--------------------------------------------------------------------------------------
// File: ZooKeeper.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "../resource.h"
#include "ZooKeeperGame.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "ParticleManager.h"
#include <windows.h>

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9				g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device


//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D(HWND hWnd)
{
	// Create the D3D object.
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// Set up the structure used to create the D3DDevice. Since we are now
	// using more complex geometry, we will create a device with a zbuffer.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// Create the D3DDevice
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	// Turn on the zbuffer
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// Turn on ambient lighting 
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void Render(float fElapsedTime)
{
	HRESULT hr;

	// Clear the render target and the zbuffer 
	V(g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0));

	// Render the scene
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		ShaderResource* shader = dynamic_cast<ShaderResource*>(ResourceManager::GetInstance()->GetResourceById(7));
		if (shader)
		{
			ID3DXEffect* shaderEffect = shader->GetEffect();
			if (shaderEffect)
			{
				ZooKeeperGame::GetInstance()->Render(g_pd3dDevice, shaderEffect);
			}
		}
		V(g_pd3dDevice->EndScene());
	}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void Tick()
{
	static float gameTime = GetTickCount() / 1000.f;

	float newGameTime = GetTickCount() / 1000.f;
	float fElapsedTime = (newGameTime - gameTime);
	gameTime = newGameTime;

	ParticleManager::GetInstance()->Tick(fElapsedTime);
	ZooKeeperGame::GetInstance()->Tick(fElapsedTime);

	Render(fElapsedTime);
}


HRESULT InitializeGame()
{
	// Initialize Resource
	if(!ResourceManager::GetInstance()->InitResource(g_pd3dDevice))
	{
		return E_FAIL;
	}
	
	// Initialize game object
	if(!ParticleManager::GetInstance()->Initialize(g_pd3dDevice))
	{
		return E_FAIL;
	}

	if(!ZooKeeperGame::GetInstance()->Initialize(g_pd3dDevice))
	{
		return E_FAIL;
	}

	return S_OK;
}


void CleanUp()
{
	ResourceManager::GetInstance()->CleanUpResource();
	ParticleManager::GetInstance()->Deinitialize();
	ZooKeeperGame::GetInstance()->Deinitialize();

	InputManager::GetInstance()->Destroy();
	ResourceManager::GetInstance()->Destroy();
	ParticleManager::GetInstance()->Destroy();
	ZooKeeperGame::GetInstance()->Destroy();
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
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
	case WM_DESTROY:
		CleanUp();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D Tutorial", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow(L"D3D Tutorial", L"D3D Tutorial 06: Meshes",
		WS_OVERLAPPEDWINDOW, 100, 100, gGridWidth * gCellSize, gGridHeight * gCellSize,
		NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// Create the scene geometry
		if (SUCCEEDED(InitializeGame()))
		{
			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			// Enter the message loop
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					Tick();
				}
			}
		}
	}

	UnregisterClass(L"D3D Tutorial", wc.hInstance);
	return 0;
}