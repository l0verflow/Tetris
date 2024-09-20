#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int grid[HEIGHT][WIDTH] = {0};
int current[4][4];
int next[4][4];
int posX = 0, posY = 0;
int gameRunning = 1;
int lastMove = 0;

int pieces[7][4][4] =
{
  {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
  {{1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
  {{1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
  {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
  {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
  {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
  {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
};

void
init ()
{
  initscr();
  noecho();
  cbreak();
  timeout(100);
  keypad(stdscr, TRUE);
  srand(time(NULL));
  gen_nnp();
  gen_np();
}

void
fin ()
{
  endwin();
}

void
draw ()
{
  clear();
  int offset_x = (COLS - WIDTH * 2) / 2;
  int offset_y = (LINES - HEIGHT) / 2;

  for (int y = 0; y < HEIGHT; y++)
    {
      for (int x = 0; x < WIDTH; x++)
        {
          if (grid[y][x] == 1)
            {
              mvprintw(offset_y + y, offset_x + x * 2, "[]");
            }
          else
            {
              mvprintw(offset_y + y, offset_x + x * 2, " .");
            }
        }
    }

  for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 4; x++)
        {
          if (current[y][x] == 1)
            {
              mvprintw(offset_y + posY + y, offset_x + (posX + x) * 2, "[]");
            }
        }
    }

  show_np();
  refresh();
}

void
gen_np ()
{
  for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 4; x++)
        {
          current[y][x] = next[y][x];
        }
    }

  gen_nnp();
  posX = WIDTH / 2 - 2;
  posY = 0;
}

void
gen_nnp ()
{
  int type = rand() % 7;
  for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 4; x++)
        {
          next[y][x] = pieces[type][y][x];
        }
    }
}

void
show_np ()
{
  int offset_x = (COLS - WIDTH * 2) / 2 + WIDTH * 2 + 4;
  int offset_y = (LINES - HEIGHT) / 2;

  mvprintw(offset_y, offset_x, "Next piece:");
  for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 4; x++)
        {
          if (next[y][x] == 1)
            {
              mvprintw(offset_y + y + 1, offset_x + x * 2, "[]");
            }
          else
            {
              mvprintw(offset_y + y + 1, offset_x + x * 2, "  ");
            }
        }
    }
}

int
check_col (int dx, int dy)
{
  for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 4; x++)
        {
          if (current[y][x] == 1)
            {
              int nx = posX + x + dx;
              int ny = posY + y + dy;
              if (nx < 0 || nx >= WIDTH || ny >= HEIGHT
                  || (ny >= 0 && grid[ny][nx] == 1))
                {
                  return 1;
                }
            }
        }
    }
  return 0;
}

void
move_p (int dx, int dy)
{
  if (!check_col(dx, dy))
    {
      posX += dx;
      posY += dy;
    }
}

void
fix_p ()
{
  for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 4; x++)
        {
          if (current[y][x] == 1)
            {
              grid[posY + y][posX + x] = 1;
            }
        }
    }
}

void
rem_lines ()
{
  for (int y = 0; y < HEIGHT; y++)
    {
      int complete = 1;
      for (int x = 0; x < WIDTH; x++)
        {
          if (grid[y][x] == 0)
            {
              complete = 0;
              break;
            }
        }
      if (complete)
        {
          for (int j = y; j > 0; j--)
            {
              for (int x = 0; x < WIDTH; x++)
                {
                  grid[j][x] = grid[j - 1][x];
                }
            }
          for (int x = 0; x < WIDTH; x++)
            {
              grid[0][x] = 0;
            }
        }
    }
}

void
rot_p ()
{
  int newPiece[4][4] = {0};
  for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 4; x++)
        {
          newPiece[x][3 - y] = current[y][x];
        }
    }
  for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 4; x++)
        {
          current[y][x] = newPiece[y][x];
        }
    }
  if (check_col(0, 0))
    {
      for (int y = 0; y < 4; y++)
        {
          for (int x = 0; x < 4; x++)
            {
              current[y][x] = newPiece[3 - x][y];
            }
        }
    }
}

void
g_over ()
{
  mvprintw(LINES / 2, (COLS - 9) / 2, "Game Over!");
  refresh();
  usleep(2000);
  gameRunning = 0;
}

void
p_input ()
{
  int key = getch();
  switch (key)
    {
      case KEY_LEFT:
        move_p(-1, 0);
        lastMove = 1;
        break;
      case KEY_RIGHT:
        move_p(1, 0);
        lastMove = 2;
        break;
      case KEY_DOWN:
        move_p(0, 1);
        break;
      case KEY_UP:
        rot_p();
        break;
    }
}

int
main ()
{
  init();
  while (gameRunning)
    {
      p_input();
      if (check_col(0, 1))
        {
          fix_p();
          rem_lines();
          gen_np();
          if (check_col(0, 0))
            {
              g_over();
            }
        }
      else
        {
          move_p(0, 1);
        }
      draw();
      usleep(100000);
    }
  fin();
  return 0;
}
