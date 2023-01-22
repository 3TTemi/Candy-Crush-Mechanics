#include "CandyCrush.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
#include "stack.h"
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;


CandyCrush::CandyCrush(int width, int height) {
    gridWidth = width;
    gridHeight = height;
    colorBoard.clear();
    colorBoard.resize(gridHeight, gridWidth);
    generateGrid();
}

void CandyCrush::generateGrid()
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

void CandyCrush::printBoard()
{
    string str = colorBoard.toString2D();
    cout << endl;
    cout << str;
}

Set<GridLocation> CandyCrush::generateValidMoves(GridLocation loc)
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



void CandyCrush::validatePath(Set<GridLocation> path)
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

    }

    bool valid = false;

    Set<GridLocation> buildPath;
    Queue<Set<GridLocation>> allPaths;

    buildPath.add(path.first());
    allPaths.enqueue(buildPath);
    Map<Set<GridLocation>, GridLocation> lastMap;
    lastMap[buildPath] = path.first();

    while (!allPaths.isEmpty())
    {
         Set<GridLocation> curPath = allPaths.dequeue();

         if(curPath == path)
         {
            valid = true;
         }

         Set<GridLocation> moves = generateValidMoves(lastMap[curPath]);

         for(GridLocation move: moves)
         {
            if(!curPath.contains(move) && path.contains(move))
            {
                Set<GridLocation> newPath = curPath;
                newPath.add(move);
                lastMap.put(newPath, move);
                allPaths.enqueue(newPath);
            }
         }
    }

    if (valid == false)
    {
        error("Path is invalid");
    }
}



void CandyCrush::remove(int row, int col)
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


Grid<string> CandyCrush::retrieveBoardState()
{
    return colorBoard;
}

void CandyCrush::doGravity()
{
    for(int r = colorBoard.numRows() - 1; r >= 0 ; r--)
    {
        for(int c = 0 ; c < colorBoard.numCols() ; c++)
        {
            if (colorBoard[r][c] != "")
            {
                int checkR = r + 1;
                while(colorBoard.inBounds(checkR,c) && colorBoard[checkR][c] == "")
                {
                    checkR++;
                }
                checkR--;
                if(r != checkR)
                {
                    colorBoard[checkR][c] = colorBoard[r][c];
                    colorBoard[r][c] = "";
                }
            }

        }
    }
}

void CandyCrush::regenerateGrid(Set<GridLocation> path)
{
    Vector<string> colors =  {"red", "blue", "green", "yellow"};

    validatePath(path);

    for(GridLocation loc: path)
    {
        remove(loc.row, loc.col);
    }

    doGravity();

    for(int r = colorBoard.numRows() - 1; r >= 0 ; r--)
    {
        for(int c = 0 ; c < colorBoard.numCols() ; c++)
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
    CandyCrush candyBoard(3,3);
    Set<GridLocation> validMoves = candyBoard.generateValidMoves(GridLocation(0,0));
    EXPECT_EQUAL(validMoves.size(), 3);

    validMoves = candyBoard.generateValidMoves(GridLocation(1,1));
    EXPECT_EQUAL(validMoves.size(), 8);

    validMoves = candyBoard.generateValidMoves(GridLocation(1,2));
    EXPECT_EQUAL(validMoves.size(), 5);
}

PROVIDED_TEST("Testing the doGravity function")
{
    setRandomSeed(0);
    CandyCrush candyBoard(3,3);
    candyBoard.remove(0,1);
    candyBoard.remove(0,2);
    candyBoard.remove(2,0);
    candyBoard.remove(2,1);
    candyBoard.remove(2,2);
    candyBoard.doGravity();

    Grid<string> board = candyBoard.retrieveBoardState();
    EXPECT(board[0][0] == "" && board[0][1] == "" && board[0][2] == "" && board[1][1] == "" && board[1][2] == "");
    EXPECT(board[1][0] == "red" && board[2][0] == "blue" && board[2][1] == "red" && board[2][2] == "yellow");



    CandyCrush board2(10,10);
    for(int i = 0; i < 10; i++)
    {
        board2.remove(1, i);
    }
    board2.doGravity();
    board = board2.retrieveBoardState();

    for(int i = 0; i < 10; i++)
    {
        EXPECT_EQUAL(board[0][i], "");
    }

    CandyCrush board3(10,10);
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            board3.remove(i,j);
        }
    }
    board3.doGravity();

    board = board3.retrieveBoardState();

    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            EXPECT_EQUAL(board[i][j], "");
        }
    }




}

PROVIDED_TEST("Testing the validatePath function")
{
    setRandomSeed(2);
    CandyCrush candyBoard(10,10);
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
    CandyCrush candyBoard(5,5);
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




