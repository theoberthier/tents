#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "toolbox.h"

game game_load(char* filename) {
  // commençon par vérifier si le pointeur existe
  assert(filename);
  // ouverture du fichier ne mode lecture
  FILE* f = fopen(filename, "r");
  assert(f);
  // si l'ouverture n'est pas null on pourra lire le fichier avec fgetc fgets fscanf
  int nb_rows, nb_cols, wrap, diag, nb_tents_rows, nb_tents_cols;
  char space;  // Permet d'éviter les conflits où plusieurs espaces seraient lues
  uint r = fscanf(f, "%d %d %d %d\n", &nb_rows, &nb_cols, &wrap, &diag);
  assert(r);
  game g = game_new_empty_ext(nb_rows, nb_cols, wrap, diag);
  for (int i = 0; i < game_nb_rows(g); i++) {
    uint d = fscanf(f, "%d", &nb_tents_rows);
    assert(d);
    char c = fscanf(f, "%c", &space);
    assert(c);
    game_set_expected_nb_tents_row(g, i, nb_tents_rows);
  }
  for (int j = 0; j < game_nb_cols(g); j++) {
    r = fscanf(f, "%d", &nb_tents_cols);
    assert(r);
    char c = fscanf(f, "%c", &space);
    assert(c);
    game_set_expected_nb_tents_col(g, j, nb_tents_cols);
  }
  fgetc(f);
  for (int i = 0; i < game_nb_rows(g); i++) {
    char c;
    for (int j = 0; j < game_nb_cols(g); j++) {
      c = fgetc(f);
      if (c == ' ') {
        game_set_square(g, i, j, EMPTY);
      }
      if (c == 'x') {
        game_set_square(g, i, j, TREE);
      }
      if (c == '*') {
        game_set_square(g, i, j, TENT);
      }
      if (c == '-') {
        game_set_square(g, i, j, GRASS);
      }
    }
    c = fscanf(f, "%c", &c);
    assert(c);
  }
  fclose(f);
  return g;
}

void game_save(cgame g, char* filename) {
  // commençon par vérifier si le pointeur existe
  assert(filename);
  // ouverture du fichier ne mode écriture
  FILE* f = fopen(filename, "w");
  assert(f);  // si l'ouverture n'est pas null on pourra utiliser fprintf pour écrire les différente information jeu a sauvegarder
  fprintf(f, "%d %d %d %d\n", game_nb_rows(g), game_nb_cols(g), game_is_wrapping(g), game_is_diagadj(g));
  for (int i = 0; i < game_nb_rows(g); i++) {
    fprintf(f, "%d ", game_get_expected_nb_tents_row(g, i));
  }
  fprintf(f, "\n");
  for (int j = 0; j < game_nb_cols(g); j++) {
    fprintf(f, "%d ", game_get_expected_nb_tents_col(g, j));
  }
  fprintf(f, "\n");
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        fprintf(f, " ");
      } else if (game_get_square(g, i, j) == TREE) {
        fprintf(f, "x");
      } else if (game_get_square(g, i, j) == GRASS) {
        fprintf(f, "-");
      } else if (game_get_square(g, i, j) == TENT) {
        fprintf(f, "*");
      }
    }
    fprintf(f, "\n");
  }
  fclose(f);
}

bool game_solve(game g) {
  if (game_is_over(g)) {
    return true;
  }
  game g_mod = game_copy(g);
  int empty_n[game_nb_rows(g) * game_nb_cols(g)];
  bool last_n[game_nb_rows(g) * game_nb_cols(g)];  // True si dernier empty de la ligne, false sinon
  uint n = 0;
  for (int i = 0; i < game_nb_rows(g_mod); i++) {
    for (int j = 0; j < game_nb_cols(g_mod); j++) {
      if (game_check_move(g_mod, i, j, TENT) == 2) {  // Ajout des herbes
        game_set_square(g_mod, i, j, GRASS);
      }
    }
  }
  // game_print(g_mod);
  uint cpt = 0;
  for (int i = 0; i < game_nb_rows(g_mod); i++) {
    for (int j = 0; j < game_nb_cols(g_mod); j++) {  // Pour chaque ligne
      if (game_get_square(g_mod, i, j) == EMPTY) {
        cpt++;
      }
    }
    if (cpt == game_get_expected_nb_tents_row(g_mod, i)) {
      game_fill_tent_row(g_mod, i);
    }
    cpt = 0;
  }
  // game_print(g_mod);
  for (int j = 0; j < game_nb_cols(g_mod); j++) {
    for (int i = 0; i < game_nb_rows(g_mod); i++) {  // Pour chaque colonne
      if ((game_get_square(g_mod, i, j) == EMPTY) || (game_get_square(g_mod, i, j) == TENT)) {
        // printf("i=%d,j=%d\n",i,j);
        cpt++;
      }
    }
    // printf("j = %d, cpt = %d\n",j,cpt);
    if (cpt == game_get_expected_nb_tents_col(g_mod, j)) {
      game_fill_tent_col(g_mod, j);
      // printf("entrée\n");
    }
    cpt = 0;
  }
  for (int i = 0; i < game_nb_rows(g_mod); i++) {  // Tableau contenant la loc des cases vides
    for (int j = 0; j < game_nb_cols(g_mod); j++) {
      if (game_get_square(g_mod, i, j) == EMPTY) {
        empty_n[n] = j + game_nb_cols(g) * i;
        n++;
      }
    }
  }
  if (n < 3) {
    if (game_is_over(g_mod)) {
      empty_n[0] = -1;
      return solve_game(g, g_mod, empty_n, last_n, 0);
    }
  }
  // game_print(g_mod);
  uint compt = 1;
  while (compt > 0) {
    compt = 0;
    // ajout herbe
    for (int i = 0; i < game_nb_rows(g_mod); i++) {
      for (int j = 0; j < game_nb_cols(g_mod); j++) {
        if ((game_get_square(g, i, j) == EMPTY) && (game_check_move(g_mod, i, j, TENT) == 2)) {  // Ajout des herbes
          game_set_square(g_mod, i, j, GRASS);
        }
      }
    }
    cpt = 0;
    for (int i = 0; i < game_nb_rows(g_mod); i++) {
      for (int j = 0; j < game_nb_cols(g_mod); j++) {  // Pour chaque ligne
        if ((game_get_square(g_mod, i, j) == EMPTY) || (game_get_square(g_mod, i, j) == TENT)) {
          cpt++;
        }
      }
      if (cpt == game_get_expected_nb_tents_row(g_mod, i)) {
        game_fill_tent_row(g_mod, i);
        // compt++;
      }
      cpt = 0;
    }
    // game_print(g_mod);
    for (int j = 0; j < game_nb_cols(g_mod); j++) {
      for (int i = 0; i < game_nb_rows(g_mod); i++) {  // Pour chaque colonne
        if ((game_get_square(g_mod, i, j) == EMPTY) || (game_get_square(g_mod, i, j) == TENT)) {
          // printf("i=%d,j=%d\n",i,j);
          cpt++;
        }
      }
      // printf("j = %d, cpt = %d\n",j,cpt);
      if (cpt == game_get_expected_nb_tents_col(g_mod, j)) {
        game_fill_tent_col(g_mod, j);
        // compt++;
        // printf("entrée\n");
      }
      cpt = 0;
    }
    // Pour chaque arbre
    for (int i = 0; i < game_nb_rows(g_mod); i++) {  // Pour chaque arbre
      for (int j = 0; j < game_nb_cols(g_mod); j++) {
        if (game_get_square(g_mod, i, j) == TREE && !(game_check_adj_ort_square(g_mod, i, j, TENT))) {
          // Si fonction != -1 alors on place une tente en pos
          int pos = pos_placeable_tent(g_mod, i, j);
          if (pos != -1) {
            game_set_square(g_mod, pos / game_nb_cols(g_mod), pos % game_nb_cols(g_mod), TENT);
            compt++;
          }
        }
      }
    }
    // printf("avant\n");
    // game_print(g_mod);
    for (uint i = 0; i < game_nb_rows(g_mod); i++) {
      compt += only_row(g_mod, i);
    }
    // printf("apres roww\n");
    // game_print(g_mod);
    for (uint j = 0; j < game_nb_cols(g_mod); j++) {
      compt += only_col(g_mod, j);
    }
    // printf("apres col \n");
    // game_print(g_mod);
  }
  // game_print(g_mod);
  // printf("il vit en toi\n");
  n = 0;
  for (int i = 0; i < game_nb_rows(g_mod); i++) {  // On reforme le tableau des empty
    for (int j = 0; j < game_nb_cols(g_mod); j++) {
      if (game_get_square(g_mod, i, j) == EMPTY) {
        empty_n[n] = j + game_nb_cols(g) * i;
        n++;
      }
    }
  }
  uint fst;
  uint snd;
  if (n > 0) {
    for (uint i = 0; i < n - 1; i++) {  // Tableau nous disant si la case empty est la derniere de la ligne
      fst = empty_n[i];
      snd = empty_n[i + 1];
      uint i_fst = fst / game_nb_rows(g_mod);
      uint i_snd = snd / game_nb_rows(g_mod);
      if (i_fst == i_snd) {
        last_n[i] = false;
        last_n[i + 1] = true;
      } else {
        last_n[i] = true;
      }
    }
  } else {
    last_n[0] = true;
  }
  if (n > 0) {
    last_n[n - 1] = true;
  } else {
    last_n[n] = true;
  }
  empty_n[n] = -1;  // Nous donne notre condition d'arret !
  // game_print(g_mod);
  // printf(" n = %d\n", n);
  return solve_game(g, g_mod, empty_n, last_n, 0);
}

uint game_nb_solutions(game g) {
  if (game_is_over(g)) {
    return 1;
  }
  game g_mod = game_copy(g);
  int empty_n[game_nb_rows(g) * game_nb_cols(g)];
  bool last_n[game_nb_rows(g) * game_nb_cols(g)];  // True si dernier empty de la ligne, false sinon
  uint n = 0;
  for (int i = 0; i < game_nb_rows(g_mod); i++) {
    for (int j = 0; j < game_nb_cols(g_mod); j++) {
      if (game_check_move(g_mod, i, j, TENT) == 2) {  // Ajout des herbes
        game_set_square(g_mod, i, j, GRASS);
      }
    }
  }
  uint cpt = 0;
  for (int i = 0; i < game_nb_rows(g_mod); i++) {
    for (int j = 0; j < game_nb_cols(g_mod); j++) {  // Pour chaque ligne
      if (game_get_square(g_mod, i, j) == EMPTY) {
        cpt++;
      }
    }
    if (cpt == game_get_expected_nb_tents_row(g_mod, i)) {
      game_fill_tent_row(g_mod, i);
    }
    cpt = 0;
  }
  for (int j = 0; j < game_nb_cols(g_mod); j++) {
    for (int i = 0; i < game_nb_rows(g_mod); i++) {  // Pour chaque colonne
      if ((game_get_square(g_mod, i, j) == EMPTY) || (game_get_square(g_mod, i, j) == TENT)) {
        cpt++;
      }
    }
    if (cpt == game_get_expected_nb_tents_col(g_mod, j)) {
      game_fill_tent_col(g_mod, j);
    }
    cpt = 0;
  }
  for (int i = 0; i < game_nb_rows(g_mod); i++) {  // Réajout des herbes
    for (int j = 0; j < game_nb_cols(g_mod); j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        if (game_check_move(g_mod, i, j, TENT) == 2) {
          game_set_square(g_mod, i, j, GRASS);
        }
      }
    }
  }

  for (int i = 0; i < game_nb_rows(g_mod); i++) {  // Tableau contenant la loc des cases vides
    for (int j = 0; j < game_nb_cols(g_mod); j++) {
      if (game_get_square(g_mod, i, j) == EMPTY) {
        empty_n[n] = j + game_nb_cols(g) * i;
        n++;
      }
    }
  }
  uint fst;
  uint snd;
  if (n > 0) {
    for (uint i = 0; i < n - 1; i++) {  // Tableau nous disant si la case empty est la derniere de la ligne
      fst = empty_n[i];
      snd = empty_n[i + 1];
      uint i_fst = fst / game_nb_rows(g_mod);
      uint i_snd = snd / game_nb_rows(g_mod);
      if (i_fst == i_snd) {
        last_n[i] = false;
        last_n[i + 1] = true;
      } else {
        last_n[i] = true;
      }
    }
  } else {
    last_n[0] = true;
  }
  if (n > 0) {
    last_n[n - 1] = true;
  } else {
    last_n[n] = true;
  }
  empty_n[n] = -1;  // Nous donne notre condition d'arret !
  return solve_nb_sol(g_mod, empty_n, last_n, 0);
}