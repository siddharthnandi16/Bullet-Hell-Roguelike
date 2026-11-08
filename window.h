#ifndef WINDOW_H
#define WINDOW_H
extern int offset_x;
extern int offset_y;
void syncConsoleBufferToWindow();
void forceConsoleSize(int cols, int rows);
void forceConsoleSize_debug(int cols, int rows) ;
void update_playfield_offset(int max_x, int max_y);
void draw_playfield_border(void);
#endif