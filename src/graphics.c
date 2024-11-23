#include <ncurses.h>
#include "graphics.h"

void setPairs()
{
    init_pair(CELL_FLAGGED_COLOR_PAIR, COLOR_YELLOW, -1);
    init_pair(NEARBY_MINE_COLOR_PAIR_LOW, COLOR_BLUE, -1);
    init_pair(NEARBY_MINE_COLOR_PAIR_MID, COLOR_GREEN, -1);
    init_pair(NEARBY_MINE_COLOR_PAIR_HIGH, COLOR_RED, -1);
}

int getNearbyMinesColorPair(int mines)
{
    if (mines >= 3)
        return NEARBY_MINE_COLOR_PAIR_HIGH;
    else if (mines == 2)
        return NEARBY_MINE_COLOR_PAIR_MID;

    return NEARBY_MINE_COLOR_PAIR_LOW;
}
