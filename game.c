#include "./game.h"

void game_init(Game *game) {
  field_init(&game->field, FIELD_SIZE, FIELD_SIZE);
  game->cursor.row = 0;
  game->cursor.col = 0;
  game->running = 1;
  game->state = GAME_STATE_PLAYING;
  game->cheat = 0;
  game->true_flagged = 0;
  game->stop = 0;
}

void game_set_input_mode(void) {
  struct termios tattr;
  
  if (!isatty(STDIN_FILENO)) {
    fprintf(stderr, "[ERROR]: This is not a terminal.\n");
    exit(1);
  }

  tcgetattr(STDIN_FILENO, &saved_attr);
  atexit(game_reset_input_mode);

  tcgetattr(STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON | ECHO); /* Clear ICANON and ECHO. */
  tattr.c_cc[VMIN] = 1;
  tattr.c_cc[VTIME] = 0;
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}

void game_reset_input_mode(void) {
  tcsetattr(STDIN_FILENO, TCSANOW, &saved_attr);
}

void game_handle_input(Game *game, char c) {
  switch(c) {
  case 'Q':
    game_handle_yes_no_question(game, "Really quit?", game_quit, game_unpause);
    break;
  case KEYCODE_UP:
  case 'w':
    game_move_cursor_up(game);
    break;
  case KEYCODE_DOWN:
  case 's':
    game_move_cursor_down(game);
    break;
  case KEYCODE_LEFT:
  case 'a':
    game_move_cursor_left(game);
    break;
  case 'd':
  case KEYCODE_RIGHT:
    game_move_cursor_right(game);
    break;
  case 'f':
    game_flag_cell_at(game, game->cursor.row, game->cursor.col);
    game_check_for_win(game);
    break;
  case ' ':
    game_handle_open_cell(game);
    game_check_for_win(game);
    break;
  case 'C':
    game->cheat = !game->cheat;
    if (game->cheat) game_reveal_everything(game);
    else game_unreveal_everything(game);
  default:
    break;
  }
}

void game_start(Game *game) {
  char input;

  game_set_input_mode();
  game_clear_everything();
  while(game->running) {
    if (!game->stop) {
      game_print(game);

      read(STDIN_FILENO, &input, 1);
      game_handle_input(game, input);
      game_clear_screen(game);
    } else {
      game_print(game);
      game_handle_game_state(game);
      game_clear_screen(game);
    }
  }
}

void game_restart(Game *game){ 
  game_clear_everything();
  game_init(game);
}

void game_quit(Game *game){ 
  game_clear_everything();
  game->running = 0;
}

void game_unpause(Game *game) {
  game_clear_everything();
  game_print(game);
  game->stop = 0;
}

void game_move_cursor_up(Game *game) {
  if (game->cursor.row - 1 >= 0) game->cursor.row--;
}

void game_move_cursor_down(Game *game) {
  if (game->cursor.row + 1 < game->field.rows) game->cursor.row++;
}

void game_move_cursor_left(Game *game) {
  if (game->cursor.col - 1 >= 0) game->cursor.col--;
}

void game_move_cursor_right(Game *game) {
  if (game->cursor.col + 1 < game->field.cols) game->cursor.col++;
}

void game_clear_screen(Game *game) {
  /* (void) game; */
  /* printf("\033[H\033[J"); */

  char escape = 27;
  printf("%c[", escape);
  printf("%dA", game->field.rows);
  printf("%c[", escape);
  printf("%dD", game->field.cols * 3);
}

void game_clear_everything() {
  printf("\033[H\033[J");
}

Cell game_open_cell_at(Game *game, int row, int col) {
  Cell cell;

  cell = field_get_cell_at(&game->field, row, col);
  if (cell.state != STATE_FLAGGED) cell.state = STATE_PLAYER_EXPLORED;
  field_set_cell_at(&game->field, row, col, cell);

  return cell;
}

void game_flag_cell_at(Game *game, int row, int col) {
  Cell cell;

  cell = field_get_cell_at(&game->field, row, col);
  if (cell.state == STATE_UNEXPLORE) cell.state = STATE_FLAGGED;
  else if (cell.state == STATE_FLAGGED) cell.state = STATE_UNEXPLORE;
  field_set_cell_at(&game->field, row, col, cell);
}

void game_open_neighbor_cell(Game *game, int row, int col) {
  Cell cell;

  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <=1; ++j) {
      if (row + i >= game->field.rows || col + j >= game->field.cols) break;
      if (row + i < 0 || col + j < 0) continue;
      if (i == 0 && j == 0) continue;

      cell = field_get_cell_at(&game->field, row + i, col + j);
      if(cell.state == STATE_EXPLORED || cell.state == STATE_PLAYER_EXPLORED) continue;

      cell.state = STATE_EXPLORED;
      if (cell.object != OBJ_MINE) {
	if (cell.mines_count > 0) {
	  field_set_cell_at(&game->field, row + i, col + j, cell);
	  continue;
	} else if (cell.mines_count == 0) {
	  field_set_cell_at(&game->field, row + i, col + j, cell);
	  game_open_neighbor_cell(game, row + i, col + j);
	}
      }
    }
  }
}

void game_handle_open_cell(Game *game) {
  Cell cell;
  cell = game_open_cell_at(game, game->cursor.row, game->cursor.col);
  if (cell.object == OBJ_MINE) {
    game_reveal_everything(game);
    game->state = GAME_STATE_LOSE;
    game->stop = 1;
  } else {
    if (cell.mines_count == 0) {
      game_open_neighbor_cell(game, game->cursor.row, game->cursor.col);
    }
  }
}

void game_check_for_win(Game *game) {
  Cell cell;
  int mine_cells = 0;

  for (int i = 0; i < game->field.rows; ++i) {
    for (int j = 0; j < game->field.cols; ++j) {
      cell = field_get_cell_at(&game->field, i, j);
      if (cell.state == STATE_UNEXPLORE) mine_cells++;
      if (cell.state == STATE_FLAGGED && cell.object == OBJ_MINE) mine_cells++;
    }
  }

  if (mine_cells == game->field.total_mines) {
    game->state = GAME_STATE_WON;
    game->stop = 1;
  }
}

void game_handle_game_state(Game *game) {
  if (game->state == GAME_STATE_WON) {
    printf("Congratulation! You won.\n");
    game_handle_yes_no_question(game, "Continue playing?", game_restart, game_quit);
  } else if (game->state == GAME_STATE_LOSE) {
    printf("Oh no! Seems like you hit a mine. That's suck.\n");
    game_handle_yes_no_question(game, "Retry?", game_restart, game_quit);
  }
}

void game_handle_yes_no_question(Game *game, const char *question, void (*yes) (Game *game), void (*no)(Game *game)) {
  char input;

  printf("%s ", question);
  printf("[y/n]\n");

  read(STDIN_FILENO, &input, 1); 
  switch (input) {
  case 'Y': case 'y':
    yes(game);
    break;
  case 'N': case 'n':
    no(game);
    break;
  default:
    printf("Please answer yes or no\n");
    game_handle_yes_no_question(game, question, yes, no);
    break;
  }
  game_clear_screen(game);
}

void game_reveal_everything(Game *game) {
  Cell cell;

  for (int i = 0; i < game->field.rows; ++i) {
    for (int j = 0; j < game->field.cols; ++j) {
      cell = field_get_cell_at(&game->field, i, j);
      if (cell.state == STATE_UNEXPLORE) cell.state = STATE_EXPLORED;
      field_set_cell_at(&game->field, i, j, cell);
    }
  }
}

void game_reveal_only_mines(Game *game) {
  Cell cell;

  for (int i = 0; i < game->field.rows; ++i) {
    for (int j = 0; j < game->field.cols; ++j) {
      cell = field_get_cell_at(&game->field, i, j);
      if (cell.state == STATE_UNEXPLORE && cell.object == OBJ_MINE) cell.state = STATE_EXPLORED;
      field_set_cell_at(&game->field, i, j, cell);
    }
  }
}

void game_unreveal_everything(Game *game) {
  Cell cell;

  for (int i = 0; i < game->field.rows; ++i) {
    for (int j = 0; j < game->field.cols; ++j) {
      cell = field_get_cell_at(&game->field, i, j);
      if (cell.state == STATE_EXPLORED) cell.state = STATE_UNEXPLORE;
      field_set_cell_at(&game->field, i, j, cell);
    }
  }
}

void game_print(Game *game) {
  Cell cell;
  for (int i = 0; i < game->field.rows; ++i) {
    for (int j = 0; j < game->field.cols; ++j) {
      cell = field_get_cell_at(&game->field, i, j);
      if (game->cursor.row == i && game->cursor.col == j) printf("["); else printf(" ");
      if (cell.state == STATE_UNEXPLORE) {
	printf(".");
      } else if (cell.state == STATE_FLAGGED) {
	printf("?");
      } else {
	if (cell.object == OBJ_MINE) {
	  printf("*");
	} else if (cell.object == OBJ_EMPTY) {
	  if (cell.mines_count != 0) printf("%d", cell.mines_count);
	  else printf(" ");
	} else {
	  printf("Unreachable\n");
	  break;
	}
      }
      if (game->cursor.row == i && game->cursor.col == j) printf("]"); else printf(" ");
    }
    printf("\n");
  }
}
