#include <windows.h>
#include <stdio.h>
#include <pdcurses.h>
#include "gamedata.h"
//Function to sync window and console buffer
void syncConsoleBufferToWindow() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);

    // Visible window dimensions (not buffer dimensions)
    int windowWidth  = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    COORD newSize;
    newSize.X = windowWidth;
    newSize.Y = windowHeight;

    SetConsoleScreenBufferSize(hOut, newSize);
}
//Function to find offset for the purposes of centering the game's playfield
int offset_x = 0, offset_y = 0;
void update_playfield_offset(int max_x, int max_y){
offset_x = (max_x - PLAYFIELD_W) / 2;
offset_y = (max_y - PLAYFIELD_H) / 2;
if (offset_x < 0) offset_x = 0;
    if (offset_y < 0) offset_y = 0;
}
//Function to draw a border around the playfield
void draw_playfield_border(void) {
    setscrreg(offset_y, offset_y + PLAYFIELD_H - 1);
    int top    = offset_y - 1;
    int bottom = offset_y + PLAYFIELD_H;
    int left   = offset_x - 1;
    int right  = offset_x + PLAYFIELD_W;

    attron(COLOR_PAIR(7)); // Reusing your existing "background objects" color

    // Corners
    mvaddch(top,    left,  ACS_ULCORNER);
    mvaddch(top,    right, ACS_URCORNER);
    mvaddch(bottom, left,  ACS_LLCORNER);
    mvaddch(bottom, right, ACS_LRCORNER);

    // Top and bottom edges
    for (int x = left + 1; x < right; x++) {
        mvaddch(top, x, ACS_HLINE);
        mvaddch(bottom, x, ACS_HLINE);
    }

    // Left and right edges
    for (int y = top + 1; y < bottom; y++) {
        mvaddch(y, left, ACS_VLINE);
        mvaddch(y, right, ACS_VLINE);
    }

    attroff(COLOR_PAIR(7));
}
//Function to forcibly resize terminal, does not work currently
void forceConsoleSize(int cols, int rows) {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_RESTORE);  // un-maximize before resizing

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Step 1: shrink the window first so it never exceeds the current buffer
    SMALL_RECT shrinkRect = {0, 0, 1, 1};
    SetConsoleWindowInfo(hOut, TRUE, &shrinkRect);

    // Step 2: resize the buffer to the target size
    COORD newBufferSize;
    newBufferSize.X = cols;
    newBufferSize.Y = rows;
    SetConsoleScreenBufferSize(hOut, newBufferSize);

    // Step 3: resize the window to match (rect is inclusive, so subtract 1)
    SMALL_RECT windowRect = {0, 0, (SHORT)(cols - 1), (SHORT)(rows - 1)};
    SetConsoleWindowInfo(hOut, TRUE, &windowRect);
    
}
void forceConsoleSize_debug(int cols, int rows) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    // Log starting state
    GetConsoleScreenBufferInfo(hOut, &csbi);
    printf("BEFORE: buffer=%d x %d, window=(%d,%d)-(%d,%d)\n",
        csbi.dwSize.X, csbi.dwSize.Y,
        csbi.srWindow.Left, csbi.srWindow.Top,
        csbi.srWindow.Right, csbi.srWindow.Bottom);

    // Check the largest window size the current font/monitor allows
    COORD maxSize = GetLargestConsoleWindowSize(hOut);
    printf("Max allowed window size: %d x %d\n", maxSize.X, maxSize.Y);

    // Step 1: shrink window first so it never exceeds current buffer
    SMALL_RECT shrinkRect = {0, 0, 1, 1};
    BOOL ok1 = SetConsoleWindowInfo(hOut, TRUE, &shrinkRect);
    printf("Shrink window: %s (err=%lu)\n", ok1 ? "OK" : "FAILED", GetLastError());

    // Step 2: resize buffer
    COORD newBufferSize = {(SHORT)cols, (SHORT)rows};
    BOOL ok2 = SetConsoleScreenBufferSize(hOut, newBufferSize);
    printf("Resize buffer to %dx%d: %s (err=%lu)\n", cols, rows, ok2 ? "OK" : "FAILED", GetLastError());

    // Step 3: resize window to target
    SMALL_RECT windowRect = {0, 0, (SHORT)(cols - 1), (SHORT)(rows - 1)};
    BOOL ok3 = SetConsoleWindowInfo(hOut, TRUE, &windowRect);
    printf("Resize window to %dx%d: %s (err=%lu)\n", cols, rows, ok3 ? "OK" : "FAILED", GetLastError());

    // Log ending state
    GetConsoleScreenBufferInfo(hOut, &csbi);
    printf("AFTER: buffer=%d x %d, window=(%d,%d)-(%d,%d)\n",
        csbi.dwSize.X, csbi.dwSize.Y,
        csbi.srWindow.Left, csbi.srWindow.Top,
        csbi.srWindow.Right, csbi.srWindow.Bottom);

    // Pause so you can read the output before curses takes over the screen
    printf("Press Enter to continue...\n");
    getchar();
}