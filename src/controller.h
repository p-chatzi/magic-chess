#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdio.h>
#include "model.h"

/*
    Main loop
*/
void boot_magic_chess();
void start_game(board_s* board, FILE* file, int current_player);
void reset_board(board_s* board);

/*
    Based on user inputs
*/
int get_piece_id(const char* name);
int get_col_id(const char* name);
int tokenise_player_choice(char* player_move, int* list_id);
void update_piece(board_s* board, int current_player, int* list_id);

/*
    Saving and loading a game
*/
void save_game(board_s* board, FILE* file, int current_player);
void load_game(board_s* board, FILE* file, int current_player);
void set_board_from_save(board_s* board, char* buffer, int* current_player);

/*
    All the checks to make sure the move is legal
*/
bool is_piece_selected_alive(board_s* board, int* list_id, int current_player);
bool is_row_blocked(board_s* board, int* list_id, int current_player);
bool is_col_blocked(board_s* board, int* list_id, int current_player);
bool is_diagonal_blocked(board_s* board, int* list_id, int current_player);
bool is_cell_occupied_by_ally(board_s* board, int* list_id, int current_player);
bool is_cell_occupied_by_enemy(board_s* board, int* list_id, int current_player);
bool is_destination_current_position(board_s* board, int* list_id, int current_player);
void move_count(board_s* board, int* list_id, int current_player);
bool will_king_be_checked(board_s* board, int* list_id, int current_player);
bool is_king_checked(board_s* board, int* list_id, int current_player);
bool is_checkmate(board_s* board, int* list_id, int current_player);

bool piece_movement_validity(board_s* board, int* list_id, int current_player);
// Is pawn move legal
bool is_pawn_move_legal(board_s* board, int* list_id, int current_player);
bool can_pawn_advance(board_s* board, int* list_id, int current_player);
bool can_pawn_eat(board_s* board, int* list_id, int current_player);
bool has_pawn_moved(board_s* board, int* list_id, int current_player);
bool is_en_passant_legal(board_s* board, int* list_id, int current_player);
// Is rook move legal
bool is_rook_move_legal(board_s* board, int* list_id, int current_player);
bool is_rook_pattern_valid(board_s* board, int* list_id, int current_player);
// Is Bishop move legal
bool is_bishop_move_legal(board_s* board, int* list_id, int current_player);
bool is_bishop_pattern_valid(board_s* board, int* list_id, int current_player);
// Is Knight move legal
bool is_knight_move_legal(board_s* board, int* list_id, int current_player);
bool is_knight_pattern_valid(board_s* board, int* list_id, int current_player);
// Is Queen move legal
bool is_queen_move_legal(board_s* board, int* list_id, int current_player);
bool is_queen_pattern_valid(board_s* board, int* list_id, int current_player);
// Is King move legal
bool is_king_move_legal(board_s* board, int* list_id, int current_player);
bool is_king_pattern_valid(board_s* board, int* list_id, int current_player);

/*
    Other fonctions
*/
void capture_enemy_piece(board_s* board, int* list_id, int current_player);

#endif
