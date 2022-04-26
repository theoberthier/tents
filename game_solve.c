#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#include "toolbox.h"

int main(int argc, char* argv[]) {
  game g = game_load(argv[2]);
  // game g = game_default();
  game_print(g);
  if (strcmp(argv[1], "-s") == 0) {
    clock_t begin = clock();
    if (game_solve(g)) {
      printf("Solution trouvée !\n");
      game_print(g);
      game_save(g, argv[3]);
      clock_t end = clock();
      double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      printf("Temps d'execution : %f\n", time_spent);
      game_delete(g);
      return EXIT_SUCCESS;
    } else {
      printf("Aucune solution trouvée !\n");
      game_delete(g);
      return EXIT_FAILURE;
    }
  } else if (strcmp(argv[1], "-c") == 0) {
    clock_t begin = clock();
    FILE* f = fopen(argv[3], "w");
    uint nb_sol = game_nb_solutions(g);
    fprintf(f, "%d", nb_sol);
    fclose(f);
    clock_t end = clock();
    game_delete(g);
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Temps d'execution : %f\n", time_spent);
    return EXIT_SUCCESS;
  }
  game_delete(g);
  return EXIT_FAILURE;
}