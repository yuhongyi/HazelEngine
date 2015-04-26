#pragma once
#include "Globals.h"
#include "Grid.h"
#include "GameObject.h"
#include "Singleton.h"

class ZooKeeperGame : public Singleton<ZooKeeperGame>, public GameObject, public IInputReceiver
{
public:
	// ctor & dtor
	ZooKeeperGame();
	~ZooKeeperGame();

	// GameObject
	virtual bool Initialize(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void Deinitialize();
	virtual void Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect);
	virtual void Tick(float deltaTime);

	// IInputReceiver
	virtual void OnMouseMove(HWND hWnd, WORD mouseX, WORD mouseY, int comboState);
	virtual void OnMouseLButtonDown(HWND hWnd, WORD mouseX, WORD mouseY, int comboState);
	virtual void OnMouseLButtonUp(HWND hWnd, WORD mouseX, WORD mouseY, int comboState);
	
private:
	Grid* mGrid;
};