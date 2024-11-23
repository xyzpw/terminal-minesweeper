#pragma once

#include <unistd.h>
#include <sys/ioctl.h>
#include "initializer.h"

#define MINE_CHARACTER "X"

#define CELL_CHAR "\u25a1"
#define FOCUSED_CELL_CHAR "\u25a3"
#define EMPTY_CELL_CHAR " "

#define FLAG_CHAR "\u00de"

void playMinesweeper(Game*);

typedef struct {
    int column;
    int row;
} Coordinate;

struct winsize getTerminalSize();
Coordinate getCenterCoordinates(struct winsize termSize);
