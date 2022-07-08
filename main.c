#include <stdio.h>

#include "./mine_field.h"
#include "./game.h"

int main(void) {
  Game game;

  game_init(&game);
  game_start(&game);

  field_free(&game.field);
  return 0;
}
