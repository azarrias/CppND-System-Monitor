#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

namespace NCursesDisplay {
void Display(System& system, int n = 10);
void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
std::string ProgressBar(float percent);
std::string lpad(const std::string &str, int padded_length, char pad_char = ' ');
std::string rpad(const std::string &str, int padded_length, char pad_char = ' ');
};  // namespace NCursesDisplay

#endif