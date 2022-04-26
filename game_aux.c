#include <stdio.h>

#include "game.h"
#include "game_ext.h"

uint tab_row[] = {3, 0, 4, 0, 4, 0, 1, 0};
uint tab_col[] = {4, 0, 1, 2, 1, 1, 2, 1};

void game_print(cgame g) {
  printf("   ");
  for (uint i = 0; i < game_nb_cols(g); i++) {
    printf("%d", i);
  }
  printf("\n   ");
  for (uint i = 0; i < game_nb_cols(g); i++) {
    printf("-");
  }
  printf(" \n");

  for (int i = 0; i < game_nb_rows(g); i++) {
    printf("%i |", i);
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        printf(" ");
      } else if (game_get_square(g, i, j) == TREE) {
        printf("x");
      } else if (game_get_square(g, i, j) == GRASS) {
        printf("-");
      } else if (game_get_square(g, i, j) == TENT) {
        printf("*");
      }
    }
    printf("| %i\n", game_get_expected_nb_tents_row(g, i));
  }

  printf("   ");
  for (uint i = 0; i < game_nb_cols(g); i++) {
    printf("-");
  }
  printf("\n   ");
  for (uint i = 0; i < game_nb_cols(g); i++) {
    printf("%d", game_get_expected_nb_tents_col(g, i));
  }
  printf("\n");
}

game game_default(void) {
  square tab[] = {EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
                  EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
                  TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};

  game new = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, tab, tab_row, tab_col, false, false);
  // game new = game_new(tab, tab_row, tab_col);
  return new;
}

game game_default_solution(void) {
  square tab[] = {TENT,  GRASS, GRASS, TENT,  TREE,  TREE,  TENT,  GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  TENT,  GRASS, GRASS, TENT,  TREE,  TENT,
                  GRASS, TENT,  TREE,  GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, TENT,  TREE,  TENT,  GRASS, TENT,  GRASS, TENT,  GRASS, TREE,  GRASS, GRASS, GRASS,
                  TREE,  GRASS, TREE,  GRASS, TENT,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};

  game new = game_new(tab, tab_row, tab_col);
  return new;
}