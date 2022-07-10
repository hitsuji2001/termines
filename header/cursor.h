#ifndef __CURSOR_H__
#define __CURSOR_H__

typedef struct {
  int row;
  int col;
} Cursor;

void cursor_move_up(Cursor *cursor);
void cursor_move_down(Cursor *cursor);
void cursor_move_left(Cursor *cursor);
void cursor_move_right(Cursor *cursor);


#endif // __CURSOR_H__
