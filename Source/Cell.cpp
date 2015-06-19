#include "Globals.h"
#include "Cell.h"
#include "Grid.h"
#include "ImageResource.h"
#include "ResourceManager.h"

Cell::Cell(Grid* parentGrid):
	mPrevCellState(CellState::Idle),
	mCellState(CellState::Idle),
	mFallingSpeed(0.f),
	mCellBelow(nullptr),
	mParentGrid(parentGrid),
	mVBResource(nullptr)
{
	mVBResource = new VertexBufferResource();

	mSize.X = (float)gCellSize;
	mSize.Y = (float)gCellSize;
}

bool Cell::Initialize(LPDIRECT3DDEVICE9 d3dDevice)
{
	D3DCOLOR cellDiffuse = 0xffffffff;
	CUSTOMVERTEX Vertices[] =
	{
		{ 0.f, 0.f, 0.f, cellDiffuse, 0.f, 0.f }, // x, y, z, tex2
		{ mSize.X, 0.f, 0.f, cellDiffuse, 1.f, 0.f },
		{ 0.f, mSize.Y, 0.f, cellDiffuse, 0.f, 1.f },
		{ mSize.X, mSize.Y, 0.f, cellDiffuse, 1.f, 1.f },
	};

	mVBResource->SetVertexBufferData(sizeof(CUSTOMVERTEX), 4, D3DFVF_CUSTOMVERTEX, Vertices);
	mVBResource->InitResource(d3dDevice);

	return true;
}

void Cell::Deinitialize()
{
}

void Cell::Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect)
{
	// Set Texture
	ArchivedGameResource* d3dResource = ResourceManager::GetInstance()->GetArchivedGameResource(mResourceName);
	ImageResource* image = dynamic_cast<ImageResource*>(d3dResource);
	if(image)
	{
		D3DXHANDLE cellTextureHandle = effect->GetParameterByName(0, "gCellTexture");
		effect->SetTexture(cellTextureHandle, image->GetTexture());
	}

	// Set VB
	d3dDevice->SetStreamSource(0, mVBResource->GetVertexBuffer(), 0, mVBResource->GetVertexStride());
	d3dDevice->SetFVF(mVBResource->GetVertexFormat());

	// Update matrix
	D3DXHANDLE wvpMatrixHandle = effect->GetParameterByName(0, "gWorldViewProjectionMatrix");
	effect->SetMatrix(wvpMatrixHandle, &mWVPMatrix);

	// Draw
	effect->CommitChanges();
	d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void Cell::SetPosition(Vector2D newPosition)
{
	if(mPosition != newPosition)
	{
		GameObject::SetPosition(newPosition);
		OnPositionUpdated();
	}
}

void Cell::SetTargetPosition(Vector2D targetPosition)
{
	mTargetPosition = targetPosition;
}

void Cell::OnPositionUpdated()
{
	D3DXMATRIX matCellTrans;
	D3DXMATRIX matCellWorldTrans;
	D3DXMatrixTranslation(&matCellTrans, mPosition.X, -mPosition.Y, 0.0f);
	D3DXMatrixMultiply(&matCellWorldTrans, &mParentGrid->GetWorldMatrix(), &matCellTrans);
	D3DXMatrixMultiply(&mWVPMatrix, &matCellWorldTrans, &mParentGrid->GetProjectionMatrix());
}

void Cell::SetResourceName(const wstring& resourceName)
{
	mResourceName = resourceName;
}

const wstring& Cell::GetResourceName() const
{
	return mResourceName;
}

void Cell::SetRow(int row)
{
	mRow = row;
}

int Cell::GetRow() const
{
	return mRow;
}

void Cell::SetColumn(int column)
{
	mColumn = column;
}

int Cell::GetColumn() const
{
	return mColumn;
}

void Cell::SetCellState(CellState newCellState)
{
	mCellState = newCellState;
}

CellState Cell::GetCellState() const
{
	return mCellState;
}

Cell* Cell::GetSwappedCell() const
{
	return mSwappedCell;
}

void Cell::SetSwappedCell(Cell* other)
{
	mSwappedCell = other;
}

void Cell::SetCellBelow(Cell* cellBelow)
{
	mCellBelow = cellBelow;
}

bool Cell::IsAdjacentTo(Cell* other)
{
	if(!other)
	{
		return false;
	}

	if((abs(this->GetRow() - other->GetRow()) + abs(this->GetColumn() - other->GetColumn())) == 1)
	{
		return true;
	}

	return false;
}

void Cell::Tick(float deltaTime)
{
	switch(mCellState)
	{
		case CellState::Falling:
			TickFalling(deltaTime);
			break;
		case CellState::Switching:
			TickSwitching(deltaTime);
			break;
	}

	mPrevCellState = mCellState;
}

void Cell::TickFalling(float deltaTime)
{
	if(mPrevCellState == CellState::Idle)
	{
		mFallingSpeed = 0.f;
	}

	mFallingSpeed += gFallingAcceleration * deltaTime;
	mPosition.Y += mFallingSpeed * deltaTime;

	// Should not overlap with cell below
	if(mCellBelow)
	{
		float maxPositionY = mCellBelow->GetPosition().Y - mSize.Y;
		mPosition.Y = min<float>(mPosition.Y, maxPositionY);
	}

	// Check if landed
	if(mPosition.Y >= mTargetPosition.Y)
	{
		mPosition.Y = mTargetPosition.Y;
		mFallingSpeed = 0.f;
		SetCellState(CellState::Idle);
	}

	OnPositionUpdated();
}

void Cell::TickSwitching(float deltaTime)
{
	// Reach target
	if(mTargetPosition.X > mPosition.X)
	{
		mPosition.X += gSwitchingSpeed * deltaTime;
		if(mPosition.X > mTargetPosition.X)
		{
			mPosition.X = mTargetPosition.X;
		}
	}
	else
	{
		mPosition.X -= gSwitchingSpeed * deltaTime;
		if(mPosition.X < mTargetPosition.X)
		{
			mPosition.X = mTargetPosition.X;
		}
	}

	if(mTargetPosition.Y > mPosition.Y)
	{
		mPosition.Y += gSwitchingSpeed * deltaTime;
		if(mPosition.Y > mTargetPosition.Y)
		{
			mPosition.Y = mTargetPosition.Y;
		}
	}
	else
	{
		mPosition.Y -= gSwitchingSpeed * deltaTime;
		if(mPosition.Y < mTargetPosition.Y)
		{
			mPosition.Y = mTargetPosition.Y;
		}
	}

	if(mPosition == mTargetPosition)
	{
		if(GetSwappedCell() != nullptr)
		{
			SetCellState(CellState::PendingCheck);
		}
		else
		{
			SetCellState(CellState::Idle);
		}
	}

	OnPositionUpdated();
}

void Cell::SwapWith(Cell* other, bool isSwapBack)
{
	const wstring tempResourceId = this->GetResourceName();
	this->SetResourceName(other->GetResourceName());
	other->SetResourceName(tempResourceId);

	const Vector2D tempPosition = this->GetPosition();
	this->SetPosition(other->GetPosition());
	other->SetPosition(tempPosition);

	if(!isSwapBack)
	{
		this->SetSwappedCell(other);
		other->SetSwappedCell(this);
	}
	else
	{
		this->SetSwappedCell(nullptr);
		other->SetSwappedCell(nullptr);
	}

	this->SetCellState(CellState::Switching);
	other->SetCellState(CellState::Switching);
}