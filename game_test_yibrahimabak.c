#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"

/*
bool nouveau(char* nom) {
    fprintf(stderr, "nouveau: %s <testname>\n", nom);
    exit(false);
}

int main(int argc, char* argv[]) {
    if (argc != 2){
        nouveau(argv[0]);
    }
    if (strcmp (argv[1] , "dummy")==0){

        return true;
    }else{
        return false;
    }

} */

bool test_yibrahimabak_game_set_expected_nb_tents_row() {
  game g = game_default_solution();

  game_set_expected_nb_tents_row(g, 0, 1);
  if (game_get_expected_nb_tents_row(g, 0) != 1) {
    return false;
  }
  game_set_expected_nb_tents_row(g, 1, 2);
  if (game_get_expected_nb_tents_row(g, 1) != 2) {
    return false;
  }
  game_set_expected_nb_tents_row(g, 2, 3);
  if (game_get_expected_nb_tents_row(g, 2) != 3) {
    return false;
  }
  game_set_expected_nb_tents_row(g, 3, 4);
  if (game_get_expected_nb_tents_row(g, 3) != 4) {
    return false;
  }
  game_set_expected_nb_tents_row(g, 4, 5);
  if (game_get_expected_nb_tents_row(g, 4) != 5) {
    return false;
  }
  game_set_expected_nb_tents_row(g, 5, 6);
  if (game_get_expected_nb_tents_row(g, 5) != 6) {
    return false;
  }
  game_set_expected_nb_tents_row(g, 6, 7);
  if (game_get_expected_nb_tents_row(g, 6) != 7) {
    return false;
  }
  game_set_expected_nb_tents_row(g, 7, 8);
  if (game_get_expected_nb_tents_row(g, 7) != 8) {
    return false;
  }
  return true;
}
bool test_yibrahimabak_game_get_expected_nb_tents_col(void) {
  game g = game_default();
  uint nb_tents_col[] = {4, 0, 1, 2, 1, 1, 2, 1};
  if (g == NULL) {
    return false;
  }
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    if (game_get_expected_nb_tents_col(g, j) != nb_tents_col[j]) {
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_yibrahimabak_game_get_expected_nb_tents_row(void) {
  game g = game_default_solution();
  uint nb_tents_row[] = {3, 0, 4, 0, 4, 0, 1, 0};
  if (g == NULL) {
    return false;
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row[i]) {
      game_delete(g);
      return false;
    }
  }
  game_delete(g);

  return true;
}

bool test_yibrahimabak_game_get_expected_nb_tents_all(void) {
  game g = game_default_solution();
  game g1 = game_default();
  if (g == NULL || g1 == NULL) {
    game_delete(g);
    game_delete(g1);
    return false;
  }
  if (game_get_expected_nb_tents_all(g) != game_get_expected_nb_tents_all(g1)) {
    game_delete(g);
    game_delete(g1);
    return false;
  }
  game_delete(g);
  game_delete(g1);
  return true;
}
bool test_yibrahimabak_game_get_current_nb_tents_col(void) {
  game g = game_default_solution();
  uint nb_tents_col[] = {4, 0, 1, 2, 1, 1, 2, 1};
  if (g == NULL) {
    return false;
  }
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    if (game_get_current_nb_tents_col(g, j) != nb_tents_col[j]) {
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_yibrahimabak_game_get_current_nb_tents_row(void) {
  game g = game_default_solution();
  uint nb_tents_row[] = {3, 0, 4, 0, 4, 0, 1, 0};
  if (g == NULL) {
    return false;
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_current_nb_tents_row(g, i) != nb_tents_row[i]) {
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_yibrahimabak_game_get_current_nb_tents_all(void) {
  game g = game_default_solution();
  if (game_get_current_nb_tents_all(g) != 12) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

void usage(char* cmd) {
  fprintf(stderr, "Usage: %s <test_yibrahimabakname>\n", cmd);
  exit(false);
}

int main(int argc, char* argv[]) {
  bool ok = false;
  if (argc != 2) {
    usage(argv[0]);
  }

  if (strcmp(argv[1], "dummy") == 0) {
    ok = true;
  } else {
    ok = false;
  }
  if (strcmp(argv[1], "game_set_expected_nb_tents_row") == 0) {
    ok = test_yibrahimabak_game_set_expected_nb_tents_row();
  } else if (strcmp(argv[1], "game_get_expected_nb_tents_col") == 0) {
    ok = test_yibrahimabak_game_get_expected_nb_tents_col();
  } else if (strcmp(argv[1], "game_get_expected_nb_tents_row") == 0) {
    ok = test_yibrahimabak_game_get_expected_nb_tents_row();
  } else if (strcmp(argv[1], "game_get_expected_nb_tents_all") == 0) {
    ok = test_yibrahimabak_game_get_expected_nb_tents_all();
  } else if (strcmp(argv[1], "game_get_current_nb_tents_col") == 0) {
    ok = test_yibrahimabak_game_get_current_nb_tents_col();
  } else if (strcmp(argv[1], "game_get_current_nb_tents_row") == 0) {
    ok = test_yibrahimabak_game_get_current_nb_tents_row();
  } else if (strcmp(argv[1], "game_get_current_nb_tents_all") == 0) {
    ok = test_yibrahimabak_game_get_current_nb_tents_all();
  }
  if (ok) {
    return EXIT_SUCCESS;
  } else {
    return EXIT_FAILURE;
  }
}