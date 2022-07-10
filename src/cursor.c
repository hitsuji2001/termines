#include "../header/cursor.h"

void cursor_move_up(Cursor *cursor) {
  cursor->row--;
}

void cursor_move_down(Cursor *cursor) {
  cursor->row++;
}

void cursor_move_left(Cursor *cursor) {
  cursor->col--;
}

void cursor_move_right(Cursor *cursor) {
  cursor->col++;
}
