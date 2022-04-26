#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_tools.h"

int test_dummy(char *c) {
  if (strcmp("dummy", c) == 0) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", c);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", c);
    return EXIT_FAILURE;
  }
}

int test_game_play_move(game g, game win) {
  printf("dans play move\n");

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(win, i, j) == TENT) {
        game_play_move(g, i, j, TENT);
        if (game_get_square(g, i, j) != TENT) {
          printf("coup : i %i , j %i\n", i, j);
          fprintf(stderr,
                  "Test game_play_move "
                  "finished: FAILURE\n");
          return EXIT_FAILURE;
        }
      }
    }
  }

  game_play_move(g, 0, 6, TENT);
  if (game_check_move(g, 0, 7, TENT) != 2) {
    fprintf(stderr, "Test game_play_move finished: FAILURE\n");
    return EXIT_FAILURE;
  }

  fprintf(stderr, "Test game_play_move finished: SUCCESS\n");
  return EXIT_SUCCESS;
}

int test_game_check_move(cgame g, cgame g2) {
  printf("dans check_move\n");
  int count1 = 0;
  int count2 = 0;
  int tree = 0;
  int tent = 0;
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    printf("premier for\n");
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      printf("deuxième for\n");

      if (game_check_move(g, i, j, TREE) != ILLEGAL) {
        printf("tree != ILLEGAL \n");
        fprintf(stderr,
                "Test game_check_move "
                "finished: FAILURE\n");
        return EXIT_FAILURE;
      }

      if (game_get_square(g, i, j) == TENT) {
        tent++;
        printf("get_square i j == TENT\n");

        if ((game_check_move(g, i, j, TENT) != REGULAR) ||
            ((game_check_move(g, i, j, GRASS) != LOSING) || (game_check_move(g, i, j, EMPTY) != REGULAR) || (game_check_move(g2, i, j, TENT) != REGULAR))) {
          printf(
              "check_move != de "
              "get_square de TENT\n");
          fprintf(stderr,
                  "Test game_check_move "
                  "finished: FAILURE\n");
          return EXIT_FAILURE;
        }
      } else if (game_get_square(g, i, j) != TENT) {
        printf("get_square i j != TENT\n");

        if (game_get_square(g2, i, j) != TREE) {
          if (game_check_move(g2, i, j, TENT) != LOSING) {
            if (!((i == 0 && j == 7) || (i == 4 && j == 5) || (i == 6 && j == 4) || (i == 6 && j == 6))) {
              fprintf(stderr,
                      "Test"
                      " gam"
                      "e_"
                      "chec"
                      "k_"
                      "move"
                      " fin"
                      "ishe"
                      "d: "
                      "FAIL"
                      "URE"
                      "\n");
              return EXIT_FAILURE;
            }
          }
        }

        if (game_get_square(g, i, j) == GRASS) {
          printf(
              "get_square i j == "
              "grass\n");
          if (game_check_move(g, i, j, TENT) != LOSING || game_check_move(g, i, j, EMPTY) != REGULAR) {
            printf(
                "TENT  sur "
                "GRASS!= "
                "LOSING\n");
            fprintf(stderr,
                    "Test "
                    "game_check_"
                    "move "
                    "finished: "
                    "FAILURE\n");
            return EXIT_FAILURE;
          }
        }

        if (game_get_square(g, i, j) == TREE) {
          printf(
              "game_get_square == "
              "TREE\n");
          tree++;
          if (game_check_move(g, i, j, TENT) != ILLEGAL || game_check_move(g, i, j, GRASS) != ILLEGAL || game_check_move(g, i, j, EMPTY) != ILLEGAL) {
            printf(
                "check_move "
                "TENT ou GRASS "
                "ou EMPTY sur "
                "TREE != "
                "ILLEGAL\n");
            fprintf(stderr,
                    "Test "
                    "game_check_"
                    "move "
                    "finished: "
                    "FAILURE\n");
            return EXIT_FAILURE;
          }
        }
      }
      count2++;
    }
    count1++;
  }
  if (tree != tent) {
    fprintf(stderr, "Test game_is_over finished: FAILURE\n");
    return EXIT_FAILURE;
  }
  printf(
      "premier a parcouru %i et le deuxième for à parcouru %i "
      "elements\n",
      count1, count2);
  fprintf(stderr, "Test game_check_move finished: SUCCESS\n");
  return EXIT_SUCCESS;
}

int test_game_is_over(cgame gwin, cgame gnew) {
  if (game_is_over(gwin) || !game_is_over(gnew)) {
    fprintf(stderr, "Test game_is_over finished: SUCCESS\n");
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test game_is_over finished: FAILURE\n");
    return EXIT_FAILURE;
  }
}

int test_game_fill_grass_row(game g) {
  game_fill_grass_row(g, 6);
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_square(g, 6, i) != GRASS) {
      fprintf(stderr,
              "Test game_fill_grass_row finished: "
              "FAILURE\n");
      return EXIT_FAILURE;
    }
  }
  fprintf(stderr, "Test game_fill_grass_row finished: SUCCESS\n");
  return EXIT_SUCCESS;
}

int test_game_fill_grass_col(game g) {
  game_fill_grass_col(g, 2);
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_square(g, i, 2) != GRASS) {
      fprintf(stderr,
              "Test game_fill_grass_row finished: "
              "FAILURE\n");
      return EXIT_FAILURE;
    }
  }

  fprintf(stderr, "Test game_fill_grass_col finished: SUCCESS\n");
  return EXIT_SUCCESS;
}

int test_game_restart(game g) {
  printf("dans restart\n");
  game_restart(g);
  game new = game_default();
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != game_get_square(new, i, j)) {
        fprintf(stderr, "Test game_is_over finished: FAILURE\n");
        game_delete(new);
        return EXIT_FAILURE;
      }
    }
  }
  game_delete(new);
  fprintf(stderr, "Test game_is_over finished: SUCCESS\n");
  return EXIT_SUCCESS;
}

int test_game_save() {
  game g = game_default();
  game_set_square(g, 0, 0, TENT);
  game_set_square(g, 2, 0, GRASS);
  game_save(g, "test_game.txt");
  game g1 = game_load("test_game.txt");
  bool ok = true;
  if (!game_equal(g, g1)) {
    ok = false;
  }
  game_delete(g);
  game_delete(g1);
  return ok;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "entre un argument\n");
    exit(EXIT_FAILURE);
  }
  if (strcmp("dummy", argv[1]) == 0) {
    return test_dummy(argv[1]);
  }
  game new = game_default();
  game new2 = game_default();
  game win = game_default_solution();
  game win2 = game_default_solution();
  if (strcmp("check_move", argv[1]) == 0) {
    test_game_check_move(win2, new2);
  } else if (strcmp("game_is_over", argv[1]) == 0) {
    test_game_is_over(win, new);
  } else if (strcmp("game_restart", argv[1]) == 0) {
    test_game_restart(win);
  } else if (strcmp("game_fill_grass_row", argv[1]) == 0) {
    test_game_fill_grass_row(new);
  } else if (strcmp("game_fill_grass_col", argv[1]) == 0) {
    test_game_fill_grass_col(new);
  } else if (strcmp("game_play_move", argv[1]) == 0) {
    test_game_play_move(new, win);
  } else if (strcmp("game_save", argv[1]) == 0) {
    test_game_save();
  }
  game_delete(new);
  game_delete(new2);
  game_delete(win);
  game_delete(win2);
}
