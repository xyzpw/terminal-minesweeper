#pragma once

#define CELL_FLAGGED_COLOR_PAIR 1
#define NEARBY_MINE_COLOR_PAIR_LOW 2
#define NEARBY_MINE_COLOR_PAIR_MID 3
#define NEARBY_MINE_COLOR_PAIR_HIGH 4

int getNearbyMinesColorPair(int mines);
void setPairs();
