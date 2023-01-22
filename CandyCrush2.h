#pragma once
#include "grid.h"
#include "gridlocation.h"
#include "set.h"
#include <string>


class CandyCrush2 {

public:

CandyCrush2(int width, int height);


Grid<std::string> retrieveBoardState();

void printBoard();


void remove(int row, int col);

Set<GridLocation> generateValidMoves(GridLocation loc);

void validatePath(Set<GridLocation> path);

bool validatePathHelper(Set<GridLocation> path, Set<GridLocation> buildPath, GridLocation cur);

void regenerateGrid(Set<GridLocation> path);

private:

Grid<std::string> colorBoard;

int gridHeight;

int gridWidth;

void generateGrid();


};



