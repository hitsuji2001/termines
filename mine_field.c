#include "mine_field.h"

void field_init(Field *field, int rows, int cols) {
  field->cells = malloc(sizeof(Cell) * (rows *cols));

  field->rows = rows;
  field->cols = cols;
  for (int i = 0; i < rows * cols; ++i) {
    field->cells[i].object = OBJ_EMPTY;
    field->cells[i].state = STATE_UNEXPLORE;
  }

  field_randomize_bomb(field, MINE_PERCENTAGE);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      field_get_mines_cell_count_at(field, i, j);
    }
  }
}

void field_randomize_bomb(Field *field, int bomb_percentages) {
  if (bomb_percentages > 100) bomb_percentages = 100;
  else if (bomb_percentages < 0) bomb_percentages = 0;

  int bombs_count = field->rows * field->cols * bomb_percentages / 100;
  int row;
  int col;
  Cell cell = {0};

  field->total_mines = bombs_count;

  cell.object = OBJ_MINE;
  while (bombs_count > 0) {
    field_find_empty_space(field, &row, &col);
    field_set_cell_at(field, row, col, cell);
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

void field_find_empty_space(Field *field, int *row, int *col) {
  srand(time(NULL));
  Cell cell;
  while (1) {
     *row = rand() % field->rows;
     *col = rand() % field->cols;
     cell = field_get_cell_at(field, *row, *col);
     if (cell.object == OBJ_EMPTY) break;
     else continue;
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
