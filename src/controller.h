#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdio.h>
#include "model.h"

/*
    Main loop
*/
void boot_magic_chess();
void start_game(board_s *board, FILE *file, int current_player);
void reset_board(board_s *board);
/*
*/

/*
    Based on user inputs
*/
int get_piece_id(const char *name);
int get_col_id(const char *name);
int tokenise_player_choice(char *player_move, char *list_id);
void update_piece(board_s *board, int current_player, char *list_id);
/*
*/

/*
    Saving and loading a game
*/
void save_game(board_s *board, FILE *file, int current_player);
void load_game(board_s *board, FILE *file, int current_player);
void set_board_from_save(board_s *board, char *buffer, int *current_player);
/**/


/*
    All the checks to make sure the move is legal
*/
bool is_cell_occupied_by_ally(board_s *board, char *list_id, int current_player);
bool is_piece_selected_alive(board_s *board, char *list_id, int current_player);
bool piece_movement_validity(board_s *board, char *list_id, int current_player);
bool is_pawn_move_legal(board_s *board, char *list_id, int current_player);
bool is_rook_move_legal(board_s *board, char *list_id, int current_player);
bool is_knight_move_legal(board_s *board, char *list_id, int current_player);
bool is_bishop_move_legal(board_s *board, char *list_id, int current_player);
// bool is_queen_move_legal(board_s *board, char *list_id, int current_player);
// bool is_king_move_legal(board_s *board, char *list_id, int current_player);
bool is_row_blocked(board_s *board, char *list_id, int current_player);
bool is_col_blocked(board_s *board, char *list_id, int current_player);
bool is_diagonal_blocked(board_s *board, char *list_id, int current_player);
bool is_cell_occupied_by_enemy(board_s *board, char *list_id, int current_player);
/*
*/

/*
    Other fonctions
*/
void capture_enemy_piece(board_s *board, char *list_id, int current_player);
/**/

#endif