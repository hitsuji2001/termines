#include <stdio.h>

#include "../header/mine_field.h"
#include "../header/game.h"

int main(void) {
  Game game;

  game_run(&game);

  field_free(&game.field);
  return 0;
}
