#pragma once
#include "grid.h"
#include "gridlocation.h"
#include "set.h"
#include <string>


class CandyCrush {

public:

CandyCrush(int width, int height);


Grid<std::string> retrieveBoardState();

void printBoard();


void remove(int row, int col);

Set<GridLocation> generateValidMoves(GridLocation loc);

void validatePath(Set<GridLocation> path);

void doGravity();

void regenerateGrid(Set<GridLocation> path);

private:

Grid<std::string> colorBoard;

int gridHeight;

int gridWidth;

void generateGrid();


};



