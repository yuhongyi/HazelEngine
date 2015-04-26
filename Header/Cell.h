#pragma once
#include "GameObject.h"

enum CellState
{
	CS_Idle,
	CS_InInteraction,
	CS_Vanish,
	CS_Falling,
	CS_Switching,
	CS_PendingCheck,
};

class Cell : public GameObject
{
public:
	// ctor & dtor
	Cell();
	
	// GameObject
	virtual bool Initialize(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void Deinitialize();
	virtual void Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect);
	virtual void Tick(float deltaTime);

	// methods
	void SetPosition(Vector2D newPosition);
	void SetTargetPosition(Vector2D targetPosition);
	void SetResourceID(int resourceId);
	int GetResourceID() const;
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
	int mResourceId;
	CellState mPrevCellState;
	CellState mCellState;
	int mRow;
	int mColumn;
	Cell* mCellBelow;

	// Physics
	Cell* mSwappedCell;
	Vector2D mTargetPosition;
	float mFallingSpeed;
	virtual void TickFalling(float deltaTime);
	virtual void TickSwitching(float deltaTime);
	virtual void OnPositionUpdated();
	virtual void UpdateVertexBuffer();

	// Render related
	LPDIRECT3DVERTEXBUFFER9 mVB;
};