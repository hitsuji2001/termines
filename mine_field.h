#ifndef __MINE_FIELD_H__
#define __MINE_FIELD_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MINE_PERCENTAGE 15
#define FIELD_SIZE 25

typedef enum {
  STATE_UNEXPLORE,
  STATE_PLAYER_EXPLORED,
  STATE_FLAGGED,
  STATE_EXPLODE,
  STATE_TRUE_FLAGGED,
  STATE_EXPLORED,
  STATE_COUNT
} State;

typedef enum {
  OBJ_EMPTY,
  OBJ_MINE,
  OBJ_COUNT
} Object;

typedef struct {
  Object object;
  int mines_count;
  State state;
} Cell;

typedef struct {
  Cell *cells;
  int total_mines;
  int rows;
  int cols;
} Field;

void field_init(Field *field, int rows, int cols);
void field_randomize_bomb(Field *field, int row, int col, int bomb_percentages);
void field_free(Field *field);

Cell field_get_cell_at(Field *field, int row, int col);
void field_set_cell_at(Field *field, int row, int col, Cell cell);

void field_find_empty_space(Field *field, int *row, int *col, int nrow, int ncol);
void field_get_mines_cell_count_at(Field *field, int row, int col);
int field_get_neighbor_mines_count_at_cell(Field *field, int row, int col);
int field_check_is_in_neighbor(int drow, int dcol, int row, int col);
int field_cap_bomb_bomb_percentages(int bomb_percentages);

void field_print(Field *field);

#endif // __MINE_FIELD_H__
