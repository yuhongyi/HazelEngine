#pragma once
#include "Globals.h"
#include "GameObject.h"
#include "VertexBufferResource.h"

class Grid;

enum class CellState
{
	Idle,
	InInteraction,
	Vanish,
	Falling,
	Switching,
	PendingCheck,
};

class Cell : public GameObject
{
public:
	// ctor & dtor
	Cell(Grid* parentGrid);
	
	// GameObject
	virtual bool Initialize(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void Deinitialize();
	virtual void Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect);
	virtual void Tick(float deltaTime);
	virtual void SetPosition(Vector2D newPosition);

	// methods
	void SetTargetPosition(Vector2D targetPosition);
	void SetResourceName(const wstring& resourceName);
	const wstring& GetResourceName() const;
	void SetRow(int row);
	int GetRow() const;
	void SetColumn(int column);
	int GetColumn() const;
	void SetCellState(CellState newCellState);
	CellState GetCellState() const;
	Cell* GetSwappedCell() const;
	void SetSwappedCell(Cell* other);
	//Cell* GetCellBelow() const;
	void SetCellBelow(Cell* cellBelow);

	virtual bool IsAdjacentTo(Cell* other);
	virtual void SwapWith(Cell* other, bool isSwapBack);

private:
	wstring mResourceName;
	CellState mPrevCellState;
	CellState mCellState;
	int mRow;
	int mColumn;
	Cell* mCellBelow;
	Grid* mParentGrid;
	D3DXMATRIX mWVPMatrix;

	// Physics
	Cell* mSwappedCell;
	Vector2D mTargetPosition;
	float mFallingSpeed;
	virtual void TickFalling(float deltaTime);
	virtual void TickSwitching(float deltaTime);
	virtual void OnPositionUpdated();

	// Render related
	VertexBufferResource* mVBResource;
};