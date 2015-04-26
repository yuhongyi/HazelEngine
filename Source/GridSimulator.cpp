#include "stdafx.h"
#include "GridSimulator.h"

GridSimulator::GridSimulator() :
	mSimGrid(NULL),
	mGridWidth(0),
	mGridHeight(0)
{
}

GridSimulator::~GridSimulator()
{
	SAFE_DELETE_ARRAY(mSimGrid);
}

void GridSimulator::ImportGrid(Grid* grid)
{
	if(grid)
	{
		Vector2D gridSize = grid->GetSize();

		ResizeGrid((int)gridSize.X, (int)gridSize.Y);

		for(int row = 0; row < mGridHeight; row++)
		{
			for(int col = 0; col < mGridWidth; col++)
			{
				mSimGrid[row * mGridWidth + col] = grid->GetCell(row, col)->GetResourceID();
			}
		}
	}
}

void GridSimulator::ExportGrid(Grid* grid)
{
	if(grid && grid->GetSize() == this->GetSize())
	{
		for(int row = 0; row < mGridHeight; row++)
		{
			for(int col = 0; col < mGridWidth; col++)
			{
				grid->GetCell(row, col)->SetResourceID(mSimGrid[row * mGridWidth + col]);
			}
		}
	}
	else
	{
		wprintf(L"Export Grid failed.\n"
				L"Size of Grid:(Width=%d, Height=%d) doesn't match\n"
				L"Size of GridSimulator(Width=%d, Height=%d)",
				(int)grid->GetSize().X, (int)grid->GetSize().Y,
				mGridWidth, mGridHeight);
	}
}

void GridSimulator::GenerateStartGrid()
{
	// Estimate max iteration
	const int max_iteration = gGridWidth * gGridHeight / gMinimumVanishCount * 10;
	int iteration_count = 0;
	bool foundMatch;

	do
	{
		foundMatch = false;

		// Go through row
		for(int row = 0; row < mGridWidth; row++)
		{
			for(int col = 0; col <= mGridHeight - gMinimumVanishCount; col++)
			{
				if(FindMatchFromCell(row, col, true))
				{
					// Switch current cell with the cell in next row
					std::swap(mSimGrid[row * mGridWidth + col], mSimGrid[(row + 1) % mGridHeight * mGridWidth + col]);
					foundMatch = true;
				}
			}
		}

		// Go through column
		for(int row = 0; row <= mGridWidth - gMinimumVanishCount; row++)
		{
			for(int col = 0; col < mGridHeight; col++)
			{
				if(FindMatchFromCell(row, col, false))
				{
					// Switch current cell with the cell in next column
					std::swap(mSimGrid[row * mGridWidth + col], mSimGrid[row * mGridWidth + (col + 1) % mGridHeight]);
					foundMatch = true;
				}
			}
		}

		// Gor through column
		// increment iteration count
		iteration_count++;
	}
	while(iteration_count < max_iteration && foundMatch == true);
	wprintf(L"Count:%d", iteration_count);
}

bool GridSimulator::FindMatchFromCell(int cellRow, int cellCol, bool inRow)
{
	if(inRow)
	{
		if(cellCol + gMinimumVanishCount > mGridWidth)
		{
			return false;
		}
		else
		{
			for(int i = 0; i < gMinimumVanishCount - 1; i++)
			{
				if(mSimGrid[cellRow * mGridWidth + cellCol + i] != mSimGrid[cellRow * mGridWidth + cellCol + i + 1])
				{
					return false;
				}
			}
		}
	}
	else
	{
		if(cellRow + gMinimumVanishCount > mGridHeight)
		{
			return false;
		}
		else
		{
			for(int i = 0; i < gMinimumVanishCount - 1; i++)
			{
				if(mSimGrid[(cellRow + i) * mGridWidth + cellCol] != mSimGrid[(cellRow + i + 1) * mGridWidth + cellCol])
				{
					return false;
				}
			}
		}

	}

	return true;
}

Vector2D GridSimulator::GetSize()
{
	return Vector2D((float)mGridWidth, (float)mGridHeight);
}

bool GridSimulator::FindAvailableMove(Vector2D& cell1, Vector2D& cell2)
{
	for(int row = 0; row < mGridWidth - 1; row++)
	{
		for(int col = 0; col < mGridHeight - 1; col++)
		{
			// Switch down and check
			std::swap(mSimGrid[row * mGridHeight + col], mSimGrid[(row + 1) * mGridHeight + col]);
			if(CheckRowHasVanish(row) || CheckRowHasVanish(row + 1))
			{
				cell1.X = (float)col;
				cell1.Y = (float)row;
				cell2.X = (float)col;
				cell2.Y = (float)row + 1;

				return true;
			}
			std::swap(mSimGrid[row * mGridHeight + col], mSimGrid[(row + 1) * mGridHeight + col]);

			// Switch right and check
			std::swap(mSimGrid[row * mGridHeight + col], mSimGrid[row  * mGridHeight + col + 1]);
			if(CheckColumnHasVanish(col) || CheckColumnHasVanish(col + 1))
			{
				cell1.X = (float)col;
				cell1.Y = (float)row;
				cell2.X = (float)col + 1;
				cell2.Y = (float)row;

				return true;
			}
			std::swap(mSimGrid[row * mGridHeight + col], mSimGrid[row * mGridHeight + col + 1]);
		}
	}

	return false;
}

bool GridSimulator::CheckRowHasVanish(int row)
{
	for(int col = 0; col <= mGridWidth - gMinimumVanishCount; col++)
	{
		if(mSimGrid[row * mGridHeight + col] == mSimGrid[row * mGridHeight + col + 1]
		&& mSimGrid[row * mGridHeight + col] == mSimGrid[row * mGridHeight + col + 2])
		{
			return true;
		}
	}

	return false;
}

bool GridSimulator::CheckColumnHasVanish(int col)
{
	for(int row = 0; row <= mGridHeight - gMinimumVanishCount; row++)
	{
		if(mSimGrid[row * mGridHeight + col] == mSimGrid[(row + 1) * mGridHeight + col]
		&& mSimGrid[row * mGridHeight + col] == mSimGrid[(row + 2) * mGridHeight + col])
		{
			return true;
		}
	}

	return false;
}

void GridSimulator::ResizeGrid(int newWidth, int newHeight)
{
	if(newWidth * newHeight == 0)
	{
		return;
	}

	mGridWidth = newWidth;
	mGridHeight = newHeight;
	SAFE_DELETE_ARRAY(mSimGrid);
	mSimGrid = new int[newWidth * newHeight];
}