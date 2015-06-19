#pragma once
#include "GameObject.h"
#include "Interfaces.h"
#include "Cell.h"
#include "vector"

using namespace std;

class Grid : public GameObject, public IInputReceiver
{
public:
	// ctor & dtor
	Grid();
	Grid(Vector2D size);
	virtual ~Grid();

	// GameObject
	virtual bool Initialize(LPDIRECT3DDEVICE9 d3dDevice) override;
	virtual void Deinitialize() override;
	virtual void Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect) override;
	virtual void Tick(float deltaTime) override;

	// IInputReceiver
	virtual void OnMouseMove(HWND hWnd, WORD mouseX, WORD mouseY, int comboState) override;
	virtual void OnMouseLButtonDown(HWND hWnd, WORD mouseX, WORD mouseY, int comboState) override;
	virtual void OnMouseLButtonUp(HWND hWnd, WORD mouseX, WORD mouseY, int comboState) override;
		
	// methods
	virtual void Initialize();
	Cell* GetCell(int row, int column);
	const D3DXMATRIX& GetWorldMatrix() { return mWorldMatrix; }
	const D3DXMATRIX& GetProjectionMatrix() { return mProjectionMatrix; }
	void UpdateWVPMatrix();

private:
	int mWidth;
	int mHeight;
	vector<Cell*> mCells;
	Cell* mCellInInteraction;
	D3DXMATRIX mWorldMatrix;
	D3DXMATRIX mProjectionMatrix;

	bool FindAndProcessMatch();
	virtual void HandleMouseInteraction(WORD mouseX, WORD mouseY, bool pickNewInteractiveCell);
	virtual bool ProcessVanishedCells();
	virtual void ProcessPendingCheckCells();
	virtual bool HasAnyMovement();
	virtual void VanishAllCells();
	virtual bool CheckVanishWithCell(Cell* cell);
	virtual bool CheckVanishFromCell(Cell* startCell, bool inRow);
};