#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"

void boot_magic_chess();
void start_game(board_s *board);
void load_game(board_s *board);
void reset_board(board_s *board);
int get_piece_id(const char *name);
int get_col_id(const char *name);
bool tokenise_player_choice(char *player_move, char *list_id);
void update_piece(board_s *board, int current_player, char *list_id);
bool is_cell_occupied_by_ally(board_s *board, char *list_id, int current_player);
void capture_enemy_piece(board_s *board, char *list_id, int current_player);
bool is_piece_selected_alive(board_s *board, char *list_id, int current_player);
// bool is_piece_movement_valid(board_s *board, char *list_id, int current_player);

#endif