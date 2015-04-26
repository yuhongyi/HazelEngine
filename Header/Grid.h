#pragma once
#include "Cell.h"
#include "GameObject.h"

class Grid : public GameObject, public IInputReceiver
{
public:
	// ctor & dtor
	Grid();
	Grid(Vector2D size);
	virtual ~Grid();

	// GameObject
	virtual bool Initialize(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void Deinitialize();
	virtual void Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect);
	virtual void Tick(float deltaTime);

	// IInputReceiver
	virtual void OnMouseMove(HWND hWnd, WORD mouseX, WORD mouseY, int comboState);
	virtual void OnMouseLButtonDown(HWND hWnd, WORD mouseX, WORD mouseY, int comboState);
	virtual void OnMouseLButtonUp(HWND hWnd, WORD mouseX, WORD mouseY, int comboState);
		
	// methods
	virtual void Initialize();
	Cell* GetCell(int row, int column);

private:
	int mWidth;
	int mHeight;
	Cell* mCells;
	Cell* mCellInInteraction;

	bool FindAndProcessMatch();
	virtual void HandleMouseInteraction(WORD mouseX, WORD mouseY, bool pickNewInteractiveCell);
	virtual bool ProcessVanishedCells();
	virtual void ProcessPendingCheckCells();
	virtual bool HasAnyMovement();
	virtual void VanishAllCells();
	virtual bool CheckVanishWithCell(Cell* cell);
	virtual bool CheckVanishFromCell(Cell* startCell, bool inRow);
};