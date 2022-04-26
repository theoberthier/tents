cmake_minimum_required(VERSION 3.0.2)
project(game_text C)
enable_testing()
include(CTest)
set(CMAKE_C_FLAGS "-Wall -std=c99")            # options de compilation génériques
set(CMAKE_C_FLAGS_DEBUG "-g -O0 --coverage")   # options de compilation en mode Debug
set(CMAKE_C_FLAGS_RELEASE "-O3")               # options de compilation en mode Release

find_package(SDL2 REQUIRED)
set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS})
set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} -lSDL2_ttf -lSDL2_image) # add extra SDL2 libraries

message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

add_executable(game_text  game_text.c)
add_executable(game_test_damy game_test_damy.c)
add_executable(game_test_tberthier game_test_tberthier.c)
add_executable(game_test_yibrahimabak game_test_yibrahimabak.c)
add_executable(game_solve game_solve.c)
add_executable(game_sdl model.c game_sdl.c)

include_directories(${SDL2_ALL_INC})
add_library(game game.c game_aux.c game_tools.c)
target_link_libraries(game_text game)
target_link_libraries(game_test_damy game)
target_link_libraries(game_test_tberthier game)
target_link_libraries(game_test_yibrahimabak game)
target_link_libraries(game_solve game)
target_link_libraries(game_sdl  ${SDL2_ALL_LIBS} game)

# file copy save
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/save DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# folder copy
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/img/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/img/)

add_test(test_damy_dummy ./game_test_damy dummy)
add_test(test_tberthier_dummy ./game_test_tberthier dummy)

add_test(test_damy_get_square ./game_test_damy get_square)
add_test(test_damy_set_square ./game_test_damy set_square)
add_test(test_damy_game_equal ./game_test_damy equal)
add_test(test_damy_set_tents_row ./game_test_damy set_tents_row)
add_test(test_damy_set_tents_col ./game_test_damy set_tents_col)
add_test(test_damy_new_empty ./game_test_damy new_empty)
add_test(test_damy_new ./game_test_damy new)
add_test(test_damy_game_copy ./game_test_damy copy)
add_test(test_damy_game_current_tents ./game_test_damy current_tents)
add_test(test_damy_game_expected_tents ./game_test_damy expected_tents)
add_test(test_damy_game_current_col ./game_test_damy current_col)
add_test(test_damy_game_check_move ./game_test_damy check_move)
add_test(test_damy_game_is_over ./game_test_damy is_over)
add_test(test_damy_undo ./game_test_damy undo)
add_test(test_damy_checkv2 ./game_test_damy checkv2)
add_test(test_damy_restartv2 ./game_test_damy restartv2)
add_test(test_damy_fill_grassv2 ./game_test_damy fill_grassv2)
add_test(test_damy_fill_equalv2 ./game_test_damy equalv2)
add_test(test_damy_game_load ./game_test_damy game_load)

#add_test(test_yibrahimabak_dummy ./game_test_yibrahimabak dummy)
#add_test(test_yibrahimabak_game_set_expected_nb_tents_row ./game_test_yibrahimabak game_set_expected_nb_tents_row)
#add_test(test_yibrahimabak_game_get_expected_nb_tents_col ./game_test_yibrahimabak game_get_expected_nb_tents_col)
#add_test(test_yibrahimabak_game_get_expected_nb_tents_row ./game_test_yibrahimabak game_get_expected_nb_tents_row)
#add_test(test_yibrahimabak_game_get_expected_nb_tents_all ./game_test_yibrahimabak game_get_expected_nb_tents_all)
#add_test(test_yibrahimabak_game_get_current_nb_tents_col ./game_test_yibrahimabak game_get_current_nb_tents_col)
#add_test(test_yibrahimabak_game_get_current_nb_tents_row ./game_test_yibrahimabak game_get_current_nb_tents_row)
#add_test(test_yibrahimabak_game_get_current_nb_tents_all ./game_test_yibrahimabak game_get_current_nb_tents_all)



add_test(test_tberthier_check_move ./game_test_tberthier check_move)
add_test(test_tberthier_game_is_over ./game_test_tberthier game_is_over)
add_test(test_tberthier_game_restart ./game_test_tberthier game_restart)
#add_test(test_tberthier_game_delete ./game_test_tberthier game_delete)#
add_test(test_tberthier_game_fill_grass_row ./game_test_tberthier game_fill_grass_row)
add_test(test_tberthier_game_fill_grass_col ./game_test_tberthier game_fill_grass_col)
add_test(test_tberthier_game_play_move ./game_test_tberthier game_play_move)
add_test(test_tberthier_game_save ./game_test_tberthier game_save)
