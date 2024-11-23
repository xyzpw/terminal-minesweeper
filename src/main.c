#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include "display_utilities.h"
#include "initializer.h"

int main(int argc, char** argv)
{
    srand(time(NULL));
    setlocale(LC_ALL, "");
    Game game = {.isActive = true};
    game.gameOver = false;
    game.difficulty = DIFFICULTY_EASY;
    game.isFirstClick = true;

    if (argc >= 2)
        game.difficulty = strToDifficulty(argv[1]);

    playMinesweeper(&game);
    return 0;
}
