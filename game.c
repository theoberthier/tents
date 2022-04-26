#include "game.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "game_aux.h"
#include "game_tools.h"
#include "toolbox.h"

struct Historic {
  square* square;
  unsigned int* row;
  unsigned int* col;
  unsigned int size;
  unsigned int capacity;
};

struct game_s {
  square* tab;
  unsigned int* row;
  unsigned int* col;
  bool wrapping;
  bool diagadj;
  uint nb_rows;
  uint nb_cols;
  struct Historic played;   // Structure permettant d'annuler les coups déjà joués
  struct Historic deleted;  // Structure permettant de rejouer les coups précédemment annulés
};

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping, bool diagadj) {
  game g = (game)malloc(sizeof(struct game_s));
  assert(g);
  g->nb_rows = nb_rows;
  g->nb_cols = nb_cols;
  g->wrapping = wrapping;
  g->diagadj = diagadj;
  g->tab = (square*)calloc(nb_rows * nb_cols + 1, sizeof(uint));  // EMPTY=0
  assert(g->tab);
  g->col = (uint*)calloc(g->nb_cols, sizeof(uint));
  assert(g->col);
  g->row = (uint*)calloc(g->nb_rows, sizeof(uint));
  assert(g->row);
  // Ces valeurs permettront d'initialiser l'historique au complet quand on joue un coup ou qu'on en annule un
  g->played.capacity = 0;
  g->deleted.capacity = 0;
  g->deleted.size = 0;
  g->played.size = 0;
  return g;
}

game game_new_ext(uint nb_rows, uint nb_cols, square* squares, uint* nb_tents_row, uint* nb_tents_col, bool wrapping, bool diagadj) {
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, diagadj);
  assert(g);
  // On copie les valeurs des paramètres dans notre jeu g
  for (int i = 0; i < nb_rows * nb_cols; i++) {
    g->tab[i] = squares[i];
  }
  for (int j = 0; j < nb_rows; j++) {
    g->row[j] = nb_tents_row[j];
  }
  for (int j = 0; j < nb_cols; j++) {
    g->col[j] = nb_tents_col[j];
  }
  return g;
}

game game_new(square* squares, uint* nb_tents_row, uint* nb_tents_col) { return game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares, nb_tents_row, nb_tents_col, false, false); }

game game_new_empty(void) { return game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, false, false); }

uint game_nb_rows(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "invalid memory\n");
    exit(EXIT_FAILURE);
  }
  return g->nb_rows;
}

uint game_nb_cols(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "invalid memory\n");
    exit(EXIT_FAILURE);
  }
  return g->nb_cols;
}

game game_copy(cgame g) { return game_new_ext(g->nb_rows, g->nb_cols, g->tab, g->row, g->col, g->wrapping, g->diagadj); }

bool game_is_wrapping(cgame g) { return g->wrapping; }

bool game_is_diagadj(cgame g) { return g->diagadj; }

bool game_equal(cgame g1, cgame g2) {
  // On compare un à un chacun des squares des deux jeux
  for (unsigned int i = 0; i < game_nb_rows(g1); i++) {
    for (unsigned int j = 0; j < game_nb_cols(g1); j++) {
      if (game_get_square(g1, i, j) != game_get_square(g2, i, j)) {
        return false;
      }
    }
  }
  // On compare un à un le nombre de tentes attendues dans en colonnes puis en lignes des deux jeux
  for (unsigned int i = 0; i < game_nb_cols(g1); i++) {
    if (game_get_expected_nb_tents_col(g1, i) != game_get_expected_nb_tents_col(g2, i)) {
      return false;
    }
    if (g1->col[i] != g2->col[i]) {
      return false;
    }
  }
  for (uint i = 0; i < game_nb_rows(g1); i++) {
    if (game_get_expected_nb_tents_row(g1, i) != game_get_expected_nb_tents_row(g2, i)) {
      return false;
    }
    if (g1->row[i] != g2->row[i]) {
      return false;
    }
  }
  // On compare les status wrapping et diagadj des deux jeux
  if (game_is_wrapping(g1) != game_is_wrapping(g2) || game_is_diagadj(g1) != game_is_diagadj(g2) || g1->nb_cols != g2->nb_cols || g1->nb_rows != g2->nb_rows) {
    return false;
  }
  return true;
}

void game_delete(game g) {
  // On libére l'ensemble des allocations faîtes dans le jeu g
  if (g != NULL) {
    free(g->col);
    free(g->row);
    free(g->tab);
    // On libére les historiques uniquement si leur capacité est différente de 0
    if (g->played.capacity != 0) {
      free(g->played.square);
      free(g->played.col);
      free(g->played.row);
    }
    if (g->deleted.capacity != 0) {
      free(g->deleted.square);
      free(g->deleted.col);
      free(g->deleted.row);
    }
  }
  free(g);
}

void game_set_square(game g, uint i, uint j, square s) {
  // j + game_nb_cols(g) * i correspond à la conversion des index d'un tableau 2d vers un tableau 1d
  g->tab[j + game_nb_cols(g) * i] = s;
}

square game_get_square(cgame g, uint i, uint j) {
  square s = g->tab[j + game_nb_cols(g) * i];
  return s;
}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents) { g->row[i] = nb_tents; }

void game_set_expected_nb_tents_col(game g, uint i, uint nb_tents) { g->col[i] = nb_tents; }

uint game_get_expected_nb_tents_row(cgame g, uint i) {
  if (g == NULL || i >= game_nb_rows(g) * game_nb_cols(g)) {
    fprintf(stderr, "Indice non défini");
    EXIT_FAILURE;
  }
  return g->row[i];
}

uint game_get_expected_nb_tents_col(cgame g, uint j) {
  if (g == NULL || j >= game_nb_rows(g) * game_nb_cols(g)) {
    fprintf(stderr, "Indice non défini");
    EXIT_FAILURE;
  }
  return g->col[j];
}

uint game_get_expected_nb_tents_all(cgame g) {
  if (g == NULL || g->col == NULL || g->row == NULL) {
    fprintf(stderr, "Memory error\n");
    EXIT_FAILURE;
  }
  // On somme le nombre de tentes attendues sur toutes les colonnes
  uint all = 0;
  for (unsigned int i = 0; i < game_nb_cols(g); i++) {
    all += g->col[i];
  }
  return all;
}

uint game_get_current_nb_tents_row(cgame g, uint i) {
  // On compte le nombre de tente en regardant un à un sur la ligne
  uint row = 0;
  for (unsigned int j = 0; j < game_nb_cols(g); j++) {
    if (game_get_square(g, i, j) == TENT) {
      row = row + 1;
    }
  }
  return row;
}

uint game_get_current_nb_tents_col(cgame g, uint j) {
  // On compte le nombre de tente en regardant un à un sur la colonne
  uint col = 0;
  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    if (game_get_square(g, i, j) == TENT) {
      col = col + 1;
    }
  }
  return col;
}

uint game_get_current_nb_tents_all(cgame g) {
  // On somme le nombre de tentes présentes sur toutes les lignes
  uint all = 0;
  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    all = all + game_get_expected_nb_tents_row(g, i);
  }
  return all;
}

bool game_check_adj_ort_square(cgame g, uint i, uint j, square s) {
  uint r = game_nb_rows(g);
  uint c = game_nb_cols(g);
  // A gauche
  if (j > 0) {  // On regarde à gauche uniquement si on est pas sur la première colonne
    if (game_get_square(g, i, j - 1) == s) {
      return true;
    }
  }
  // A Droite
  if (j < c - 1) {  // On regarde à droite uniquement si on est pas sur la dernière colonne
    if (game_get_square(g, i, j + 1) == s) {
      return true;
    }
  }
  // En Dessous
  if (i < r - 1) {  // On regarde en dessous uniquement si on est pas sur la dernière ligne
    if (game_get_square(g, i + 1, j) == s) {
      return true;
    }
  }
  // Au Dessus
  if (i > 0) {  // On regarde au dessus uniquement si on est pas sur la première ligne
    if (game_get_square(g, i - 1, j) == s) {
      return true;
    }
  }
  if (g->wrapping) {
    if (i == r - 1) {  // Si i est sur la dernière ligne, on regarde en 0
      if (game_get_square(g, 0, j) == s) {
        return true;
      }
    }
    if (j == c - 1) {  // Si j est sur la dernière colonne, on regarde en 0
      if (game_get_square(g, i, 0) == s) {
        return true;
      }
    }
    if (i == 0) {  // Si i est sur la première ligne, on regarde à la dernière
      if (game_get_square(g, r - 1, j) == s) {
        return true;
      }
    }
    if (j == 0) {  // Si j est sur la première colonne, on regarde à la dernière
      if (game_get_square(g, i, c - 1) == s) {
        return true;
      }
    }
  }
  return false;  // Cas où aucun arbre est adjacent
}

bool game_check_adj_diag_square(cgame g, uint i, uint j,
                                square s) {  // Cette fonction retourne true si la case est adjacente diagonalement à un square donné, false sinon
  uint r = game_nb_rows(g);
  uint c = game_nb_cols(g);
  // Diagonale NW
  if (j > 0 && i > 0) {
    if (game_get_square(g, i - 1, j - 1) == s) {
      return true;
    }
  }
  // Diagonale NE
  if (j < (c - 1) && i > 0) {
    if (game_get_square(g, i - 1, j + 1) == s) {
      return true;
    }
  }
  // Diagonale SW
  if (j > 0 && i < (r - 1)) {
    if (game_get_square(g, i + 1, j - 1) == s) {
      return true;
    }
  }
  // Diagonale SE
  if (j < (c - 1) && i < (r - 1)) {
    if (game_get_square(g, i + 1, j + 1) == s) {
      return true;
    }
  }
  // On traite le cas où le wrapping est activé
  if (g->wrapping) {
    // Cas i = 0
    if (i == 0) {
      if (j == 0) {  // Cas i & j = 0
        if (game_get_square(g, r - 1, c - 1) == s) {
          return true;
        }
      }
      if (j == c - 1) {  // Cas i = 0 & j = fin
        if (game_get_square(g, r - 1, 0) == s) {
          return true;
        }
      }
      // Cas général pour i = 0
      if ((j < c - 1) && (j > 0)) {  // On s'assure de ne pas sortir du tableau
        if ((game_get_square(g, r - 1, j - 1) == s) || (game_get_square(g, r - 1, j + 1) == s)) {
          return true;
        }
      }
    }
    if (j == 0) {
      if (i == r - 1) {  // Cas j = 0 & i = fin
        if (game_get_square(g, 0, c - 1) == s) {
          return true;
        }
      }
      // Cas général pour j = 0
      if ((i < r - 1) && (i > 0)) {  // On s'assure de ne pas sortir du tableau
        if ((game_get_square(g, i - 1, c - 1) == s) || (game_get_square(g, i - 1, c + 1) == s)) {
          return true;
        }
      }
    }
    if ((i == r - 1) && (j == c - 1)) {
      if (game_get_square(g, 0, 0) == s) {
        return true;
      }
    }
  }
  return false;
}

bool game_check_placeable_tent(cgame g, uint i, uint j) {
  if (game_check_adj_ort_square(g, i, j,
                                TREE) == 0) {  // S'il n'y a pas d'arbre adjacent, le cas ne nous intéresse pas
    return 1;
  }
  // row et col sont les tableaux des indices des positions adjacentes à celles données en paramètre
  int row[4];
  int col[4];
  row[0] = i;
  col[0] = j - 1;  // 0 : A gauche
  row[1] = i - 1;
  col[1] = j;  // 1 : Au dessus
  row[2] = i;
  col[2] = j + 1;  // 2 : A droite
  row[3] = i + 1;
  col[3] = j;  // 3 : En dessous

  for (unsigned int k = 0; k < 4; k++) {                                                               // On formate le tableau de sorte à ce que seuls les cases avec des arbres soient utilisables
    if ((row[k] >= 0) && (col[k] >= 0) && (row[k] < game_nb_rows(g)) && (col[k] < game_nb_cols(g))) {  // On ne prends pas en compte les valeurs en dehors du jeu
      if (game_get_square(g, row[k], col[k]) == TREE) {
        if (!(game_check_adj_ort_square(g, row[k], col[k], EMPTY) || game_check_adj_ort_square(g, row[k], col[k],
                                                                                               TENT))) {  // S'il n'y a pas de place pour une tente, on retourne faux
          return 0;
        }
      }
    }
  }
  // S'il n'y a pas de problème, on retourne true
  return 1;
}

int pos_placeable_tent(cgame g, uint i, uint j) {  // On donne la position de la tente placeable a côté de l'arbre en i,j si elle existe et est unique
  int row[4];
  int col[4];
  row[0] = i;
  col[0] = j - 1;  // 0 : A gauche
  row[1] = i - 1;
  col[1] = j;  // 1 : Au dessus
  row[2] = i;
  col[2] = j + 1;  // 2 : A droite
  row[3] = i + 1;
  col[3] = j;  // 3 : En dessous
  bool alone = false;
  uint n = -1;
  // printf("i = %d, j = %d\n",i,j);
  for (unsigned int k = 0; k < 4; k++) {                                                               // On formate le tableau de sorte à ce que seuls les cases avec des arbres soient utilisables
    if ((row[k] >= 0) && (col[k] >= 0) && (row[k] < game_nb_rows(g)) && (col[k] < game_nb_cols(g))) {  // On ne prends pas en compte les valeurs en dehors du jeu
      if (game_get_square(g, row[k], col[k]) == EMPTY) {
        if (game_check_move(g, row[k], col[k], TENT) == 1) {
          if (alone) {
            // printf("stop, row = %d, col = %d\n",row[k],col[k]);
            return -1;
          } else {
            alone = true;
            n = col[k] + row[k] * game_nb_cols(g);
            // printf("n ok = %d,row = %d, col = %d\n",n,row[k],col[k]);
            // game_print(g);
          }
        }
      }
    }
  }
  if (alone) {
    return n;
  } else {
    return -1;
  }
}

void hist_play(game g, uint i, uint j) {
  if (g->played.capacity == 0) {
    // On alloue les tableaux si la capacité est à 0
    g->played.capacity = 1;
    g->played.row = malloc(sizeof(uint));
    g->played.col = malloc(sizeof(uint));
    g->played.square = malloc(sizeof(square));
    g->played.size = 0;
  } else if (g->played.size >= g->played.capacity) {
    // On double la capacité et on réalloue les tableaux en conséquences si la capacité est trop petite
    g->played.capacity *= 2;
    g->played.row = realloc(g->played.row, g->played.capacity * sizeof(uint));
    g->played.col = realloc(g->played.col, g->played.capacity * sizeof(uint));
    g->played.square = realloc(g->played.square, g->played.capacity * sizeof(uint));
  }
  // On met à jour l'historique des coups joués en gardant en mémoire le square précédent
  uint n = g->played.size;
  g->played.row[n] = i;
  g->played.col[n] = j;
  g->played.square[n] = game_get_square(g, i, j);
  g->played.size += 1;
  if (g->deleted.size != 0) {
    // On remet les coups à refaire à 0 lorsqu'on joue un coup
    free(g->deleted.row);
    free(g->deleted.col);
    free(g->deleted.square);
    g->deleted.size = 0;
    g->deleted.capacity = 0;
  }
}

void game_play_move(game g, uint i, uint j, square s) {
  if (s != TREE && game_get_square(g, i, j) != TREE) {
    // On appelle la fonction qui gère l'historique des coups jouées puis on joue le coup
    hist_play(g, i, j);
    game_set_square(g, i, j, s);
  }
}

int game_check_move(cgame g, uint i, uint j, square s) {
  // Cas des mouvements illegaux
  if (s == TREE || game_get_square(g, i, j) == TREE) {
    return 0;
  }
  if (i < 0 || j < 0 || i > game_nb_rows(g) - 1 || j > (game_nb_cols(g)) - 1) {  // Cas où on sort des limites du jeu
    return 0;
  }
  // Cas des mouvements perdants
  if (s == TENT && (game_get_square(g, i,
                                    j) != TENT)) {  // On ne traite pas le cas où on place une tente déjà presente
    if (!game_check_adj_ort_square(g, i, j,
                                   TREE)) {  // Placer une tente non adjacente à un arbre est perdant
      return 2;
    }
    if (game_is_diagadj(g)) {
      if (game_check_adj_ort_square(g, i, j,
                                    TENT)) {  // Placer deux tentes adjacentes est losing sauf si elle sont en diagonal avec le mode diagadj
        return 2;
      }
    } else {
      if (game_check_adj_ort_square(g, i, j, TENT) || game_check_adj_diag_square(g, i, j,
                                                                                 TENT)) {  // Placer deux tentes adjacentes est losing, même diagonalement
        return 2;
      }
    }
    if (game_get_current_nb_tents_row(g, i) >= game_get_expected_nb_tents_row(g,
                                                                              i)) {  // Jouer plus de tentes qu'attendus sur une ligne est losing
      return 2;
    }
    if (game_get_current_nb_tents_col(g, j) >= game_get_expected_nb_tents_col(g,
                                                                              j)) {  // Jouer plus de tentes qu'attendus sur une colonne est losing
      return 2;
    }
  }
  if (s == GRASS && (game_get_square(g, i,
                                     j) != GRASS)) {  // Si c'est de l'herbe, on vérifie qu'il y ait la place pour le nombre de tentes attendues
    // On ne traite pas le cas où on place de l'herbe déjà presente
    int compt_row = 0;
    int compt_col = 0;
    if ((game_get_expected_nb_tents_col(g, j) == 0) || (game_get_expected_nb_tents_row(g, i) == 0)) {
      return 1;
    }
    for (unsigned int k = 0; k < game_nb_rows(g); k++) {  // On compte le nombre de tentes + le nombre de empty (pour savoir la place possible)
      if ((game_get_square(g, i, k) == TENT) || (game_get_square(g, i, k) == EMPTY)) {
        compt_row++;
      }
    }
    for (unsigned int k = 0; k < game_nb_cols(g); k++) {
      if ((game_get_square(g, k, j) == TENT) || (game_get_square(g, k, j) == EMPTY)) {
        compt_col++;
      }
    }
    if (game_get_expected_nb_tents_row(g,
                                       i) > compt_row - 1) {  // On met strictement sup car on remplace une case vide ou une tente par de l'herbe, on perds donc une place pour une tente
      return 2;
    }
    if (game_get_expected_nb_tents_col(g,
                                       j) > compt_col - 1) {  // On met strictement sup car on remplace une case vide ou une tente par de l'herbe, on perds donc une place pour une tente
      return 2;
    }
    game gc = game_copy(g);
    game_set_square(gc, i, j, GRASS);
    if (!game_check_placeable_tent(gc, i,
                                   j)) {  // On vérifie qu'une tente reste plaçable autour des arbres adjacents si on effectue le mouvement
      return 2;
    }
    game_delete(gc);
    // On va regarder si placer de l'herbe bloque les autres arbres de la lignes / colonnes
  }
  return 1;  // Les autres coups sont regular
}

bool game_is_over(cgame g) {
  // Regle 1 : Pas deux tentes adjacentes, mêmes diagonales
  if (game_is_diagadj(g)) {
    for (unsigned int i = 0; i < game_nb_cols(g); i++) {
      for (unsigned int j = 0; j < game_nb_rows(g); j++) {
        if (game_get_square(g, i, j) == TENT) {
          if (game_check_adj_ort_square(g, i, j, TENT)) {
            return 0;
          }
        }
      }
    }
  } else {
    for (unsigned int i = 0; i < game_nb_rows(g); i++) {
      for (unsigned int j = 0; j < game_nb_cols(g); j++) {
        if (game_get_square(g, i, j) == TENT) {
          if (game_check_adj_ort_square(g, i, j, TENT) || game_check_adj_diag_square(g, i, j, TENT)) {
            return 0;
          }
        }
      }
    }
  }
  // Regle 2 : Nombre de tentes par lignes et par colonnes sont celles prévues
  for (unsigned int j = 0; j < game_nb_cols(g); j++) {
    if ((game_get_expected_nb_tents_col(g, j) != game_get_current_nb_tents_col(g, j))) {
      return 0;
    }
  }
  for (uint i = 0; i < game_nb_rows(g); i++) {
    if (game_get_expected_nb_tents_row(g, i) != game_get_current_nb_tents_row(g, i)) {
      return 0;
    }
  }

  // Regle 3 : Il y autant de tentes que d'arbres
  uint tents = game_get_current_nb_tents_all(g);
  uint trees = 0;
  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    for (unsigned int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == TREE) {
        trees++;
      }
    }
  }
  if (trees != tents) {
    return 0;
  }
  // Regle 4 : Chaque tente doît être adjacentes ort à au moins un arbre et vice-versa
  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    for (unsigned int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i,
                          j) == TENT) {  // Vérification de chaque tente
        if (!game_check_adj_ort_square(g, i, j, TREE)) {
          return 0;
        }
      }
      if (game_get_square(g, i, j) == TREE) {
        if (!game_check_adj_ort_square(g, i, j, TENT)) {
          return 0;
        }
      }
    }
  }

  return 1;
}

void game_fill_grass_row(game g, uint i) {
  // On place l'herbe un à un en mettant à jour l'historique
  for (int j = 0; j < game_nb_cols(g); j++) {
    if (game_get_square(g, i, j) == EMPTY) {
      hist_play(g, i, j);
      game_set_square(g, i, j, GRASS);
    }
  }
}
void game_fill_grass_col(game g, uint i) {
  // On place l'herbe un à un en mettant à jour l'historique
  for (int j = 0; j < game_nb_rows(g); j++) {
    if (game_get_square(g, j, i) == EMPTY) {
      hist_play(g, i, j);
      game_set_square(g, j, i, GRASS);
    }
  }
}

void game_restart(game g) {
  // On remet l'emsemble des squares à 0
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) != TREE && game_get_square(g, i, j) != EMPTY) {
        game_set_square(g, i, j, EMPTY);
      }
    }
  }
  // On remet les historiques à 0 (non obligatoire)
  if (g->played.capacity > 0) {
    free(g->played.col);
    free(g->played.row);
    free(g->played.square);
    g->played.capacity = 0;
    g->played.size = 0;
  }
  if (g->deleted.capacity > 0) {
    free(g->deleted.col);
    free(g->deleted.row);
    free(g->deleted.square);
    g->deleted.size = 0;
    g->deleted.capacity = 0;
  }
}

void game_undo(game g) {
  if (g->played.size == 0) {  // Cas où l'utilisateur tente d'annuler un coup alors que ce n'est pas possible
    printf("Il n'y a aucun coup à annuler !\n");
    return;
  } else {
    g->played.size--;
    unsigned int n = g->played.size;  // Correspond à l'emplacement du dernier coup joué dans le tableau
    square s = game_get_square(g, g->played.row[n], g->played.col[n]);
    game_set_square(g, g->played.row[n], g->played.col[n],
                    g->played.square[n]);  // On rejoue le coup précédent
    unsigned int m = g->deleted.size;
    // On ajuste la taille des tableaux de l'historique des coups effacés
    if (g->deleted.capacity == 0) {
      g->deleted.capacity = 1;
      g->deleted.row = malloc(sizeof(uint));  // Taille 1
      g->deleted.col = malloc(sizeof(uint));
      g->deleted.square = malloc(sizeof(square));
    } else if (m >= g->deleted.capacity) {
      g->deleted.capacity *= 2;
      g->deleted.row = realloc(g->deleted.row, g->deleted.capacity * sizeof(uint));
      g->deleted.col = realloc(g->deleted.col, g->deleted.capacity * sizeof(uint));
      g->deleted.square = realloc(g->deleted.square, g->deleted.capacity * sizeof(uint));
    }
    // On sauvegarde le coup effacé pour pouvoir utiliser le redo
    g->deleted.row[m] = g->played.row[n];
    g->deleted.col[m] = g->played.col[n];
    g->deleted.square[m] = s;
    g->deleted.size += 1;
  }
}

void game_redo(game g) {
  if (g->deleted.size == 0) {  // Cas où l'utilisateur essaie de rejouer un coup effacé alors que ce n'est pas possible
    printf("Il n'y a aucun coup à refaire !\n");
    return;
  }
  g->deleted.size--;
  unsigned int n = g->deleted.size;  // Correspond à l'emplacement du dernier coup effacé dans le tableau
  square s = game_get_square(g, g->deleted.row[n], g->deleted.col[n]);
  game_set_square(g, g->deleted.row[n], g->deleted.col[n],
                  g->deleted.square[n]);  // On rejoue le dernier coup effacé
  unsigned int m = g->played.size;
  if (m >= g->played.capacity) {  // On ajoute le coup rejoué à l'historique des coups joués
    g->played.capacity *= 2;
    g->played.row = realloc(g->played.row, g->played.capacity * sizeof(uint));
    g->played.col = realloc(g->played.col, g->played.capacity * sizeof(uint));
    g->played.square = realloc(g->played.square, g->played.capacity * sizeof(uint));
  }
  g->played.row[m] = g->deleted.row[n];
  g->played.col[m] = g->deleted.col[n];
  g->played.square[m] = s;
  g->played.size++;
}

void del_empty(int* empty, uint i, uint size) {
  for (uint n = i; n < size - 1; n++) {
    empty[n] = empty[n + 1];
  }
}

uint only_row(game g, uint i) {  // Donne le nombre d'arbres sur la ligne i qui ne peuvent avoir une tente que sur cette même ligne
  uint res = 0;
  uint cpt = 0;
  uint tente_obligatoire[game_nb_cols(g)];
  bool up_down = false;
  int row[4];
  int col[4];
  bool changed = false;
  uint compt_u = 0;
  for (uint j = 0; j < game_nb_cols(g); j++) {
    if ((game_get_square(g, i, j) == TREE) && !(game_check_adj_ort_square(g, i, j, TENT))) {
      up_down = false;  // Pour chaque arbre
      row[0] = i + 1;
      col[0] = j;  // 0 : En dessous
      row[1] = i - 1;
      col[1] = j;  // 1 : Au dessus
      row[2] = i;
      col[2] = j - 1;  // 2 : A gauche
      row[3] = i;
      col[3] = j + 1;  // 3 : A droite

      for (unsigned int k = 0; k < 2; k++) {  // On formate le tableau de sorte à ce que seuls les cases avec des arbres soient utilisables
        if ((row[k] >= 0) && (col[k] >= 0) && (row[k] < game_nb_rows(g)) && (col[k] < game_nb_cols(g))) {  // On ne prends pas en compte les valeurs en dehors du jeu
          if (game_get_square(g, row[k], col[k]) == EMPTY) {
            up_down = true;  //ça veut dire qu'une tente est potentiellement placeable hors de la ligne
          }
        }
      }
      for (unsigned int k = 2; k < 4; k++) {  // On formate le tableau de sorte à ce que seuls les cases avec des arbres soient utilisables
        if ((row[k] >= 0) && (col[k] >= 0) && (row[k] < game_nb_rows(g)) && (col[k] < game_nb_cols(g))) {  // On ne prends pas en compte les valeurs en dehors du jeu
          if (game_get_square(g, row[k], col[k]) == EMPTY) {
            if (!up_down) {
              tente_obligatoire[cpt] = col[k] + row[k] * game_nb_cols(g);
              cpt++;  // Il y aura forcement une tente sur ligne à cet emplacement
              changed = true;
            }  // Il faut ne pas compter s'il y déjà une tente
          }
        }
      }
      if (changed) {
        changed = false;
        compt_u++;
      }
    }
  }
  uint nb_empty = 0;
  uint nb_tents = 0;
  for (uint j = 0; j < game_nb_cols(g); j++) {
    if (game_get_square(g, i, j) == EMPTY) {
      nb_empty++;
    } else if (game_get_square(g, i, j) == TENT) {
      nb_tents++;
    }
  }
  // printf("cpt = %d\n", cpt);
  if (game_get_expected_nb_tents_row(g, i) == nb_tents + compt_u) {
    // Parcourt et place les herbes partout sauf dans les emplacement de tente_obli
    for (uint j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        changed = false;
        for (uint k = 0; k < cpt; k++) {
          if (tente_obligatoire[k] == j + game_nb_cols(g) * i) {
            // printf("tente obli\n");
            changed = true;
          }
        }
        if (!changed) {  // Si l'empty ne fait pas parti des tentes obligatoire alors elle devient de l'herbe
          game_set_square(g, i, j, GRASS);
          res++;
        }
      }
    }
  }
  return res;
}

uint only_col(game g, uint j) {  // Donne le nombre d'arbres sur la ligne i qui ne peuvent avoir une tente que sur cette même ligne
  uint res = 0;
  uint cpt = 0;
  uint tente_obligatoire[game_nb_rows(g)];
  bool left_right = false;
  int row[4];
  int col[4];
  bool changed = false;
  uint compt_u = 0;
  for (uint i = 0; i < game_nb_rows(g); i++) {
    if (game_get_square(g, i, j) == TREE && !(game_check_adj_ort_square(g, i, j, TENT))) {
      left_right = false;  // Pour chaque arbre
      row[0] = i + 1;
      col[0] = j;  // 0 : En dessous
      row[1] = i - 1;
      col[1] = j;  // 1 : Au dessus
      row[2] = i;
      col[2] = j - 1;  // 2 : A gauche
      row[3] = i;
      col[3] = j + 1;  // 3 : A droite

      for (unsigned int k = 2; k < 4; k++) {  // On formate le tableau de sorte à ce que seuls les cases avec des arbres soient utilisables
        if ((row[k] >= 0) && (col[k] >= 0) && (row[k] < game_nb_rows(g)) && (col[k] < game_nb_cols(g))) {  // On ne prends pas en compte les valeurs en dehors du jeu
          if (game_get_square(g, row[k], col[k]) == EMPTY) {
            left_right = true;  //ça veut dire qu'une tente est potentiellement placeable hors de la ligne
          }
        }
      }
      for (unsigned int k = 0; k < 2; k++) {  // On formate le tableau de sorte à ce que seuls les cases avec des arbres soient utilisables
        if ((row[k] >= 0) && (col[k] >= 0) && (row[k] < game_nb_rows(g)) && (col[k] < game_nb_cols(g))) {  // On ne prends pas en compte les valeurs en dehors du jeu
          if (game_get_square(g, row[k], col[k]) == EMPTY) {
            if (!left_right) {
              tente_obligatoire[cpt] = col[k] + row[k] * game_nb_cols(g);
              cpt++;  // Il y aura forcement une tente sur ligne à cet emplacement
              changed = true;
            }
          }
        }
      }
      if (changed) {
        changed = false;
        compt_u++;
      }
    }
  }
  uint nb_empty = 0;
  uint nb_tents = 0;
  for (uint i = 0; i < game_nb_rows(g); i++) {
    if (game_get_square(g, i, j) == EMPTY) {
      nb_empty++;
    } else if (game_get_square(g, i, j) == TENT) {
      nb_tents++;
    }
  }
  // printf("cpt = %d\n", cpt);
  if (game_get_expected_nb_tents_col(g, j) == nb_tents + compt_u) {
    // Parcourt et place les herbes partout sauf dans les emplacement de tente_obli
    for (uint i = 0; i < game_nb_cols(g); i++) {
      if (game_get_square(g, i, j) == EMPTY) {
        changed = false;
        for (uint k = 0; k < cpt; k++) {
          if (tente_obligatoire[k] == j + game_nb_cols(g) * i) {
            // printf("tente obli\n");
            changed = true;
          }
        }
        if (!changed) {  // Si l'empty ne fait pas parti des tentes obligatoire alors elle devient de l'herbe
          game_set_square(g, i, j, GRASS);
          res++;
        }
      }
    }
  }
  return res;
}

bool solve_game(game g_ini, game g, int* empty_n, bool* last_n, uint n) {
  // printf("passage, n = %d,empty = %d\n", n, empty_n[n]);
  // game_print(g);
  if (empty_n[n] == -1) {  // Conditon d'arret
    // printf("fin\n");
    // game_print(g);
    if (game_is_over(g)) {
      // printf("fini\n");
      for (unsigned int i = 0; i < game_nb_rows(g); i++) {
        for (unsigned int j = 0; j < game_nb_cols(g); j++) {
          game_set_square(g_ini, i, j, game_get_square(g, i, j));
        }
      }  // printf("avant\n");
      game_delete(g);
      // printf("apres\n");
      return true;
    } else {
      // printf("ou pas\n");
      game_delete(g);
      return false;
    }
  }
  // game_print(g);
  uint i = empty_n[n] / game_nb_cols(g);
  uint j = empty_n[n] % game_nb_cols(g);
  uint check = game_check_move(g, i, j, TENT);
  // check ligne précédente ok
  if (n > 0) {
    if (last_n[n - 1]) {
      // game_print(g);
      uint i_pre = empty_n[n - 1] / game_nb_rows(g);
      // printf("last valide : n = %d, i = %d, j = %d, empty_n = %d\n", n, i, j,empty_n[n]);
      if (game_get_current_nb_tents_row(g, i_pre) != game_get_expected_nb_tents_row(g, i_pre)) {
        // printf("Suppr\n");
        game_delete(g);
        return false;
      }
    }
  }
  if (check == 1) {  // Coup régular -> on teste herbe et tente
    game gs = game_copy(g);
    game_set_square(g, i, j, TENT);
    if (solve_game(g_ini, g, empty_n, last_n, n + 1)) {
      game_delete(gs);
      return true;
    } else {
      game_set_square(gs, i, j, GRASS);
      return solve_game(g_ini, gs, empty_n, last_n, n + 1);
    }
  } else {
    game_set_square(g, i, j, GRASS);  // Coup losing -> on test uniquement l'herbe
    return solve_game(g_ini, g, empty_n, last_n, n + 1);
  }
}

uint solve_nb_sol(game g, int* empty_n, bool* last_n, uint n) {
  if (empty_n[n] == -1) {  // Conditon d'arret
    if (game_is_over(g)) {
      game_delete(g);
      return 1;
    } else {
      game_delete(g);
      return 0;
    }
  }
  uint i = empty_n[n] / game_nb_cols(g);
  uint j = empty_n[n] % game_nb_cols(g);
  uint check = game_check_move(g, i, j, TENT);
  // check ligne précédente ok
  if (n > 0) {
    if (last_n[n - 1]) {
      // game_print(g);
      uint i_pre = empty_n[n - 1] / game_nb_rows(g);
      // printf("last valide : n = %d, i = %d, j = %d, empty_n = %d\n", n, i, j,empty_n[n]);
      if (game_get_current_nb_tents_row(g, i_pre) != game_get_expected_nb_tents_row(g, i_pre)) {
        // printf("Suppr\n");
        game_delete(g);
        return 0;
      }
    }
  }
  if (check == 1) {  // Coup régular -> on teste herbe et tente
    game gs = game_copy(g);
    game_set_square(g, i, j, TENT);
    game_set_square(gs, i, j, GRASS);
    return solve_nb_sol(g, empty_n, last_n, n + 1) + solve_nb_sol(gs, empty_n, last_n, n + 1);
  } else {
    game_set_square(g, i, j, GRASS);  // Coup losing -> on test uniquement l'herbe
    return solve_nb_sol(g, empty_n, last_n, n + 1);
  }
}

void game_fill_tent_row(game g, uint i) {
  for (int j = 0; j < game_nb_cols(g); j++) {
    if (game_get_square(g, i, j) == EMPTY) {
      game_set_square(g, i, j, TENT);
    }
  }
}

void game_fill_tent_col(game g, uint j) {
  for (int i = 0; i < game_nb_rows(g); i++) {
    if (game_get_square(g, i, j) == EMPTY) {
      game_set_square(g, i, j, TENT);
    }
  }
}