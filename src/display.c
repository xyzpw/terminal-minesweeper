#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include "game_utilities.h"
#include "display_utilities.h"
#include "initializer.h"
#include "graphics.h"
#include "display_helper.h"

void drawGrid(Grid*, int playtime);
void handleKeyPress(Game*, Grid*, char);
void drawGameOver(Game*, Grid*, int playtime);

void playMinesweeper(Game* game)
{
    Grid grid;
    initGrid(&grid, game->difficulty);

    initscr();
    nodelay(stdscr, 1);
    noecho();
    keypad(stdscr, 1);
    curs_set(0);
    use_default_colors();
    start_color();
    setPairs();

    int playtimeSeconds = 0;

    // Display game to screen.
    while (game->isActive)
    {
        erase();
        char pressedKey = getch();

        if (pressedKey == 'q'){
            game->isActive = false;
        }

        handleKeyPress(game, &grid, pressedKey);

        if (game->gameOver){
            drawGameOver(game, &grid, playtimeSeconds);
        }
        else{
            drawGrid(&grid, playtimeSeconds);
        }

        refresh();

        if (!game->gameOver && !game->isFirstClick)
            playtimeSeconds = time(NULL) - game->startEpoch;

        napms(40);
    }

    endwin();
    freeGrid(&grid);
}

void handleKeyPress(Game* game, Grid* grid, char key)
{
    switch (key){
        case 'd':
        case 'D':
            if (grid->focusedCellPos.column + 1 < grid->columnCount)
                grid->focusedCellPos.column++;
            else
                grid->focusedCellPos.column = 0;
            break;
        case 'a':
        case 'A':
            if (grid->focusedCellPos.column - 1 >= 0)
                grid->focusedCellPos.column--;
            else
                grid->focusedCellPos.column = grid->columnCount - 1;
            break;
        case 'w':
        case 'W':
            if (grid->focusedCellPos.row - 1 >= 0)
                grid->focusedCellPos.row--;
            else
                grid->focusedCellPos.row = grid->rowCount - 1;
            break;
        case 's':
        case 'S':
            if (grid->focusedCellPos.row + 1 < grid->rowCount)
                grid->focusedCellPos.row++;
            else
                grid->focusedCellPos.row = 0;
            break;
        case 'f':
        case 'F':
            GridCell* cell = &grid->cells[
                grid->focusedCellPos.row][grid->focusedCellPos.column];
            if (cell->wasClicked ||
                    (grid->flagsRemaining <= 0 && !cell->isFlagged))
                break;
            cell->isFlagged = cell->isFlagged ? false : true;
            if (cell->isFlagged)
                --grid->flagsRemaining;
            else
                ++grid->flagsRemaining;
            break;
        case ' ':
            openCell(game, grid);
            break;
    }
}

void drawGrid(Grid* grid, int playtime)
{
    for (int row = 0; row < grid->rowCount; ++row)
    {
        for (int col = 0; col < grid->columnCount; ++col)
        {
            GridCell *cell = &grid->cells[row][col];

            bool isCellFocused = grid->focusedCellPos.row == row &&
                    grid->focusedCellPos.column == col;

            Position currentPosition = {.row=row, .column=col};
            if (isCellFocused){
                displayFocusedCell(cell, currentPosition);
            }
            else {
                displayUnfocusedCell(cell, currentPosition);
            }

        }
    }
    char mineCountStr[10];
    sprintf(mineCountStr, "Flags: %d", grid->flagsRemaining);
    mvprintw(grid->rowCount + 1, 0, mineCountStr);
    char clockStr[5];
    sprintf(clockStr, "%02d:%02d", playtime / 60, playtime - (playtime / 60));
    mvprintw(grid->rowCount + 2, 0, clockStr);
}

void drawGameOver(Game* game, Grid* grid, int playtime)
{
    for (int row = 0; row < grid->rowCount; ++row)
    {
        for (int col = 0; col < grid->columnCount; ++col)
        {

            GridCell *cell = &grid->cells[row][col];

            if (cell->wasClicked && !game->hasWon){
                mvprintw(row, col, cell->hasMine ?
                         MINE_CHARACTER : EMPTY_CELL_CHAR);
            }
            else if (!game->hasWon){
                mvprintw(row, col, cell->hasMine ? MINE_CHARACTER : CELL_CHAR);
            }

            if (game->hasWon){
                mvprintw(row, col, EMPTY_CELL_CHAR);
            }
        }
    }
    const char* msg = game->hasWon ? "You won" : "You opened a mine cell!";
    mvprintw(grid->rowCount + 1, 0, msg);
    char clockStr[5];
    sprintf(clockStr, "%02d:%02d", playtime / 60, playtime - (playtime / 60));
    mvprintw(grid->rowCount + 2, 0, clockStr);
}
