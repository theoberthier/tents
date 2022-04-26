/**
 * @file toolbox.h
 * @brief Toolbox with functions for developpers
 * @details See @ref index for further details.
 * @copyright University of Bordeaux. All rights reserved, 2020.
 **/

#ifndef __TOOLBOX_H__
#define __TOOLBOX_H__
#include <stdbool.h>
#include "game.h"



/**
 * @brief Tests if a square s is orthogonally adjacent to the position gave
 * @param g the game
 * @param i row index
 * @param j column index
 * @param s the square value
 * @pre @p g must be a valid pointer toward a game structure
 * @pre @p i < game width
 * @pre @p j < game height
 * @pre @p s must be either EMPTY, GRASS, TENT or TREE
 * @return true if a square s is orthogonally adjacent
 **/
bool game_check_adj_ort_square(cgame g, uint i, uint j, square s);
/**
 * @brief Tests if a square s is diagonally adjacent to the position gave
 * @param g the game
 * @param i row index
 * @param j column index
 * @param s the square value
 * @pre @p g must be a valid pointer toward a game structure
 * @pre @p i < game width
 * @pre @p j < game height
 * @pre @p s must be either EMPTY, GRASS, TENT or TREE
 * @return true if a square s is diagonally adjacent
 **/
bool game_check_adj_diag_square(cgame g, uint i, uint j, square s);
/**
 * @brief Tests if there is a placeable tent if we place a GRASS
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure
 * @pre @p i < game width
 * @pre @p j < game height
 * @return true if there is a placeable tent if we place a GRASS, false otherwise
 **/
bool game_check_placeable_tent(cgame g, uint i, uint j);
/**
 * @brief Update the historic that correspond to the played move
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure
 * @pre @p i < game width
 * @pre @p j < game height
 **/
void hist_play(game g, uint i, uint j);
/**
 * @brief Calculate a solution if it exist and modify g_ini to match it
 * @param g_ini the initial game
 * @param g a game to calculate a solution
 * @pre @p g_ini must be a valid pointer toward a game structure
 * @pre @p g must be a valid pointer toward a game structure
 **/
bool solve_game(game g_ini, game g, int * empty_n, bool * last_n, uint n);

uint solve_nb_sol(game g, int * empty_n, bool * last_n, uint n);

void game_fill_tent_row(game g, uint i);

void game_fill_tent_col(game g, uint i);

int pos_placeable_tent(cgame g, uint i, uint j);

void del_empty(int * empty, uint i, uint size);

uint only_row(game g, uint i);

uint only_col(game g, uint j);
#endif //__TOOLBOX_H__