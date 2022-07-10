#ifndef __MENU_H__
#define __MENU_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>

#include "./cursor.h"

#define COLOR_BLACK	"\033[30m"
#define COLOR_RED	"\033[31m"
#define COLOR_GREEN	"\033[32m"
#define COLOR_YELLOW	"\033[33m"
#define COLOR_BLUE	"\033[34m"
#define COLOR_MAGENTA	"\033[35m"
#define COLOR_CYAN	"\033[36m"
#define COLOR_WHITE	"\033[37m"
#define COLOR_RESET	"\033[0m"

#define MENU_NUMBER_OF_MENU_CHOICES 3
#define MENU_NUMBER_OF_OPTION_CHOICES 1

#define ARRAY_LEN(x) sizeof((x)) / sizeof((x)[0])

typedef struct {
  char *menu_choices[MENU_NUMBER_OF_MENU_CHOICES];
  char *option_choices[MENU_NUMBER_OF_OPTION_CHOICES];
  int is_in_menu;
  Cursor cursor;
} Menu;

void menu_init_menu(Menu *menu);
void menu_init_option(Menu *menu);
void menu_handle(Menu *menu);
void menu_print_menu(Menu *menu);
void menu_print_option_menu(Menu *menu);
void menu_handle_input(Menu *menu, char input);
void menu_free(Menu *menu);
void menu_clear_everthing();

#endif // __MENU_H__
