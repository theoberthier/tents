// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include "model.h"

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"

/* **************************************************************** */
#ifdef __ANDROID__
  #define i_EMPTY "empty.png"
  #define i_GRASS "grass.png"
  #define i_TREE "tree.png"
  #define i_LOSE "tentlose.png"
  #define i_LOSE_GRASS "lose_grass.png"
  #define i_TENT "tent.png"
  #define i_right "droite.png"
  #define i_left "gauche.png"
  #define FONT "arial.ttf"
  #define FONTSIZE 36
#else
  #define i_EMPTY "./img/empty.png"
  #define i_GRASS "./img/grass.png"
  #define i_TREE "./img/tree.png"
  #define i_LOSE "./img/tentlose.png"
  #define i_LOSE_GRASS "./img/lose_grass.png"
  #define i_TENT "./img/tent.png"
  #define i_right "./img/droite.png"
  #define i_left "./img/gauche.png"
  #define FONT "./img/arial.ttf"
  #define FONTSIZE 36
#endif

/* **************************************************************** */
struct Env_t {
  /* PUT YOUR VARIABLES HERE */
  SDL_Texture *empty;
  SDL_Texture *tent;
  SDL_Texture *tree;
  SDL_Texture *grass;
  SDL_Texture *left;
  SDL_Texture *right;
  SDL_Texture *restart;
  SDL_Texture *undo;
  SDL_Texture *redo;
  SDL_Texture *solve;
  SDL_Texture *lose;
  SDL_Texture *losing_grass;
  SDL_Texture *win;
  int grid_x, grid_y;
  game g;
  square selected;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));
  if (argc > 1) {
    env->g = game_load((char *)argv[1]);
  } else {
    env->g = game_default();
  }
  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  // Initialisation du menu de selection des squares à TENT
  env->selected = TENT;
  /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */

  env->empty = IMG_LoadTexture(ren, i_EMPTY);
  if (!env->empty) ERROR("IMG_LoadTexture: %s\n", i_EMPTY);
  env->grass = IMG_LoadTexture(ren, i_GRASS);
  if (!env->grass) ERROR("IMG_LoadTexture: %s\n", i_GRASS);
  env->tree = IMG_LoadTexture(ren, i_TREE);
  if (!env->tree) ERROR("IMG_LoadTexture: %s\n", i_TREE);
  env->tent = IMG_LoadTexture(ren, i_TENT);
  if (!env->tent) ERROR("IMG_LoadTexture: %s\n", i_TENT);
  env->left = IMG_LoadTexture(ren, i_left);
  if (!env->left) ERROR("IMG_LoadTexture: %s\n", i_left);
  env->right = IMG_LoadTexture(ren, i_right);
  if (!env->right) ERROR("IMG_LoadTexture: %s\n", i_right);
  env->lose = IMG_LoadTexture(ren, i_LOSE);
  if (!env->lose) ERROR("IMG_LoadTexture: %s\n", i_LOSE);
  env->losing_grass = IMG_LoadTexture(ren, i_LOSE_GRASS);
  if (!env->lose) ERROR("IMG_LoadTexture: %s\n", i_LOSE_GRASS);
  /* ini text for restart and solve */
  SDL_Color color = {0, 0, 255, 255}; /* blue color in RGBA */
  TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE);
  if (!font) ERROR("Erreur de type TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font, TTF_STYLE_BOLD);                                // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL
  SDL_Surface *surf_r = TTF_RenderText_Blended(font, "Restart", color);  // blended rendering for ultra nice text
  SDL_Surface *surf_s = TTF_RenderText_Blended(font, "Solve", color);
  SDL_Surface *surf_u = TTF_RenderText_Blended(font, "Undo", color);
  SDL_Surface *surf_redo = TTF_RenderText_Blended(font, "Redo", color);
  SDL_Surface *surf_win = TTF_RenderText_Blended(font, "You WIN !", color);

  env->restart = SDL_CreateTextureFromSurface(ren, surf_r);
  env->solve = SDL_CreateTextureFromSurface(ren, surf_s);
  env->undo = SDL_CreateTextureFromSurface(ren, surf_u);
  env->redo = SDL_CreateTextureFromSurface(ren, surf_redo);
  env->win = SDL_CreateTextureFromSurface(ren, surf_win);

  SDL_FreeSurface(surf_r);
  SDL_FreeSurface(surf_s);
  SDL_FreeSurface(surf_u);
  SDL_FreeSurface(surf_win);

  TTF_CloseFont(font);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) { /* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */

  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(ren, 0, h / 10, w, h / 10);  // Ligne qui sépare le menu de la grille

  /* *********** Menu *********** */
  /* Square selectionné */
  SDL_Rect Square_menu;
  Square_menu.x = w / 20;  // Départ x
  Square_menu.y = 0;       // Départ y
  Square_menu.w = w / 10;  // Largeur
  Square_menu.h = h / 10;  // Hauteur
  if (env->selected == EMPTY) {
    SDL_RenderCopy(ren, env->empty, NULL, &Square_menu);
  } else if (env->selected == GRASS) {
    SDL_RenderCopy(ren, env->grass, NULL, &Square_menu);
  } else if (env->selected == TREE) {
    SDL_RenderCopy(ren, env->tree, NULL, &Square_menu);
  } else if (env->selected == TENT) {
    SDL_RenderCopy(ren, env->tent, NULL, &Square_menu);
  }

  /* Flèche pour changer le square selectionné */
  // Fleche gauche
  SDL_Rect Left;
  Left.x = 0;       // Départ x
  Left.y = 0;       // Départ y
  Left.w = w / 20;  // Largeur
  Left.h = h / 10;  // Hauteur
  SDL_RenderCopy(ren, env->left, NULL, &Left);
  // Fleche droite
  SDL_Rect Right;
  Right.x = 3 * w / 20;  // Départ x
  Right.y = 0;           // Départ y
  Right.w = w / 20;      // Largeur
  Right.h = h / 10;      // Hauteur
  SDL_RenderCopy(ren, env->right, NULL, &Right);

  /* Undo */
  SDL_Rect Undo;
  Undo.x = 2 * w / 5;
  Undo.y = 0;
  Undo.w = w / 10;
  Undo.h = h / 10;
  SDL_RenderCopy(ren, env->undo, NULL, &Undo);

  /* Redo */
  SDL_Rect Redo;
  Redo.x = 5.5 * w / 10;
  Redo.y = 0;
  Redo.w = w / 10;
  Redo.h = h / 10;
  SDL_RenderCopy(ren, env->redo, NULL, &Redo);

  /* Restart */
  SDL_Rect Restart;
  Restart.x = 7 * w / 10;
  Restart.y = 0;
  Restart.w = 2 * w / 15;
  Restart.h = h / 10;
  SDL_RenderCopy(ren, env->restart, NULL, &Restart);

  /* Solve */
  SDL_Rect Solve;
  Solve.x = 8.8 * w / 10;
  Solve.y = 0;
  Solve.w = w / 10;
  Solve.h = h / 10;
  SDL_RenderCopy(ren, env->solve, NULL, &Solve);

  /* grille */
  SDL_Color color = {0, 0, 255, 255};
  TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE);
  if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font, TTF_STYLE_BOLD);
  char temps[2];
  SDL_Texture *rowTexture;
  SDL_Texture *colTexture;
  SDL_Rect Rect_row;
  SDL_Rect Rect_col;

  SDL_Rect pos;

  pos.w = w / (game_nb_cols(env->g) + 1);
  pos.h = (9 * h / 10) / (game_nb_rows(env->g) + 1);
  pos.x = pos.w;
  pos.y = h / 10 + pos.h;
  env->grid_x = pos.w;
  env->grid_y = pos.h;

  Rect_row.h = pos.h;
  Rect_row.w = pos.w;
  Rect_row.x = 0;
  Rect_row.y = pos.y;

  Rect_col.h = pos.h;
  Rect_col.w = pos.w;
  Rect_col.y = h / 10;
  Rect_col.x = pos.x;

  SDL_Rect Rect_win;
  Rect_win.w = 4 * w / 9;
  Rect_win.h = h / 10;
  Rect_win.x = w / 2 - w / 4;
  Rect_win.y = h / 2;
  for (int i = 0; i < game_nb_rows(env->g); i++) {
    if (i != 0) {
      pos.y += pos.h;
    }

    pos.x = pos.w;

    for (int j = 0; j < game_nb_cols(env->g); j++) {
      if (j != 0) {
        pos.x += pos.w;
      }

      if (game_get_square(env->g, i, j) == TREE) {
        SDL_RenderCopy(ren, env->tree, NULL, &pos);
      }
      if (game_get_square(env->g, i, j) == TENT) {
        game losing_control = game_copy(env->g);
        game_set_square(losing_control, i, j, EMPTY);
        if (game_check_move(losing_control, i, j, TENT) == 2) {
          SDL_RenderCopy(ren, env->lose, NULL, &pos);
        } else {
          SDL_RenderCopy(ren, env->tent, NULL, &pos);
        }
        game_delete(losing_control);
      }
      if (game_get_square(env->g, i, j) == GRASS) {
        game losing_control = game_copy(env->g);
        game_set_square(losing_control, i, j, EMPTY);
        if (game_check_move(losing_control, i, j, GRASS) == 2) {
          SDL_RenderCopy(ren, env->losing_grass, NULL, &pos);
        } else {
          SDL_RenderCopy(ren, env->grass, NULL, &pos);
        }
        game_delete(losing_control);
      }
      if (game_get_square(env->g, i, j) == EMPTY) {
        SDL_RenderCopy(ren, env->empty, NULL, &pos);
      }
    }
  }
  for (int j = 0; j < game_nb_rows(env->g); j++) {
    sprintf(temps, "%d", game_get_expected_nb_tents_row(env->g, j));
    SDL_Surface *surf3 = TTF_RenderText_Blended(font, temps, color);
    rowTexture = SDL_CreateTextureFromSurface(ren, surf3);
    SDL_RenderCopy(ren, rowTexture, NULL, &Rect_row);
    SDL_FreeSurface(surf3);
    Rect_row.y += pos.h;
  }
  for (int i = 0; i < game_nb_cols(env->g); i++) {
    sprintf(temps, "%d", game_get_expected_nb_tents_col(env->g, i));
    SDL_Surface *surf2 = TTF_RenderText_Blended(font, temps, color);
    colTexture = SDL_CreateTextureFromSurface(ren, surf2);
    SDL_RenderCopy(ren, colTexture, NULL, &Rect_col);
    SDL_FreeSurface(surf2);
    Rect_col.x += pos.w;
  }
  if (game_is_over(env->g)) {
    SDL_RenderCopy(ren, env->win, NULL, &Rect_win);
  }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  int pos_x = -1;
  int pos_y = -1;
  if (e->type == SDL_QUIT) {
    return true;
  }
/*android*/
#ifdef __ANDROID__
  else if (e->type == SDL_FINGERDOWN) {
    pos_x = e->tfinger.x * w; /* tfinger.x, normalized in [0..1] */
    pos_y = e->tfinger.y * h; /* tfinger.y, normalized in [0..1] */
  }
/* other events */
#else
  else if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    pos_x = mouse.x;
    pos_y = mouse.y;
  }
#endif
  if ((pos_x == -1) || (pos_y == -1)) return false;
  if (pos_y < h / 10) {
    // On est dans le menu
    if (pos_x < w / 20) {  // Fleche gauche
      // On passe au selected précédent
      if (env->selected == EMPTY) {
        env->selected = GRASS;
      } else {
        env->selected--;
        if (env->selected == TREE) {  // On skip les arbres, on ne veut pas que l'utilisateur puisse placer des arbres
          env->selected--;
        }
      }
    } else if (pos_x < 3 * w / 20) {  // Select, on ne fait rien
      return false;
    } else if (pos_x < w / 5) {  // Fleche droite
      // On passe au selected suivant
      if (env->selected == GRASS) {
        env->selected = EMPTY;
      } else {
        env->selected++;
        if (env->selected == TREE) {  // On skip les arbres
          env->selected++;
        }
      }
    } else if (pos_x < 2 * w / 5) {
      return false;  // On ne fait rien
    } else if (pos_x < w / 2) {
      game_undo(env->g);
    } else if (pos_x < 5.5 * w / 10) {
      return false;
    } else if (pos_x < 6.5 * w / 10) {
      game_redo(env->g);
    } else if (pos_x < 7 * w / 10) {
      return false;
    } else if (pos_x < 5 * w / 6) {
      game_restart(env->g);
    } else if (pos_x < 8.8 * w / 10) {
      return false;
    } else if (pos_x < 9.8 * w / 10) {
      game_solve(env->g);
    } else {
      return false;
    }

  } else {
    if (!game_is_over(env->g)) {
      int val_row = (pos_y - h / 10 - (9 * h / 10) / (game_nb_rows(env->g) + 1)) * (game_nb_rows(env->g) + 1) / (9 * h / 10);
      int val_col = (pos_x - w / (game_nb_cols(env->g) + 1)) * (game_nb_cols(env->g) + 1) / w;
      // On est dans la grille du jeu
      // printf("val_row = %d,val_col = %d\n", val_row, val_col);
      if (val_row >= 0 && val_col >= 0 && val_row <= game_nb_rows(env->g) && val_col <= game_nb_cols(env->g)) {
        if (game_get_square(env->g, val_row, val_col) != env->selected) {
          game_play_move(env->g, val_row, val_col, env->selected);
        }
      }
    }
  }

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  SDL_DestroyTexture(env->empty);
  SDL_DestroyTexture(env->tent);
  SDL_DestroyTexture(env->tree);
  SDL_DestroyTexture(env->grass);
  SDL_DestroyTexture(env->left);
  SDL_DestroyTexture(env->right);
  SDL_DestroyTexture(env->solve);
  SDL_DestroyTexture(env->restart);
  SDL_DestroyTexture(env->undo);
  SDL_DestroyTexture(env->redo);
  SDL_DestroyTexture(env->lose);
  SDL_DestroyTexture(env->losing_grass);
  SDL_DestroyTexture(env->win);
  game_delete(env->g);
  free(env);
}

/* **************************************************************** */
