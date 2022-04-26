Dorian Amy:

    game game_new(square *squares, uint *nb_tents_row, uint *nb_tents_col);//Done
    game game_new_empty(void);//Done
    game game_copy(cgame g);//Done
    bool game_equal(cgame g1, cgame g2);//Done
    int game_check_move(cgame g, uint i, uint j, square s);//Done
    square game_get_square(cgame g, uint i, uint j);//Done
    void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents);//Done
    void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents);//Done
    game game_default_solution(void);//Done

Youssouf Ibrahim Abakar:


    uint game_get_expected_nb_tents_row(cgame g, uint i);
    uint game_get_expected_nb_tents_col(cgame g, uint j);
    uint game_get_expected_nb_tents_all(cgame g);
    uint game_get_current_nb_tents_row(cgame g, uint i);
    uint game_get_current_nb_tents_col(cgame g, uint j);
    uint game_get_current_nb_tents_all(cgame g);
    game game_default(void);

Berthier Théo:

    void game_play_move(game g, uint i, uint j, square s);fait
    int game_check_move(cgame g, uint i, uint j, square s);
    bool game_is_over(cgame g);fait
    void game_fill_grass_row(game g, uint i);fait
    void game_fill_grass_col(game g, uint j);fait
    void game_set_square(game g, uint i, uint j, square s);fait
    void game_restart(game g);fait
    void game_delete(game g);fait ? 
    void game_print(cgame g);fait

