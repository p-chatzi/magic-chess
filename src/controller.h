#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"

void boot_magic_chess();
void start_game(board_s *board);
void load_game(board_s *board);
void reset_board(board_s *board);
int get_piece_id(const char *name);
int get_col_id(const char *name);
int player_moves_piece(board_s *board, int current_player, char player_move[20]);

#endif