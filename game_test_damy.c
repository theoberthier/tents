#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

//  Fonction d'usage

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <login>\n", argv[0]);
  exit(EXIT_FAILURE);
}

square s[] = {EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
              EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
              TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
uint row[] = {3, 0, 4, 0, 4, 0, 1, 0};
uint col[] = {4, 0, 1, 2, 1, 1, 2, 1};

bool test_game_set_expected_nb_tents_row(void) {
  bool test = true;
  game g = game_default();
  for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
    for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
      game_set_expected_nb_tents_row(g, i, j);
      if (game_get_expected_nb_tents_row(g, i) != j) {
        test = false;
      }
    }
  }
  game_delete(g);
  return test;
}

bool test_game_set_expected_nb_tents_col(void) {
  bool test = true;
  game g = game_default();
  for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
    for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
      game_set_expected_nb_tents_col(g, i, j);
      if (game_get_expected_nb_tents_col(g, i) != j) {
        test = false;
      }
    }
  }
  game_delete(g);
  return test;
}

bool test_game_get_current_nb_tents_all(void) {
  bool test = true;
  game g = game_default_solution();
  // Nous allons calculer le nombre de tentes et lignes et en colonnes
  unsigned int row = 0;
  unsigned int col = 0;
  unsigned int tents = game_get_current_nb_tents_all(g);
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    row = row + game_get_current_nb_tents_row(g, i);
    col = col + game_get_current_nb_tents_col(g, i);
  }
  if (row != col) {
    test = false;
  }
  if (row != tents) {
    test = false;
  }
  game_delete(g);
  return test;
}

bool test_game_get_expected_nb_tents_all(void) {
  bool test = true;
  game g = game_default();
  // Nous allons calculer le nombre de tentes et lignes et en colonnes
  unsigned int row = 0;
  unsigned int col = 0;
  unsigned int tents = game_get_expected_nb_tents_all(g);
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    row = row + game_get_expected_nb_tents_row(g, i);
    col = col + game_get_expected_nb_tents_row(g, i);
  }
  if (row != col) {
    test = false;
  }
  if (row != tents) {
    test = false;
  }
  game_delete(g);
  return test;
}

bool test_game_get_current_col(void) {
  bool test = true;
  game g = game_default_solution();
  unsigned int row = 0;
  unsigned int col = 0;
  // unsigned int tents = game_get_current_nb_tents_all(g);
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    row = row + game_get_current_nb_tents_row(g, i);
    col = col + game_get_current_nb_tents_col(g, i);
  }
  if (row != col) {
    test = false;
  }
  if (game_get_current_nb_tents_row(g, 0) != 3) {
    test = false;
  }
  if (game_get_current_nb_tents_row(g, 1) != 0) {
    test = false;
  }
  if (game_get_current_nb_tents_row(g, 2) != 4) {
    test = false;
  }
  if (game_get_current_nb_tents_row(g, 3) != 0) {
    test = false;
  }
  if (game_get_current_nb_tents_row(g, 4) != 4) {
    test = false;
  }
  if (game_get_current_nb_tents_row(g, 5) != 0) {
    test = false;
  }
  if (game_get_current_nb_tents_row(g, 6) != 1) {
    test = false;
  }
  if (game_get_current_nb_tents_row(g, 7) != 0) {
    test = false;
  }
  game_delete(g);
  return test;
}

bool test_game_equal(void) {
  printf("test");
  bool test = true;
  game g = game_new_empty();
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      game_set_square(g, i, j, 3);
    }
  }
  game h = game_copy(g);
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      game_set_square(h, i, j, 2);
      if (game_equal(g, h)) {
        printf("ERREUR ! \n");
        test = false;
      }
      game_set_square(h, i, j, 3);
    }
  }
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    game_set_expected_nb_tents_col(h, i, 2);
    if (game_equal(g, h)) {
      printf("ERREUR ! \n");
      test = false;
    }
    game_set_expected_nb_tents_col(h, i, 0);
  }
  game_delete(g);
  game_delete(h);
  return test;
}

bool test_game_copy(void) {
  bool test = true;
  game g = game_default();
  for (unsigned int i = 0; i < 3; i++) {
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      game_set_square(g, i, j, 3);
    }
  }
  game h = game_copy(g);
  for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
    for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
      if (game_get_square(h, i, j) != game_get_square(g, i, j)) {
        printf("ERREUR ! \n");
        test = false;
      }
    }
  }
  // On test de modifier g pour voir si h est également modifié, ce qui
  // ne doit pas être le cas
  game_set_square(g, 0, 0, 0);
  if (game_get_square(h, 0, 0) != 3) {
    test = false;
  }
  game_delete(g);
  game_delete(h);
  return test;
}

bool test_set_square(void) {
  bool test = true;
  game g = game_default();
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      game_set_square(g, i, j, EMPTY);
      if (game_get_square(g, i, j) != EMPTY) {
        test = false;
      }
      game_set_square(g, i, j, TREE);
      if (game_get_square(g, i, j) != TREE) {
        test = false;
      }
      game_set_square(g, i, j, TENT);
      if (game_get_square(g, i, j) != TENT) {
        test = false;
      }
      game_set_square(g, i, j, GRASS);
      if (game_get_square(g, i, j) != GRASS) {
        test = false;
      }
    }
  }
  game_delete(g);
  return test;
}

bool test_game_new_empty(void) {
  game g = game_new_empty();
  square tab[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint row_col[] = {0, 0, 0, 0, 0, 0, 0, 0};
  game h = game_new(tab, row_col, row_col);
  bool test = game_equal(g, h);
  game_delete(g);
  game_delete(h);
  return test;
}
bool test_game_new(void) {
  // game_default fait appel à game_new
  bool test = true;
  game g = game_default();
  uint tab_row[] = {3, 0, 4, 0, 4, 0, 1, 0};
  uint tab_col[] = {4, 0, 1, 2, 1, 1, 2, 1};
  // Les valeurs sont déjà tester dans get_square, reste à tester les
  // lignes et colonnes
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    uint tents_row = game_get_expected_nb_tents_row(g, i);
    uint tents_col = game_get_expected_nb_tents_col(g, i);
    if ((tab_row[i] != tents_row) || (tab_col[i] != tents_col)) {
      test = false;
    }
  }
  game_delete(g);
  return test;
}

bool test_get_square(void) {
  // tab permettra de tester un à un
  square tab[] = {EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
                  EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
                  TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  // on va comparer game_get_square(g,i,j) sur tous les i et j dans une
  // boucle while avec les valeurs de g (dans game_default)
  game g = game_default();
  bool test = true;  // true = pas d'erreur
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != tab[j + DEFAULT_SIZE * i]) {
        test = false;
      }
    }
  }
  game_delete(g);
  return test;
}

bool test_game_check_move(void) {
  bool test = true;
  game g = game_default();
  game gs = game_default_solution();
  // game gc = game_copy(g);
  // printf("check_move = %d\n", game_check_move(g, 0, 3, GRASS));
  // Placer des tentes
  if (game_check_move(g, 0, 3, TENT) != 1) {
    test = false;
    printf("Err1");
  }
  if (game_check_move(g, 0, 4, TENT) != 0) {
    test = false;
    printf("Err1");
  }
  if (game_check_move(g, 0, 1, TENT) != 2) {
    test = false;
    printf("Err1");
  }
  // Placer toutes les tentes
  square tab[] = {TENT,  GRASS, GRASS, TENT,  TREE,  TREE,  TENT,  GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  TENT,  GRASS, GRASS, TENT,  TREE,  TENT,
                  GRASS, TENT,  TREE,  GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, TENT,  TREE,  TENT,  GRASS, TENT,  GRASS, TENT,  GRASS, TREE,  GRASS, GRASS, GRASS,
                  TREE,  GRASS, TREE,  GRASS, TENT,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      if (tab[j + i * DEFAULT_SIZE] == TENT) {
        if (game_check_move(g, i, j,
                            TENT) != 1) {  // Mouvements REGULAR
          printf(
              "ERRb1, i = %d, j = %d, "
              "check_move = %d\n",
              i, j, game_check_move(g, i, j, TENT));
          test = false;
        }
        if (game_check_move(gs, i, j, GRASS) != 2) {
          printf("Errb1.5");
          test = false;
        }
      } else if (tab[j + i * DEFAULT_SIZE] == TREE) {
        if (game_check_move(g, i, j,
                            TENT) != 0) {  // Mouvements ILLEGAL
          printf(
              "ERRb2, i = %d, j = %d, "
              "check_move = %d\n",
              i, j, game_check_move(g, i, j, TENT));
          test = false;
        }
      } else {
        if (game_check_move(g, i, j,
                            TENT) != 2) {  // Mouvements Losing
          if (!((i == 0 && j == 7) || (i == 4 && j == 5) || (i == 6 && j == 4) || (i == 6 && j == 6))) {
            // On enlève les
            // seuls valeurs
            // pour lesquels le
            // check_move ne
            // renvoie pas
            // losing
            printf(
                "ERRb3, i = "
                "%d, j = %d, "
                "check_move = "
                "%d\n",
                i, j, game_check_move(g, i, j, TENT));
            test = false;
          }
        }
      }
    }
  }
  if (game_check_move(g, 0, 0, GRASS) != 2) {
    test = false;
    printf("Err2\n");
  }
  if (game_check_move(g, 2, 0, GRASS) != 2) {
    test = false;
    printf("Err2\n");
  }
  if (game_check_move(g, 4, 0, GRASS) != 2) {
    test = false;
    printf("Err2\n");
  }
  if (game_check_move(g, 6, 0, GRASS) != 2) {
    test = false;
    printf("Err2\n");
  }
  // Placer un arbre = ILLEGAL
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_check_move(g, i, j, TREE) != 0) {
        test = false;
      }
    }
  }
  // Placer de l'herbe
  if (game_check_move(g, 0, 3, GRASS) != 1) {
    test = false;
    printf("ERR1\n");
  }
  if (game_check_move(g, 0, 4, GRASS) != 0) {
    test = false;
    printf("ERR2\n");
  }
  if (game_check_move(g, 0, 1, GRASS) != 1) {
    test = false;
    printf("ERR3\n");
  }
  // Plaçons quelques tentes
  game_play_move(g, 0, 6, TENT);
  if (game_check_move(g, 0, 7, TENT) != 2) {
    test = false;
    printf("ERR4, check_move = %d\n", game_check_move(g, 0, 7, TENT));
    game_print(g);
  }
  game_delete(g);
  game_delete(gs);
  return test;
}

bool test_game_is_over(void) {
  bool test = true;
  game g = game_default();
  game gs = game_default_solution();
  if (game_is_over(g) == 1) {  // Test sur game_default
    test = false;
  }
  if (game_is_over(gs) == 0) {  // Test sur game_default_solution
    test = false;
  }
  game gc2 = game_copy(g);
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {  // Tests de placer des tentes partout pour voir
                                                     // si le jeu se termine au bout d'un moment
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_check_move(g, i, j,
                          TENT) == 1) {  // Si le mouvement est validé
        game gc1 = game_copy(g);
        game_play_move(gc1, i, j, TENT);
        if (game_is_over(gc1)) {
          test = false;
        }
        game_delete(gc1);
        // Plaçons des tentes partout
        game_play_move(gc2, i, j, TENT);
      }
    }
  }
  game_delete(gc2);
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {  // Tests complets en changeant uniquement les
                                                     // lignes ou les colonnes
    game gc4 = game_copy(gs);
    game gc5 = game_copy(gs);
    // printf("test = %d\n",test);
    if (game_get_expected_nb_tents_row(gc4, i) != 0) {
      game_set_expected_nb_tents_row(gc4, i, 0);
      if (game_is_over(gc4) == 1) {
        printf("testb = %d\n", test);
        test = false;
        printf("testa = %d\n", test);
      }
    }
    if (game_get_expected_nb_tents_col(gc5, i) != 0) {
      game_set_expected_nb_tents_col(gc5, i, 0);
      if (game_is_over(gc5) == 1) {
        printf("testb = %d\n", test);
        test = false;
        printf("testa = %d\n", test);
      }
    }
    game_delete(gc4);
    game_delete(gc5);
  }
  game gc6 = game_copy(gs);
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {  // Tests de remplacer des grass par empty un à
                                                     // un et complétement
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      game gc3 = game_copy(gs);
      if (game_get_square(gc3, i, j) == GRASS) {
        game_set_square(gc3, i, j, EMPTY);
        game_set_square(gc6, i, j, EMPTY);
      }
      if (game_is_over(gc3) != 1) {
        test = false;
      }
      game_delete(gc3);
    }
  }
  game_delete(gc6);
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {  // Remplacer l'herbe par un arbre
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      game gc7 = game_copy(gs);
      if (game_get_square(gc7, i, j) == GRASS) {
        game_set_square(gc7, i, j, TREE);
        if (game_is_over(gc7) != 0) {
          test = false;
        }
      }
      game_delete(gc7);
    }
  }
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {  // Remplacer un arbre par de l'herbe
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      game gc8 = game_copy(gs);
      if (game_get_square(gc8, i, j) == TREE) {
        game_set_square(gc8, i, j, GRASS);
        if (game_is_over(gc8) != 0) {
          test = false;
        }
      }
      game_delete(gc8);
    }
  }
  game gnew = game_new_empty();
  if (game_is_over(gnew) != 1) {  // un jeu vide doit être gagnant
    test = false;
  }
  game_delete(gnew);
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {  // Enlever une tente
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      game gc9 = game_copy(gs);
      if (game_get_square(gc9, i, j) == TENT) {
        game_set_square(gc9, i, j, GRASS);
        if (game_is_over(gc9) != 0) {
          test = false;
        }
      }
      game_delete(gc9);
    }
  }
  game gc10 = game_copy(gs);  // Plaçons une tente en diagonale en
                              // respectant les autres regles
  game_set_square(gc10, 7, 1, TENT);
  game_set_square(gc10, 7, 2, TREE);
  game_set_expected_nb_tents_row(g, 7, 1);
  game_set_expected_nb_tents_col(g, 1, 1);
  if (game_is_over(gc10) != 0) {
    test = false;
  }
  game_delete(gc10);
  // game_print(gs);

  game gc11 = game_copy(gs);  // Test regle 4 en bougeant un arbre
  game_set_square(gc11, 7, 0, GRASS);
  game_set_square(gc11, 7, 1, TREE);
  if (game_is_over(gc11) != 0) {
    test = false;
  }
  game_delete(gc11);
  int compt = 0;
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {  // On test d'enlever une tente, puis d'en
                                                     // replacer une à tous les endroits possibles
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(gs, i, j) == TENT) {
        for (unsigned int k = 0; k < DEFAULT_SIZE; k++) {
          for (unsigned int l = 0; l < DEFAULT_SIZE; l++) {
            if ((i != k) && (j != l)) {  // Pas
                                         // besoin
                                         // de
                                         // replacer
                                         // une
                                         // tente
                                         // au
                                         // même
                                         // endroit
              game gc12 = game_copy(gs);
              int rowb = game_get_expected_nb_tents_row(gc12, i);
              int colb = game_get_expected_nb_tents_col(gc12, j);
              game_set_square(gc12, i, j,
                              GRASS);  // On enlève la tente
              game_set_expected_nb_tents_row(gc12, i, rowb - 1);
              game_set_expected_nb_tents_col(gc12, j,
                                             colb - 1);  // On fait en sorte que les indices
                                                         // de lignes et colonnes restent bons
              int rowa = game_get_expected_nb_tents_row(gc12, k);
              int cola = game_get_expected_nb_tents_col(gc12, l);
              game_set_square(gc12, k, l, TENT);
              game_set_expected_nb_tents_row(gc12, k, rowa + 1);
              game_set_expected_nb_tents_col(gc12, l, cola + 1);
              if (game_is_over(gc12) != 0) {
                compt = compt + 1;  // On compte le nombre de fois où le jeu est
                                    // terminé (12 dans l'original sans bug)
              }
              game_delete(gc12);
            }
          }
        }
      }
    }
  }
  if (compt != 12 && compt != 0) {  // Cas différent du game.o sans bug
    test = false;
    printf("compt = %i", compt);
  }
  game_delete(g);
  game_delete(gs);
  // is_over diagadj
  square s[4] = {TENT, TREE, TREE, TENT};
  uint row_col[2] = {1, 1};
  game gsdiag = game_new_ext(2, 2, s, row_col, row_col, false, true);
  if (!game_is_over(gsdiag)) {
    test = false;
  }
  game_delete(gsdiag);
  return test;
}

bool test_game_undo(square *s, uint *row, uint *col) {
  game g = game_new_ext(8, 8, s, row, col, false, false);
  game gc = game_copy(g);
  game_undo(g);
  if (!game_equal(g, gc)) {
    game_delete(g);
    game_delete(gc);
    return false;
  }
  game_play_move(g, 0, 0, TENT);
  game gc2 = game_copy(g);
  game_undo(g);
  if (!game_equal(g, gc)) {
    game_delete(g);
    game_delete(gc);
    game_delete(gc2);
    return false;
  }
  game_redo(g);
  if (!game_equal(g, gc2)) {
    game_delete(g);
    game_delete(gc);
    game_delete(gc2);
    return false;
  }
  game_delete(g);
  game_delete(gc);
  game_delete(gc2);
  return true;
}

bool test_game_checkv2(square *s, uint *row, uint *col) {
  square tab[] = {TENT,  GRASS, GRASS, TENT,  TREE,  TREE,  TENT,  GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  TENT,  GRASS, GRASS, TENT,  TREE,  TENT,
                  GRASS, TENT,  TREE,  GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, TENT,  TREE,  TENT,  GRASS, TENT,  GRASS, TENT,  GRASS, TREE,  GRASS, GRASS, GRASS,
                  TREE,  GRASS, TREE,  GRASS, TENT,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};
  // wrapping
  game g = game_new_ext(8, 8, s, row, col, true, false);
  game gs = game_new_ext(8, 8, tab, row, col, true, false);
  printf("%d\n", game_check_move(g, 0, 0, TENT));
  for (unsigned int i = 0; i < 8; i++) {
    for (unsigned int j = 0; j < 8; j++) {
      if (tab[j + i * DEFAULT_SIZE] == TENT) {
        if (game_check_move(g, i, j,
                            TENT) != 1) {  // Voir check_move v1

          printf(
              "wrapping i =  %d, j = "
              "%d\n",
              i, j);
          game_delete(g);
          game_delete(gs);
          return false;
        }
        if (game_check_move(gs, i, j, GRASS) != 2) {
          game_delete(g);
          game_delete(gs);
          printf("Errb1.5");
          return false;
        }
      } else if (tab[j + i * DEFAULT_SIZE] == TREE) {
        if (game_check_move(g, i, j,
                            TENT) != 0) {  // Mouvements ILLEGAL
          printf(
              "ERRb2, i = %d, j = %d, "
              "check_move = %d\n",
              i, j, game_check_move(g, i, j, TENT));
          game_delete(g);
          game_delete(gs);
          return false;
        }
      } else {
        if (game_check_move(g, i, j,
                            TENT) != 2) {  // Mouvements Losing
          if (!((i == 0 && j == 7) || (i == 4 && j == 5) || (i == 6 && j == 4) || (i == 6 && j == 6))) {
            // On enlève les seuls valeurs pour lesquels le check_move ne renvoie pas losing
            game_delete(g);
            game_delete(gs);
            return false;
          }
        }
      }
    }
  }
  game_delete(g);
  game_delete(gs);
  square s_w[] = {TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY};
  uint row_w[4] = {1, 1, 1, 1};
  uint col_w[2] = {1, 1};
  game gw = game_new_ext(4, 2, s_w, row_w, col_w, true, false);
  if (game_check_move(gw, 3, 0, TENT) != 1) {
    printf("check_move gw = %d", game_check_move(gw, 3, 0, TENT));
    game_delete(gw);
    return false;
  }
  game_delete(gw);
  // diagadj
  game g2 = game_new_ext(8, 8, s, row, col, false, true);
  game_set_expected_nb_tents_row(g2, 1, 1);
  game_set_square(g2, 0, 7, TENT);
  printf("%d\n", game_check_move(g2, 1, 6, TENT));
  if (game_check_move(g2, 1, 6, TENT) != 1) {
    printf("diagadj ! \n");
    game_delete(g2);
    return false;
  }
  game_delete(g2);
  return true;
}

bool test_restart_v2(void) {
  square s_r[] = {TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY};
  uint row_r[4] = {1, 1, 1, 1};
  uint col_r[2] = {1, 1};
  game gr = game_new_ext(4, 2, s_r, row_r, col_r, true, false);
  game gr1 = game_copy(gr);
  game_play_move(gr1, 3, 0, TENT);
  game gr2 = game_copy(gr1);
  game_restart(gr2);  // On restart gr2 pour garder l'historique sur gr1 car il
                      // n'est pas dit que game_copy garde l'historique
  if (game_get_square(gr2, 3, 0) != EMPTY) {
    game_delete(gr);
    game_delete(gr1);
    game_delete(gr2);
    return false;
  }
  game_restart(gr1);
  game_set_square(gr1, 3, 0, TENT);
  game gru1 = game_copy(gr1);
  game_undo(gr1);
  if (!game_equal(gr1, gru1)) {
    game_delete(gr);
    game_delete(gr1);
    game_delete(gr2);
    game_delete(gru1);
    return false;
  }
  game_print(gr1);
  game_delete(gr);
  game_delete(gr1);
  game_delete(gr2);
  game_delete(gru1);
  return true;
}

bool test_game_fill_grass_row_col(void) {
  game g = game_default();
  game_play_move(g, 0, 0, TENT);
  game gc = game_copy(g);
  game_fill_grass_row(g, 0);
  if (game_get_square(g, 0, 0) != TENT) {
    printf("a");
    game_delete(g);
    game_delete(gc);
    return false;
  }
  if (game_get_square(g, 0, 1) != GRASS) {
    printf("b");
    game_delete(g);
    game_delete(gc);
    return false;
  }
  if (game_get_square(g, 0, 5) != TREE) {
    printf("c");
    game_delete(g);
    game_delete(gc);
    return false;
  }
  for (unsigned int i = 1; i < 8; i++) {  // On ne prends pas en compte la première ligne
    for (unsigned int j = 0; j < 8; j++) {
      if (game_get_square(g, i, j) == GRASS) {
        printf("d");
        game_delete(g);
        game_delete(gc);
        return false;
      }
    }
  }
  for (unsigned int i = 0; i < 5; i++) {
    game_undo(g);
  }
  game_play_move(g, 0, 1, TENT);
  game_play_move(gc, 0, 1,
                 TENT);  // Cette manoeuvre permet de mettre undo et
                         // redo à égalité !
  if (!game_equal(g, gc)) {
    game_print(g);
    game_print(gc);
    game_delete(g);
    game_delete(gc);
    return false;
  }
  game_delete(g);
  game_delete(gc);
  return true;
}

bool test_game_equalv2(square *s, uint *row, uint *col) {
  game g1 = game_new_ext(8, 8, s, row, col, true, false);
  game g2 = game_new_ext(8, 8, s, row, col, false, false);
  game g3 = game_new_ext(8, 8, s, row, col, false, true);
  if (game_equal(g1, g2)) {
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    return false;
  }
  if (game_equal(g2, g3)) {
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  return true;
}

bool test_game_load(void) {
  game g = game_load("data/game_default.tnt");
  game g2 = game_default();
  bool test = game_equal(g, g2);
  game_delete(g);
  game_delete(g2);
  return test;
}

int main(int argc, char *argv[]) {
  bool test = true;
  if (argc == 1) usage(argc, argv);
  // Test dummy
  int result = strcmp(argv[1], "dummy");
  if (result == 0) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  }
  // Test get_square
  if (strcmp(argv[1], "get_square") == 0) {
    test = test_get_square();
    printf("Valeur de test : %d \n", test);
  }
  // Test game_equal
  if (strcmp(argv[1], "equal") == 0) {
    test = test_game_equal();
    printf("Valeur de test : %d\n", test);
  }
  // Test game_set_square
  if (strcmp(argv[1], "set_square") == 0) {
    test = test_set_square();
    printf("Valeur de test : %d \n", test);
  }
  // Test set_tents_row
  if (strcmp(argv[1], "set_tents_row") == 0) {
    test = test_game_set_expected_nb_tents_row();
    printf("Valeur de test : %d \n", test);
  }

  // Test set_tents_row
  if (strcmp(argv[1], "set_tents_col") == 0) {
    test = test_game_set_expected_nb_tents_col();
    printf("Valeur de test : %d \n", test);
  }
  // Test game_new_empty
  if (strcmp(argv[1], "new_empty") == 0) {
    test = test_game_new_empty();
    printf("Valeur de test : %d \n", test);
  }
  // Test game_new
  if (strcmp(argv[1], "new") == 0) {
    test = test_game_new();
    printf("Valeur de test : %d \n", test);
  }
  // Test game_copy
  if (strcmp(argv[1], "copy") == 0) {
    test = test_game_copy();
    printf("Valeur de test : %d\n", test);
  }
  // Test_game_current_tents
  if (strcmp(argv[1], "current_tents") == 0) {
    test = test_game_get_current_nb_tents_all();
    printf("Valeur de test : %d\n", test);
  }
  // Test_game_expected_tents
  if (strcmp(argv[1], "expected_tents") == 0) {
    test = test_game_get_expected_nb_tents_all();
    printf("Valeur de test : %d\n", test);
  }
  // Test_game_get_current_col
  if (strcmp(argv[1], "current_col") == 0) {
    test = test_game_get_current_col();
    printf("Valeur de test : %d\n", test);
  }
  // Test_game_check_move
  if (strcmp(argv[1], "check_move") == 0) {
    test = test_game_check_move();
    printf("Valeur de test : %d\n", test);
  }
  // Test_game_is_over
  if (strcmp(argv[1], "is_over") == 0) {
    test = test_game_is_over();
    printf("Valeur de test : %d\n", test);
  }
  //************DEBUT TEST V2*******************//
  // Test_game_undo
  if (strcmp(argv[1], "undo") == 0) {
    test = test_game_undo(s, row, col);
    printf("Valeur de test : %d\n", test);
  }
  // Test_game_checkv2
  if (strcmp(argv[1], "checkv2") == 0) {
    test = test_game_checkv2(s, row, col);
    printf("Valeur de test : %d\n", test);
  }
  // Test_game_restartv2
  if (strcmp(argv[1], "restartv2") == 0) {
    test = test_restart_v2();
    printf("Valeur de test : %d\n", test);
  }
  // Test_game_fill_grassv2
  if (strcmp(argv[1], "fill_grassv2") == 0) {
    test = test_game_fill_grass_row_col();
    printf("Valeur de test : %d\n", test);
  }
  // Test_game_fill_equalv2
  if (strcmp(argv[1], "equalv2") == 0) {
    test = test_game_equalv2(s, row, col);
    printf("Valeur de test : %d\n", test);
  }
  // Test_game_load
  if (strcmp(argv[1], "game_load") == 0) {
    test = test_game_load();
    printf("Valeur de test : %d\n", test);
  }
  // Fin des test
  return !test;
}