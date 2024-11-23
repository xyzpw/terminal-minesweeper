#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "display_utilities.h"

struct winsize getTerminalSize()
{
    struct winsize wSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wSize);
    return wSize;
}

Coordinate getCenterCoordinates(struct winsize wSize)
{
    Coordinate c;
    c.column = wSize.ws_col / 2;
    c.row = wSize.ws_row / 2;
    return c;
}
