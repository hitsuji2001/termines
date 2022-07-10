#include "../header/menu.h"

void menu_init_menu(Menu *menu) {
  static const char *MENU_CHOICES[] = {
    "Play game",
    "Options",
    "Quit",
  };

  static_assert(MENU_NUMBER_OF_MENU_CHOICES == ARRAY_LEN(MENU_CHOICES));
  
  menu->is_in_menu = 1;
  menu->cursor.row = 0;
  menu->cursor.col = 0;

  for (size_t i = 0; i < ARRAY_LEN(MENU_CHOICES); ++i) {
    menu->menu_choices[i] = malloc((strlen(MENU_CHOICES[i]) + 1) * sizeof(char));
    memcpy(menu->menu_choices[i], MENU_CHOICES[i], strlen(MENU_CHOICES[i]) + 1);
  }
}

void menu_init_option(Menu *menu) {
  static const char *OPTION_CHOICES[] = {
    "a",
  };
  static_assert(MENU_NUMBER_OF_OPTION_CHOICES == ARRAY_LEN(OPTION_CHOICES));

  for (size_t i = 0; i < ARRAY_LEN(menu->option_choices); ++i) {
    menu->option_choices[i] = malloc((strlen(OPTION_CHOICES[i]) + 1) * sizeof(char));
    memcpy(menu->option_choices[i], OPTION_CHOICES[i], strlen(OPTION_CHOICES[i]) + 1);
  }
}

void menu_handle(Menu *menu) {
  char input;

  menu_init_menu(menu);
  while (menu->is_in_menu) {
    menu_print_menu(menu);

    read(STDIN_FILENO, &input, 1);
    menu_handle_input(menu, input);
    menu_clear_everthing();
  }
}

void menu_print_menu(Menu *menu) {
  for (size_t i = 0; i < ARRAY_LEN(menu->menu_choices); ++i) {
    if ((size_t)menu->cursor.row == i) printf("["); else printf(" ");
    printf("%zu", i + 1);
    if ((size_t)menu->cursor.row == i) printf("]"); else printf(" ");
    
    printf(" %s\n", menu->menu_choices[i]);
  }
}

void menu_print_option_menu(Menu *menu) {
  (void) menu;
}

void menu_handle_input(Menu *menu, char input) {
  switch(input) {
  case 'w':
    if (menu->cursor.row - 1 >= 0) cursor_move_up(&menu->cursor);
    break;
  case 's':
    if (menu->cursor.row + 1 < MENU_NUMBER_OF_OPTION_CHOICES) cursor_move_up(&menu->cursor);
    break;
  default:
    break;
  }
}

void menu_free(Menu *menu) {
  for (size_t i = 0; i < ARRAY_LEN(menu->menu_choices); ++i) {
    free(menu->menu_choices[i]);
  }

  if (menu->option_choices != NULL) {
    for (size_t i = 0; i < ARRAY_LEN(menu->option_choices); ++i) {
      free(menu->option_choices[i]);
    }
  }
}

void menu_clear_everthing() {
  printf("\033[H\033[J");
}
