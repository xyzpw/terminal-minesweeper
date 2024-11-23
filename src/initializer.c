#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "initializer.h"

void setTouchingMinesCount(Grid*);
void initTouchingMinesCount(Grid*);
void initCellMines(Grid*, Position, DIFFICULTY);
int getMineCount(DIFFICULTY);
void setGridSize(Grid*, DIFFICULTY);

void freeGrid(Grid* grid)
{
    for (int i = 0; i < grid->rowCount; ++i){
        free(grid->cells[i]);
    }
    free(grid->cells);
}

void initGrid(Grid* grid, DIFFICULTY difficulty)
{
    grid->focusedCellPos.column = 0;
    grid->focusedCellPos.row = 0;
    setGridSize(grid, difficulty);
    grid->flagsRemaining = getMineCount(difficulty);

    // NOTE: Cells format is `cells[row][column] = Cell`.
    grid->cells = malloc(grid->rowCount * sizeof(GridCell*));
    for (int i = 0; i < grid->rowCount; ++i){
        grid->cells[i] = malloc(grid->columnCount * sizeof(GridCell));
    }

    for (int row = 0; row < grid->rowCount; ++row)
    {
        for (int col = 0; col < grid->columnCount; ++col)
        {
            grid->cells[row][col].isFlagged = DEFAULT_FLAGGED_STATE;
            grid->cells[row][col].wasClicked = DEFAULT_CLICKED_STATE;
            grid->cells[row][col].hasMine = DEFAULT_MINE_STATE;
        }
    }
}

// Use Fisher-Yates shuffle to randomize mines.
void initCellMines(Grid* grid, Position position, DIFFICULTY difficulty)
{
    int mineCount = getMineCount(difficulty);
    int cellCount = grid->columnCount * grid->rowCount;
    int *indices = malloc(cellCount * sizeof(int));
    for (int i = 0; i < cellCount; ++i)
        indices[i] = i;

    // Fisher-Yates shuffle to randomize.
    for (int i = cellCount - 1; i > 0; --i){
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    // Place mines but avoid the first click and its surrounding cells.
    for (int i = 0; i < mineCount; ++i)
    {
        int mineIndex = indices[i];
        int row = mineIndex / grid->columnCount;
        int col = mineIndex % grid->columnCount;

        // Skip the first clicked cell and the cells near it.
        if ((row >= position.row - 1 && row <= position.row + 1) &&
            (col >= position.column - 1 && col <= position.column + 1))
        {
            int temp = indices[i];
            indices[i] = indices[cellCount - 1];
            indices[cellCount - 1] = temp;
            cellCount--;
            --i;
            continue;
        }

        grid->cells[row][col].hasMine = true;
    }
    free(indices);
    initTouchingMinesCount(grid);
}

int getNearbyMinesCount(Grid* grid, Position pos)
{
    int mineCount = 0;
    int row = pos.row;
    int column = pos.column;

    for (int i = -1; i <= 1; ++i)
        for (int j = -1; j <= 1; ++j){
            if (i == 0 && j == 0)
                continue;
            if (row + i > grid->rowCount - 1 || row + i < 0)
                continue;
            else if (column + j < 0 || column + j > grid->columnCount - 1)
                continue;
            if (grid->cells[row + i][column + j].hasMine)
                ++mineCount;
        }
    return mineCount;
}

// Compute how many mines are touching each cell.
void initTouchingMinesCount(Grid* grid)
{
    for (int row = 0; row < grid->rowCount; ++row)
    {
        for (int col = 0; col < grid->columnCount; ++col)
        {
            GridCell *cell = &grid->cells[row][col];
            Position pos = {.row=row, .column=col};
            cell->nearbyMinesCount = getNearbyMinesCount(grid, pos);
        }
    }
}

int getMineCount(DIFFICULTY difficulty)
{
    switch (difficulty){
        case DIFFICULTY_EASY:
            return DEFAULT_EASY_MINE_COUNT;
        case DIFFICULTY_MODERATE:
            return DEFAULT_MODERATE_MINE_COUNT;
        case DIFFICULTY_EXPERT:
            return DEFAULT_EXPERT_MINE_COUNT;
        default:
            return 0;
    }
}

void setGridSize(Grid* grid, DIFFICULTY difficulty)
{
    GRID_SIZE gridSize;
    switch (difficulty){
        case DIFFICULTY_EASY:
            gridSize = EASY_GRID_SIZE;
            break;
        case DIFFICULTY_MODERATE:
            gridSize = MODERATE_GRID_SIZE;
            break;
        case DIFFICULTY_EXPERT:
            gridSize = EXPERT_GRID_SIZE;
            break;
    }
    grid->columnCount = gridSize.columns;
    grid->rowCount = gridSize.rows;
}

DIFFICULTY strToDifficulty(const char* str)
{
    if (strcmp(str, "easy") == 0)
        return DIFFICULTY_EASY;
    else if (strcmp(str, "moderate") == 0)
        return DIFFICULTY_MODERATE;
    else if (strcmp(str, "expert") == 0)
        return DIFFICULTY_EXPERT;
    return DIFFICULTY_EASY;
}
