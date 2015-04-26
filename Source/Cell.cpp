#include "stdafx.h"
#include "Cell.h"
#include "ImageResource.h"

Cell::Cell():
	mResourceId(-1),
	mVB(NULL),
	mPrevCellState(CS_Idle),
	mCellState(CS_Idle),
	mFallingSpeed(0.f),
	mCellBelow(NULL)
{
	mSize.X = (float)gCellSize;
	mSize.Y = (float)gCellSize;
}

bool Cell::Initialize(LPDIRECT3DDEVICE9 d3dDevice)
{
    if( FAILED( d3dDevice->CreateVertexBuffer( 4 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &mVB, NULL ) ) )
    {
        return false;
    }

	return true;
}

void Cell::Deinitialize()
{
	if(mVB)
	{
		mVB->Release();
	}
}

void Cell::Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect)
{
	// Set Texture
	GameResource* d3dResource = ResourceManager::GetInstance()->GetResourceById(mResourceId);
	ImageResource* image = dynamic_cast<ImageResource*>(d3dResource);
	if(image)
	{
		D3DXHANDLE cellTextureHandle = effect->GetParameterByName(NULL, "gCellTexture");
		effect->SetTexture(cellTextureHandle, image->GetTexture());
	}

	// Set VB
	d3dDevice->SetStreamSource(0, mVB, 0, sizeof(CUSTOMVERTEX));
	d3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

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
	// Update VB
	UpdateVertexBuffer();
}


void Cell::SetResourceID(int resourceId)
{
	mResourceId = resourceId;
}

int Cell::GetResourceID() const
{
	return mResourceId;
}

void Cell::UpdateVertexBuffer()
{
	D3DCOLOR cellDiffuse = 0xffffffff;
#if COLORDEBUGGING
	switch(mCellState)
	{
	case CS_Idle:
		cellDiffuse = 0xffffffff;
		break;
	case CS_InInteraction:
		cellDiffuse = 0xffff0000;
		break;
	case CS_Vanish:
		cellDiffuse = 0xff00ff00;
		break;
	case CS_Falling:
		cellDiffuse = 0xff0000ff;
		break;
	}
#endif
	CUSTOMVERTEX Vertices[] =
	{
		{ mPosition.X,  mPosition.Y, 0.f, cellDiffuse, 0.f, 0.f}, // x, y, z, tex2
		{ mPosition.X + mSize.X,  mPosition.Y, 0.f, cellDiffuse, 1.f, 0.f}, 
		{ mPosition.X,  mPosition.Y + mSize.Y, 0.f, cellDiffuse, 0.f, 1.f}, 
		{ mPosition.X + mSize.X,  mPosition.Y + mSize.Y, 0.f, cellDiffuse, 1.f, 1.f}, 
	};

	VOID* pVertices;
	if(!mVB || FAILED(mVB->Lock(0, sizeof(Vertices), (void**)&pVertices, 0)))
	{
		return;
	}

	memcpy( pVertices, Vertices, sizeof( Vertices ) );
	mVB->Unlock();
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
#if COLORDEBUGGING
	UpdateVertexBuffer();
#endif
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

//Cell* Cell::GetCellBelow() const
//{
//	return mCellBelow;
//}

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
		case CS_Falling:
			TickFalling(deltaTime);
			break;
		case CS_Switching:
			TickSwitching(deltaTime);
			break;
	}

	mPrevCellState = mCellState;
}

void Cell::TickFalling(float deltaTime)
{
	if(mPrevCellState == CS_Idle)
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
		SetCellState(CS_Idle);
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
		if(GetSwappedCell() != NULL)
		{
			SetCellState(CS_PendingCheck);
		}
		else
		{
			SetCellState(CS_Idle);
		}
	}

	OnPositionUpdated();
}

void Cell::SwapWith(Cell* other, bool isSwapBack)
{
	int tempResourceId = this->GetResourceID();
	this->SetResourceID(other->GetResourceID());
	other->SetResourceID(tempResourceId);

	Vector2D tempPosition = this->GetPosition();
	this->SetPosition(other->GetPosition());
	other->SetPosition(tempPosition);

	if(!isSwapBack)
	{
		this->SetSwappedCell(other);
		other->SetSwappedCell(this);
	}
	else
	{
		this->SetSwappedCell(NULL);
		other->SetSwappedCell(NULL);
	}

	this->SetCellState(CS_Switching);
	other->SetCellState(CS_Switching);
}