#include "mine_field.h"

void field_init(Field *field, int rows, int cols) {
  field->cells = malloc(sizeof(Cell) * (rows *cols));

  field->rows = rows;
  field->cols = cols;
  for (int i = 0; i < rows * cols; ++i) {
    field->cells[i].object = OBJ_EMPTY;
    field->cells[i].state = STATE_UNEXPLORE;
  }
}

void field_find_empty_space(Field *field, int *row, int *col, int nrow, int ncol) {
  srand(time(NULL));
  Cell cell;
  while (1) {
     *row = rand() % field->rows;
     *col = rand() % field->cols;
     cell = field_get_cell_at(field, *row, *col);
     if (cell.object == OBJ_EMPTY && !field_check_is_in_neighbor(*row, *col, nrow, ncol)) break;
     else if (*row == nrow && *col == ncol) continue;
     else if (field_check_is_in_neighbor(*row, *col, nrow, ncol)) continue;
     else continue;
  }
}

void field_randomize_bomb(Field *field, int row, int col, int bomb_percentages) {
  bomb_percentages = field_cap_bomb_bomb_percentages(bomb_percentages);

  int bombs_count = field->rows * field->cols * bomb_percentages / 100;
  int drow;
  int dcol;
  Cell cell = {0};

  field->total_mines = bombs_count;
  if (field->rows * field->cols <= 9) bombs_count = 0;

  cell.object = OBJ_MINE;
  while (bombs_count > 0) {
    field_find_empty_space(field, &drow, &dcol, row, col);
    field_set_cell_at(field, drow, dcol, cell);
    bombs_count--;
  }
}

void field_get_mines_cell_count_at(Field *field, int row, int col) {
  int bombs_count = 0;
  Cell cell = field_get_cell_at(field,row, col);
  bombs_count += field_get_neighbor_mines_count_at_cell(field, row, col);
  cell.mines_count = bombs_count;
  field_set_cell_at(field, row, col, cell);
}

int field_get_neighbor_mines_count_at_cell(Field *field, int row, int col) {
  Cell cell;
  int bombs_count = 0;

  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      if (i == 0 && j == 0) continue;
      cell = field_get_cell_at(field, row + i, col +j);
      if (cell.object == OBJ_MINE) bombs_count++;
    }
  }

  return bombs_count;
}

Cell field_get_cell_at(Field *field, int row, int col) {
  if (row < field->rows && col < field->cols && row >= 0 && col >= 0) {
    return field->cells[row + field->rows * col];
  }
  return (Cell) {0};
}

void field_set_cell_at(Field *field, int row, int col, Cell cell) {
  if (row < field->rows && col < field->cols && row >= 0 && col >= 0) {
    field->cells[row + field->rows * col] = cell;
  }
}

void field_print(Field *field) {
  Cell cell;
  for (int i = 0; i < field->rows; ++i) {
    for (int j = 0; j < field->cols; ++j) {
      cell = field_get_cell_at(field, i, j);
      if (cell.state == STATE_UNEXPLORE) {
	printf("# ");
      } else {
	if (cell.object == OBJ_MINE) {
	  printf("* ");
	} else if (cell.object == OBJ_EMPTY) {
	  if (cell.mines_count != 0) printf("%d ", cell.mines_count);
	  else printf("  ");
	} else {
	  printf("Unreachable\n");
	  break;
	}
      }
    }
    printf("\n");
  }
}

void field_free(Field *field) {
  free(field->cells);
}

int field_check_is_in_neighbor(int drow, int dcol, int row, int col) {
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      if (drow == row + i && dcol == col + j) return 1;
    }
  }
  return 0;
}

int field_cap_bomb_bomb_percentages(int bomb_percentages) {
  if (bomb_percentages < 0) bomb_percentages = 0;
  else if (bomb_percentages < 80);
  else {
    float temp = 80.f / 100.f;
    temp = 0.25f * temp * temp * temp + 0.5;
    temp *= 100;
    bomb_percentages = (int)temp;
  }

  return bomb_percentages;
}
