#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

#include "./mine_field.h"

struct termios saved_attr;

#define COLOR_BLACK	"\033[30m"
#define COLOR_RED	"\033[31m"
#define COLOR_GREEN	"\033[32m"
#define COLOR_YELLOW	"\033[33m"
#define COLOR_BLUE	"\033[34m"
#define COLOR_MAGENTA	"\033[35m"
#define COLOR_CYAN	"\033[36m"
#define COLOR_WHITE	"\033[37m"
#define COLOR_RESET	"\033[0m"

typedef struct {
  int row;
  int col;
} Cursor;

typedef enum {
  GAME_STATE_PLAYING,
  GAME_STATE_LOSE,
  GAME_STATE_WON,
  GAME_STATE_IN_MENU,
  GAME_STATE_CONFIG,
  GAME_STATE_COUNT
} GameState;

typedef struct {
  Field field;
  Cursor cursor;
  GameState state;

  int true_flagged;
  int first_pick;

  int running;
  int stop;

  time_t timer;

  int cheat;
} Game;

void game_init(Game *game);
void game_set_input_mode(void);
void game_reset_input_mode(void);
void game_handle_input(Game *game, char c);
void game_start(Game *game);
void game_restart(Game *game);
void game_quit(Game *game);
void game_unpause(Game *game);

void game_move_cursor_up(Game *game);
void game_move_cursor_down(Game *game);
void game_move_cursor_left(Game *game);
void game_move_cursor_right(Game *game);

void game_clear_screen(Game *game);
void game_clear_everything();
Cell game_open_cell_at(Game *game, int row, int col);
void game_flag_cell_at(Game *game, int row, int col);
void game_open_neighbor_cell(Game *game, int row, int col);

void game_handle_open_cell(Game *game);
void game_handle_game_state(Game *game);
void game_handle_yes_no_question(Game *game, const char *question, void (*yes)(Game *game), void (*no)(Game *game));
void game_check_for_win(Game *game);

void game_reveal_everything(Game *game);
void game_reveal_only_mines(Game *game);
void game_unreveal_everything(Game *game);

void game_print(Game *game);

#endif // __GAME_H__
