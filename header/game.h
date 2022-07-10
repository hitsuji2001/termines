#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

#include "./mine_field.h"
#include "./menu.h"

struct termios saved_attr;

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
  Menu menu;

  int mine_percentages;
  int rows;
  int cols;

  int true_flagged;
  int first_pick;

  int running;
  int stop;

  time_t timer;
} Game;

void game_init(Game *game, int rows, int cols);
void game_set_input_mode(void);
void game_reset_input_mode(void);
void game_handle_input(Game *game, char c);

void game_run(Game *game);
void game_start(Game *game);
void game_restart(Game *game);
void game_quit(Game *game);
void game_unpause(Game *game);

void game_clear_screen(Game *game);
void game_clear_everything();
Cell game_open_cell_at(Game *game, int row, int col);
void game_flag_cell_at(Game *game, int row, int col);
void game_open_neighbor_cell(Game *game, int row, int col);

void game_handle_open_cell(Game *game);
void game_handle_game_state(Game *game);
void game_handle_yes_no_question(Game *game, const char *question, void (*yes)(Game *game), void (*no)(Game *game));
void game_handle_win_lose_state(Game *game);
void game_check_for_win(Game *game);

void game_reveal_everything(Game *game);
void game_reveal_only_mines(Game *game);
void game_unreveal_everything(Game *game);

void game_print(Game *game);

#endif // __GAME_H__
