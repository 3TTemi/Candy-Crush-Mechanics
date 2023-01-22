#include "CandyCrush2.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"

#include "testing/SimpleTest.h"
using namespace std;


CandyCrush2::CandyCrush2(int width, int height) {
    gridWidth = width;
    gridHeight = height;
    colorBoard.clear();
    colorBoard.resize(gridHeight, gridWidth);
    generateGrid();
}

void CandyCrush2::generateGrid()
{
    Vector<string> colors =  {"red", "blue", "green", "yellow"};
    for(int r = 0; r < colorBoard.numRows(); r++)
    {
        for(int c = 0; c < colorBoard.numCols(); c++)
        {
            string randomColor = randomElement(colors);
            colorBoard.set(r, c, randomColor);
        }
    }
}

void CandyCrush2::printBoard()
{
    string str = colorBoard.toString2D();
    cout << endl;
    cout << str;
}

Set<GridLocation> CandyCrush2::generateValidMoves(GridLocation loc)
{
    Set<GridLocation> moves;
    GridLocation posMove;
    for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            posMove.row = loc.row + x;
            posMove.col = loc.col + y;
            if (colorBoard.inBounds(posMove.row, posMove.col) && posMove != loc)
            {
                moves.add(posMove);
            }
        }
    }
    return moves;
}

bool CandyCrush2::validatePathHelper(Set<GridLocation> path,  Set<GridLocation> buildPath, GridLocation cur)
{
    buildPath.add(cur);

    if(buildPath == path)
    {
        return true;
    }

    Set<GridLocation> moves = generateValidMoves(cur);
    for (GridLocation move: moves)
    {
        if(path.contains(move) && !buildPath.contains(move))
        {
               return validatePathHelper(path, buildPath, move);
        }
    }

    buildPath.remove(cur);
    return false;

}

void CandyCrush2::validatePath(Set<GridLocation> path)
{
    GridLocation first = path.first();
    string expectedColor = colorBoard.get(first);

    if (path.size() < 3)
    {
        error("Path size is less than 3 candies");
    }

    for(GridLocation loc: path)
    {
        bool valid = false;
        if (colorBoard.get(loc) != expectedColor)
        {
            error("Different colors found in path");
        }
        if (!colorBoard.inBounds(loc))
        {
            error("A location in the path is not in bounds");
        }

        Set<GridLocation> buildPath;
        valid = validatePathHelper(path, buildPath ,path.first());
        if(!valid)
        {
            error("Path is invlaid");
        }
    }
}

void CandyCrush2::remove(int row, int col)
{
    if(colorBoard.inBounds(row,col))
    {
        colorBoard.set(row,col, "");
    }
    else
    {
       error("Trying to remove location not in bounds of the board");
    }
}


Grid<string> CandyCrush2::retrieveBoardState()
{
    return colorBoard;
}


void CandyCrush2::regenerateGrid(Set<GridLocation> path)
{
    Vector<string> colors =  {"red", "blue", "green", "yellow"};

    int pathSize = path.size();
    validatePath(path);

    for(GridLocation loc: path)
    {
        remove(loc.row, loc.col);
    }

    for(int i = 0; i < pathSize; i++)
    {
        GridLocation last = path.last();
        int checkR = last.row - 1;
        while(colorBoard.inBounds(checkR,last.col) && colorBoard[checkR][last.col] == "")
        {
            checkR--;
        }
        if(colorBoard.inBounds(checkR, last.col))
        {
            colorBoard[last] = colorBoard[checkR][last.col];
            colorBoard[checkR][last.col] = "";
        }
        path.remove(last);
    }


    int numFillRows = pathSize / gridWidth;
    numFillRows++;

    for(int r = 0; r < numFillRows; r++)
    {
        for(int c = 0; c < colorBoard.numCols(); c++)
        {
            if(colorBoard[r][c] == "")
            {
                string randomColor = randomElement(colors);
                colorBoard.set(r, c, randomColor);
            }
        }
    }

}

PROVIDED_TEST("Testing the generate Valid Moves function")
{
    CandyCrush2 candyBoard(3,3);
    Set<GridLocation> validMoves = candyBoard.generateValidMoves(GridLocation(0,0));
    EXPECT_EQUAL(validMoves.size(), 3);

    validMoves = candyBoard.generateValidMoves(GridLocation(1,1));
    EXPECT_EQUAL(validMoves.size(), 8);

    validMoves = candyBoard.generateValidMoves(GridLocation(1,2));
    EXPECT_EQUAL(validMoves.size(), 5);
}


PROVIDED_TEST("Testing the validatePath function")
{
    setRandomSeed(2);
    CandyCrush2 candyBoard(10,10);
    candyBoard.printBoard();
        Set<GridLocation> path;


    path.add(GridLocation(1,0));
    path.add(GridLocation(2,0));
    path.add(GridLocation(3,0));

    candyBoard.validatePath(path);

    path.clear();

    path.add(GridLocation(0,8));
    path.add(GridLocation(0,7));
    path.add(GridLocation(1,6));
    path.add(GridLocation(0,5));

    candyBoard.validatePath(path);

    path.clear();

    path.add(GridLocation(9,8));
    path.add(GridLocation(8,9));
    path.add(GridLocation(7,9));
    path.add(GridLocation(8,7));

    EXPECT_ERROR(candyBoard.validatePath(path));


}


PROVIDED_TEST("Testing regenerating the Grid")
{
    setRandomSeed(1);
    CandyCrush2 candyBoard(5,5);
    candyBoard.printBoard();

    Set<GridLocation> path;
    Vector<string> tempPath;
    path.add(GridLocation(0,4));
    path.add(GridLocation(1,3));
    path.add(GridLocation(1,2));
    path.add(GridLocation(2,1));
    path.add(GridLocation(3,1));
    path.add(GridLocation(4,1));


    candyBoard.regenerateGrid(path);
    Grid<string> board = candyBoard.retrieveBoardState();

    EXPECT(board[4][1] == "red" && board[1][2] == "yellow" && board[1][3] == "blue");
    EXPECT(board[0][2] != "" && board[0][3] != "" && board[0][4] != "");

}




