#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>

#define HEIGHT 20
#define WIDTH 10

extern int grid[HEIGHT][WIDTH];
extern int current[4][4];
extern int next[4][4];
extern int posX;
extern int posY;
extern int gameRunning;
extern int lastMove;
extern int pieces[7][4][4];

void init();
void fin();
void draw();
void gen_np();
void gen_nnp();
void show_np();
int check_col(int dx, int dy);
void move_p(int dx, int dy);
void fix_p();
void rem_lines();
void rot_p();
void g_over();
void p_input();

#endif
