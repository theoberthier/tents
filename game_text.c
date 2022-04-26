#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char const *argv[]) {
  game new;
  if (argc > 1) {
    new = game_load((char *)argv[1]);
  } else {
    new = game_default();
  }
  game_print(new);
  printf("? [press h for help]\n");
  while (game_is_over(new) != true) {
    char c;
    int p2;
    int p3;
    char save_loc[100];
    // On va définir que la sauvegarde aura 100 charactères max
    char l = scanf("%c", &c);
    assert(l);
    if (c == 's') {
      char le = scanf("%s", save_loc);
      assert(le);
      game_save(new, save_loc);
      game_print(new);
      printf("? [press h for help]\n");
    }
    if (c == 'h') {
      game_print(new);
      printf(
          "HELP:\n-h: for help\n-r: for restart a "
          "game\ne: for enter Empty "
          "into the square\nt: for Enter Tree into the "
          "square\ng: for Enter "
          "Grass ine the square\nq: for quit a game\n");
      char c = scanf("%c %i %i", &c, &p2, &p3);
      assert(c);
    }

    else if (c == 'r') {
      game_restart(new);
      game_print(new);
      printf("? [press h for help]\n");
    } else if (c == 'q') {
      printf("what a shame, you gave up :-( !\n");
      break;
    } else if (c == 'z') {
      game_undo(new);
      game_print(new);
      printf("? [press h for help]\n");
    } else if (c == 'y') {
      game_redo(new);
      game_print(new);
      printf("? [press h for help]\n");
    }
    char d = scanf("%i %i", &p2, &p3);
    assert(d);
    if ((c == 'e' || c == 't' || c == 'g') && (p2 > -1 && p3 > -1)) {
      if (c == 'e' && game_check_move(new, p2, p3, EMPTY) != ILLEGAL) {
        printf(
            ">action: play move %c into square "
            "(%i,%i)\n",
            c, p2, p3);
        game_play_move(new, p2, p3, EMPTY);
        game_print(new);
        printf("? [press h for help]\n");
      }
      if (c == 't' && game_check_move(new, p2, p3, TENT) != ILLEGAL) {
        printf(
            ">action: play move %c into square "
            "(%i,%i)\n",
            c, p2, p3);
        if (game_check_move(new, p2, p3, TENT) == LOSING) {
          printf(
              ">action: warning losing "
              "play move (%i,%i)\n",
              p2, p3);
        }
        game_play_move(new, p2, p3, TENT);
        game_print(new);
        printf("? [press h for help]\n");
      }
      if (c == 'g' && game_check_move(new, p2, p3, GRASS) != ILLEGAL) {
        printf(
            ">action: play move %c into square "
            "(%i,%i)\n",
            c, p2, p3);
        game_play_move(new, p2, p3, GRASS);
        game_print(new);
        printf("? [press h for help]\n");
      }
      if (game_check_move(new, p2, p3, EMPTY) == ILLEGAL || game_check_move(new, p2, p3, TENT) == ILLEGAL || game_check_move(new, p2, p3, GRASS) == ILLEGAL) {
        printf(
            ">action: play move %c into square "
            "(%i,%i)\n",
            c, p2, p3);
        printf(
            ">warning: forbidden play move "
            "(%i,%i)\n",
            p2, p3);
        game_print(new);
        printf("? [press h for help]\n");
      }
    }
  }
  if (game_is_over(new)) {
    printf("Congratulation you Win!\n");
  }
  game_delete(new);
  return 0;
}