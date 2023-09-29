#pragma once

#include <iostream>
#include "ncurses.h"

struct MessageData {
    char message[2048] = { 0 };
    char author[48] = { 0 };
    char time[16] = { 0 };
    int colorIndex = { 0 };
};