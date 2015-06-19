#pragma once
#include "Grid.h"
#include "vector"
#include "string"

using namespace std;

class GridSimulator
{
public:
	GridSimulator();

	// Methods
	bool FindAvailableMove(Vector2D& cell1, Vector2D& cell2);
	void ImportGrid(Grid* grid);
	void ExportGrid(Grid* grid);
	void GenerateStartGrid();
	Vector2D GetSize();
private:
	vector<wstring> mSimGrid;
	int mGridWidth;
	int mGridHeight;
	
	bool CheckRowHasVanish(int row);
	bool CheckColumnHasVanish(int col);
	void ResizeGrid(int newWidth, int newHeight);
	bool FindMatchFromCell(int cellRow, int cellCol, bool inRow);
};