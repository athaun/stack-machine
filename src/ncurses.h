#pragma once

#include "messageData.h"

// Indices for each color in ncurses
#define DEFAULT_BACKGROUND 0
#define YELLOW_COLOR 1
#define CYAN_COLOR 2
#define MAGENTA_COLOR 3
#define RED_COLOR 4
#define BLUE_COLOR 5
#define GREEN_COLOR 6
#define WHITE_COLOR 7

void setup_ncurses();
void input();
void output();
void event_listener();