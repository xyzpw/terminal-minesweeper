#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "display_helper.h"
#include "game_utilities.h"
#include "graphics.h"
#include "display_utilities.h"

void displayUnfocusedCell(GridCell* cell, Position position)
{
    int row = position.row;
    int col = position.column;

    int nearbyMinesCount = cell->nearbyMinesCount;

    if (cell->wasClicked)
    {
        if (cell->nearbyMinesCount > 0){
            char mineCountStr[1];
            sprintf(mineCountStr, "%d", nearbyMinesCount);

            attron(COLOR_PAIR(getNearbyMinesColorPair(nearbyMinesCount)));

            mvprintw(row, col, mineCountStr);

            attroff(COLOR_PAIR(getNearbyMinesColorPair(nearbyMinesCount)));
        }
        else {
            mvprintw(row, col, EMPTY_CELL_CHAR);
        }
    }
    else if (cell->isFlagged)
    {
        attron(COLOR_PAIR(CELL_FLAGGED_COLOR_PAIR));

        mvprintw(row, col, FLAG_CHAR);

        attroff(COLOR_PAIR(CELL_FLAGGED_COLOR_PAIR));
    }
    else
    {
        mvprintw(row, col, CELL_CHAR);
    }
}

void displayFocusedCell(GridCell* cell, Position position)
{
    int row = position.row;
    int col = position.column;

    int nearbyMinesCount = cell->nearbyMinesCount;

    if (cell->wasClicked)
    {
        if (cell->nearbyMinesCount > 0){
            char mineCountStr[1];
            sprintf(mineCountStr, "%d", nearbyMinesCount);

            attron(A_BOLD);

            mvprintw(row, col, mineCountStr);

            attroff(A_BOLD);
        }
        else {
            mvprintw(row, col, FOCUSED_CELL_CHAR);
        }
    }
    else if (cell->isFlagged)
    {
        attron(A_BOLD);

        mvprintw(row, col, FLAG_CHAR);

        attroff(A_BOLD);
    }
    else
    {
        mvprintw(row, col, FOCUSED_CELL_CHAR);
    }
}
