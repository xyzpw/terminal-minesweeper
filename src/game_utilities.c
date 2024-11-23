#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "game_utilities.h"
#include "initializer.h"

bool checkGridCellExists(Grid*, Position);
bool checkAllCellsCleared(Grid*);

void openCell(Game* game, Grid* grid)
{
    Position p = {
        .row = grid->focusedCellPos.row,
        .column = grid->focusedCellPos.column
    };
    if (!checkGridCellExists(grid, p))
        return;

    if (game->isFirstClick){
        initCellMines(grid, p, game->difficulty);
        game->isFirstClick = false;
        game->startEpoch = time(NULL);
    }

    GridCell *cell = &grid->cells
        [
            grid->focusedCellPos.row][grid->focusedCellPos.column];
    if (cell->wasClicked || cell->isFlagged)
        return;
    if (cell->hasMine){
        cell->wasClicked = true;
        game->gameOver = true;
        game->hasWon = false;
        return;
    }
    openNearbyCells(grid, grid->focusedCellPos);
    if (checkAllCellsCleared(grid)){
        game->gameOver = true;
        game->hasWon = true;
    }
}

void openNearbyCells(Grid* grid, Position position)
{
    if (!checkGridCellExists(grid, position))
        return;

    GridCell *cell = &grid->cells[position.row][position.column];
    if (cell->wasClicked || cell->isFlagged){
        return;
    }

    cell->wasClicked = true;

    if (cell->nearbyMinesCount > 0){
        return;
    }

    for (int r = -1; r <= 1; ++r){
        for (int c = -1; c <= 1; ++c){
            if (r == 0 && c == 0)
                continue;
            Position newPos = {
                .row = position.row + r,
                .column = position.column + c
            };
            if (checkGridCellExists(grid, newPos))
                openNearbyCells(grid, newPos);
        }
    }

}

bool checkGridCellExists(Grid* grid, Position pos)
{
    bool invalidRow = pos.row > grid->rowCount - 1 || pos.row < 0;
    bool invalidCol = pos.column < 0 || pos.column > grid->columnCount - 1;
    if (invalidRow || invalidCol)
        return false;
    return true;
}

bool checkAllCellsCleared(Grid* grid)
{
    int activeCells = 0;
    int activeMines = 0;
    for (int r = 0; r < grid->rowCount; ++r)
    {
        for (int c = 0; c < grid->columnCount; ++c)
        {
            GridCell *cell = &grid->cells[r][c];
            if (cell->hasMine)
                activeMines++;
            if (!cell->wasClicked)
                activeCells++;
        }
    }
    return activeCells == activeMines;
}
