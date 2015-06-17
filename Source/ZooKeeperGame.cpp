#include "Time.h"
#include "ZooKeeperGame.h"
#include "InputManager.h"
#include "ParticleManager.h"

ZooKeeperGame::ZooKeeperGame()
{
	mGrid = new Grid(Vector2D((float)gGridWidth, (float)gGridHeight));
}

ZooKeeperGame::~ZooKeeperGame()
{
	delete mGrid;
}

void ZooKeeperGame::Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect)
{
	if(mGrid)
	{
		mGrid->Render(d3dDevice, effect);
		ParticleManager::GetInstance()->Render(d3dDevice, effect);
	}
}

bool ZooKeeperGame::Initialize(LPDIRECT3DDEVICE9 d3dDevice)
{
	if(!mGrid || !mGrid->Initialize(d3dDevice))
	{
		return false;
	}

	// Register to input manager
	InputManager::GetInstance()->RegisterInputReceiver(this);
	
	// Set random seed for game
	srand((unsigned int)time(nullptr));

	return true;
}

void ZooKeeperGame::Deinitialize()
{
	if(mGrid)
	{
		mGrid->Deinitialize();
	}
}

void ZooKeeperGame::OnMouseMove(HWND hWnd, WORD mouseX, WORD mouseY, int comboState)
{
	if(mGrid)
	{
		mGrid->OnMouseMove(hWnd, mouseX, mouseY, comboState);
	}
}

void ZooKeeperGame::OnMouseLButtonDown(HWND hWnd, WORD mouseX, WORD mouseY, int comboState)
{
	if(mGrid)
	{
		mGrid->OnMouseLButtonDown(hWnd, mouseX, mouseY, comboState);
	}
}

void ZooKeeperGame::OnMouseLButtonUp(HWND hWnd, WORD mouseX, WORD mouseY, int comboState)
{
	if(mGrid)
	{
		mGrid->OnMouseLButtonUp(hWnd, mouseX, mouseY, comboState);
	}
}

void ZooKeeperGame::Tick(float deltaTime)
{
	if(mGrid)
	{
		mGrid->Tick(deltaTime);
	}
}