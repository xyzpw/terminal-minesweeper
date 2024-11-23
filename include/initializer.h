#pragma once

#include <stdbool.h>
#include <time.h>

#define DEFAULT_FLAGGED_STATE false
#define DEFAULT_CLICKED_STATE false
#define DEFAULT_MINE_STATE false

#define DEFAULT_EASY_MINE_COUNT 10
#define DEFAULT_MODERATE_MINE_COUNT 40
#define DEFAULT_EXPERT_MINE_COUNT 99

typedef enum {
    DIFFICULTY_EASY,
    DIFFICULTY_MODERATE,
    DIFFICULTY_EXPERT,
} DIFFICULTY;

typedef struct {
    int columns;
    int rows;
} GRID_SIZE;

#define EASY_GRID_SIZE (GRID_SIZE){.columns=9, .rows=9}
#define MODERATE_GRID_SIZE (GRID_SIZE){.columns=16, .rows=16}
#define EXPERT_GRID_SIZE (GRID_SIZE){.columns=30, .rows=16}

typedef struct {
    int row;
    int column;
} Position;

typedef struct {
    bool hasMine;
    bool isFlagged;
    int nearbyMinesCount;
    bool wasClicked;
} GridCell;

typedef struct {
    GridCell **cells;
    int columnCount;
    int rowCount;
    int flagsRemaining;
    Position focusedCellPos;
} Grid;

typedef struct {
    bool isActive;
    bool gameOver;
    bool hasWon;
    bool isFirstClick;
    time_t startEpoch;
    DIFFICULTY difficulty;
} Game;

void initGrid(Grid*, DIFFICULTY);
void freeGrid(Grid*);

void setTouchingMinesCount(Grid*);
int getNearbyMinesCount(Grid*, Position);
void initCellMines(Grid*, Position, DIFFICULTY);
void initTouchingMinesCount(Grid*);
DIFFICULTY strToDifficulty(const char*);
