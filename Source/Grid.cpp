#include <map>

#include "Globals.h"
#include "Grid.h"
#include "GridSimulator.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "ParticleManager.h"

using namespace std;

Grid::Grid():
mCells(nullptr), mWidth(1), mHeight(1), mCellInInteraction(nullptr)
{
}

Grid::Grid(Vector2D size) :
	GameObject(size),
	mWidth((int)size.X),
	mHeight((int)size.Y),
	mCellInInteraction(nullptr)
{
	mCells = new Cell[mWidth * mHeight];
}

Grid::~Grid()
{
	SAFE_DELETE_ARRAY(mCells);
}

void Grid::Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect)
{
	if(!mCells)
	{
		return;
	}

	HRESULT hr;
	UINT iPass, cPasses;

	// Set WVP matrix
	D3DXMATRIX matWorld, matScale, matTrans, matProj, matWVP;

	D3DXMatrixScaling(&matScale, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&matTrans, -0.5f, mHeight * gCellSize + 0.5f, 0.0f);
	D3DXMatrixMultiply(&matWorld, &matScale, &matTrans);

	D3DXMatrixOrthoOffCenterLH(
		&matProj,
		0.0f, (float)mWidth * gCellSize,
		0.0f, (float)mHeight * gCellSize,
		0.0f, 1.0f);

	D3DXMatrixMultiply(&matWVP, &matWorld, &matProj);

	D3DXHANDLE wvpMatrixHandle = effect->GetParameterByName(0, "gWorldViewProjectionMatrix");
	effect->SetMatrix(wvpMatrixHandle, &matWVP);

	D3DXHANDLE techniqueHandle = effect->GetTechniqueByName("RenderSceneWithTexture");
	effect->SetTechnique(techniqueHandle);

	V(effect->Begin(&cPasses, 0));
	for(iPass = 0; iPass < cPasses; iPass++)
	{
		V(effect->BeginPass(iPass));

		// Render
		for(int i = 0; i < mWidth * mHeight; i++)
		{
			mCells[i].Render(d3dDevice, effect);
		}

		V(effect->EndPass());
	}
	V(effect->End());
}

bool Grid::Initialize(LPDIRECT3DDEVICE9 d3dDevice)
{
	if(!mCells)
	{
		return false;
	}

	// Initialize cells
	for(int i = 0; i < mWidth * mHeight; i++)
	{
		if(!mCells[i].Initialize(d3dDevice))
		{
			return false;
		}
	}

	// Initialize relative position and size of cell
	for(int i = 0; i < mWidth * mHeight; i++)
	{
		Cell& currentCell = mCells[i];
		int row = i / mWidth;
		int column = i % mWidth;

		currentCell.SetSize(Vector2D(static_cast<float>(gCellSize), static_cast<float>(gCellSize)));
		currentCell.SetPosition(Vector2D(static_cast<float>(gCellSize * column), static_cast<float>(gCellSize * row)));
		currentCell.SetTargetPosition(currentCell.GetPosition());
		currentCell.SetResourceID(ResourceManager::GetInstance()->GetRandomImageResourceId());
		currentCell.SetRow(row);
		currentCell.SetColumn(column);

		if(row < mHeight - 1)
		{
			currentCell.SetCellBelow(GetCell(row + 1, column));
		}
	}

	// Simulate grid
	GridSimulator gridSim;
	gridSim.ImportGrid(this);
	gridSim.GenerateStartGrid();
	gridSim.ExportGrid(this);

	return true;
}

void Grid::Deinitialize()
{
	if(!mCells)
	{
		return;
	}
	for(int i = 0; i < mWidth * mHeight; i++)
	{
		mCells[i].Deinitialize();
	}
}

void Grid::Initialize()
{
	if(!mCells)
	{
		return;
	}
}

Cell* Grid::GetCell(int row, int column)
{
	if(mCells
		&&row >= 0
		&& row < mHeight
		&& column >=0
		&& column < mWidth)
	{
		return &mCells[row * mWidth + column];
	}

	return nullptr;
}

void Grid::OnMouseMove(HWND hWnd, WORD mouseX, WORD mouseY, int comboState)
{
	if(InputManager::GetInstance()->GetIsMouseLeftButtonDown())
	{
		HandleMouseInteraction(mouseX, mouseY, false);
	}
}

void Grid::OnMouseLButtonDown(HWND hWnd, WORD mouseX, WORD mouseY, int comboState)
{
	HandleMouseInteraction(mouseX, mouseY, true);
}

void Grid::OnMouseLButtonUp(HWND hWnd, WORD mouseX, WORD mouseY, int comboState)
{
}

void Grid::HandleMouseInteraction(WORD mouseX, WORD mouseY, bool pickNewInteractiveCell)
{
	Vector2D relativePosition = Vector2D(mouseX, mouseY) - this->mPosition;
	Cell* hitCell = this->GetCell((int)relativePosition.Y / gCellSize, (int)relativePosition.X / gCellSize);

	if(hitCell)
	{
		if(mCellInInteraction
			&& mCellInInteraction != hitCell
			&& hitCell->GetCellState() == CS_Idle
			&& mCellInInteraction->IsAdjacentTo(hitCell))
		{
			mCellInInteraction->SwapWith(hitCell, false);
			mCellInInteraction = nullptr;
		}
		else if(hitCell->GetCellState() == CS_Idle)
		{
			if(mCellInInteraction)
			{
				mCellInInteraction->SetCellState(CS_Idle);
			}
			if(pickNewInteractiveCell)
			{
				hitCell->SetCellState(CS_InInteraction);
				mCellInInteraction = hitCell;
			}
		}
	}
}

// Which could be improved here is to only check the rows and columns that has been changed
bool Grid::FindAndProcessMatch()
{
	bool retval = false;

	// Check match in row
	for(int row = 0; row < mHeight; row++)
	{
		for(int col = 0; col <= mWidth - gMinimumVanishCount; col++)
		{
			if(CheckVanishFromCell(GetCell(row, col), true))
			{
				retval = true;
			}
		}
	}

	// Check match in column
	for(int row = 0; row <= mHeight - gMinimumVanishCount; row++)
	{
		for(int col = 0; col < mWidth; col++)
		{
			if(CheckVanishFromCell(GetCell(row, col), false))
			{
				retval = true;
			}
		}
	}

	return retval;
}

void Grid::Tick(float deltaTime)
{
	// Check whether there is no more movement.
	if(HasAnyMovement())
	{
		// Process pending-check cells
		ProcessPendingCheckCells();

		// Check match each frame
		FindAndProcessMatch();
	}

	// Process vanished cells
	ProcessVanishedCells();

	// Tick cells
	for(int i = 0; i < mWidth * mHeight; i++)
	{
		mCells[i].Tick(deltaTime);
	}
}

bool Grid::ProcessVanishedCells()
{
	bool retval = false;

	// Go through each column
	for(int col = 0; col < mWidth; col++)
	{
		int vanishedCellCounter = 0;
		for(int row = mHeight - 1; row >= 0; row--)
		{
			Cell* currentCell = GetCell(row, col);
			if(currentCell)
			{
				if(currentCell->GetCellState() == CS_Vanish)
				{
					vanishedCellCounter++;

					// Play particle
					ParticleManager::GetInstance()->SpawnParticle(currentCell, gParticleLifeTime);
				}
				else
				{
					if(vanishedCellCounter > 0)
					{
						Cell* cellToMove = GetCell(row + vanishedCellCounter, col);
						if(cellToMove)
						{
							cellToMove->SetPosition(currentCell->GetPosition());
							cellToMove->SetCellState(CS_Falling);
							cellToMove->SetResourceID(currentCell->GetResourceID());
						}
					}
				}
			}
		}

		// Find the height of last falling cell
		float lastFallingCellHeight = 0.f;
		for(int row = mHeight - 1; row >= vanishedCellCounter; row--)
		{
			Cell* fallingCell = GetCell(row, col);
			if(fallingCell && fallingCell->GetPosition().Y < lastFallingCellHeight)
			{
				lastFallingCellHeight = fallingCell->GetPosition().Y;
			}
		}

		// Generate new cells
		for(int row = 0; row < vanishedCellCounter; row++)
		{
			Cell* newCell = GetCell(row, col);
			if(newCell)
			{
				Vector2D newCellPosition = newCell->GetPosition();
				newCellPosition.Y = lastFallingCellHeight -((vanishedCellCounter - row) * newCell->GetSize().Y);
				newCell->SetPosition(newCellPosition);
				newCell->SetCellState(CS_Falling);
				newCell->SetResourceID(ResourceManager::GetInstance()->GetRandomImageResourceId());
			}
		}

		if(vanishedCellCounter > 0)
		{
			retval = true;
		}
	}

	return retval;
}

bool Grid::HasAnyMovement()
{
	// All cells need to be stable
	for(int i = 0; i < mHeight * mHeight; i++)
	{
		if(mCells[i].GetCellState() != CS_Idle
			&& mCells[i].GetCellState() != CS_InInteraction)
		{
			return true;
		}
	}

	// Check whether there is available movement
	GridSimulator gridSim;
	gridSim.ImportGrid(this);

	Vector2D cell1, cell2;

	if(!gridSim.FindAvailableMove(cell1, cell2))
	{
		VanishAllCells();

		return false;
	}

	return true;
}

void Grid::VanishAllCells()
{
	for(int i = 0; i < mHeight * mWidth; i++)
	{
		mCells[i].SetCellState(CS_Vanish);
	}
}

void Grid::ProcessPendingCheckCells()
{
	map<Cell*, bool> cellVanishTable;

	for(int row = 0; row < mHeight; row++)
	{
		for(int col = 0; col <mWidth; col++)
		{
			Cell* currentCell = GetCell(row, col);
			if(currentCell && currentCell->GetCellState() == CS_PendingCheck)
			{
				if(cellVanishTable.find(currentCell) == cellVanishTable.cend())
				{
					cellVanishTable[currentCell] = CheckVanishWithCell(currentCell);
				}

				Cell* pairCell = currentCell->GetSwappedCell();
				if(pairCell && cellVanishTable.find(pairCell) == cellVanishTable.cend())
				{
					assert(pairCell->GetCellState() == CS_PendingCheck);
					cellVanishTable[pairCell] = CheckVanishWithCell(pairCell);
				}

				// Swap back if no vanish on each
				if(!cellVanishTable[currentCell]
					&& !cellVanishTable[pairCell])
				{
					currentCell->SwapWith(pairCell, true);
				}
				else
				{
					if(currentCell->GetCellState() == CS_PendingCheck)
					{
						currentCell->SetCellState(CS_Idle);
					}

					if(pairCell->GetCellState() == CS_PendingCheck)
					{
						pairCell->SetCellState(CS_Idle);
					}
				}

				// Clear vanish cells
				cellVanishTable.erase(currentCell);
				cellVanishTable.erase(pairCell);
			}
		}
	}
}


bool Grid::CheckVanishWithCell(Cell* cell)
{
	bool retval = false;

	int cellRow = cell->GetRow();
	int cellCol = cell->GetColumn();

	// Check match in row
	int colMin = max<int>(0, cellCol - 2);
	int colMax = min<int>(mWidth - gMinimumVanishCount, cellCol);
	for(int col = colMin; col <= colMax; col++)
	{
		if(CheckVanishFromCell(GetCell(cellRow, col), true))
		{
			retval = true;
		}
	}

	// Check match in column
	int rowMin = max<int>(0, cellRow - 2);
	int rowMax = min<int>(mHeight - gMinimumVanishCount, cellRow);
	for(int row = rowMin; row <= rowMax; row++)
	{
		if(CheckVanishFromCell(GetCell(row, cellCol), false))
		{
			retval = true;
		}
	}

	return retval;
}

bool Grid::CheckVanishFromCell(Cell* startCell, bool inRow)
{
	if(!startCell)
	{
		return false;
	}

	Cell* cellsToVanish[gMinimumVanishCount];
	int startCellRow = startCell->GetRow();
	int startCellColumn = startCell->GetColumn();
	if(inRow)
	{
		for(int i = 0; i < gMinimumVanishCount; i++)
		{
			int col = i + startCellColumn;
			cellsToVanish[i] = GetCell(startCellRow, col);

			if(!cellsToVanish[i]
				|| (i > 0
					&& cellsToVanish[i]->GetResourceID() != cellsToVanish[i - 1]->GetResourceID())
				|| (cellsToVanish[i]->GetCellState() != CS_Idle
					&& cellsToVanish[i]->GetCellState() != CS_Vanish
					&& cellsToVanish[i]->GetCellState() != CS_PendingCheck))
			{
				return false;
			}
		}
	}
	else
	{
		for(int i = 0; i < gMinimumVanishCount; i++)
		{
			int row = i + startCellRow;
			cellsToVanish[i] = GetCell(row, startCellColumn);

			if(!cellsToVanish[i]
			|| (i > 0
				&& cellsToVanish[i]->GetResourceID() != cellsToVanish[i - 1]->GetResourceID())
			|| (cellsToVanish[i]->GetCellState() != CS_Idle
				&& cellsToVanish[i]->GetCellState() != CS_Vanish
				&& cellsToVanish[i]->GetCellState() != CS_PendingCheck))
			{
				return false;
			}
		}
	}

	// Set all cells to Vanish
	for(int i = 0; i < gMinimumVanishCount; i++)
	{
		cellsToVanish[i]->SetCellState(CS_Vanish);
	}

	return true;
}