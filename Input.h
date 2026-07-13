#pragma once

#include <conio.h>

inline char getInput() { return static_cast<char>(_getch()); }
void waitForInput();
