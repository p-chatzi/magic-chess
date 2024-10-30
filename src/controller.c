#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "controller.h"
#include "view.h"
#include "model.h"

bool is_col_blocked(board_s *board, char *list_id, int current_player);
bool is_row_blocked(board_s *board, char *list_id, int current_player);
bool is_pawn_move_legal(board_s *board, char *list_id, int current_player);
bool is_pawn_able_to_capture(board_s *board, char *list_id, int current_player);
bool is_rook_move_legal(board_s *board, char *list_id, int current_player);
bool is_knight_move_legal(board_s *board, char *list_id, int current_player);
bool is_bishop_move_legal(board_s *board, char *list_id, int current_player);
bool is_queen_move_legal(board_s *board, char *list_id, int current_player);
bool is_king_move_legal(board_s *board, char *list_id, int current_player);

void boot_magic_chess()
{
    board_s board;
    while (true)
    {
        print_main_menu();

        switch (choose_main_menu())
        {
        case START:
            start_game(&board);
            continue;

        case LOAD:
            load_game(&board);
            continue;

        case RULES:
            print_rules();
            continue;

        case SETTINGS:
            print_settings();
            continue;

        case QUIT:
            print_byecat();
            return;

        default:
            printf("\nInvalid input");
            continue;
        }
    }
}

void start_game(board_s *board)
{
    reset_board(board);
    print_board(board);

    int current_player = WHITE, turn_count = 1;
    bool is_game_finished = false;
    while (1)
    {
        char player_move[20], list_id[3];
        get_player_choice(player_move);
        is_game_finished = tokenise_player_choice(player_move, list_id);
        if (is_game_finished)
            break;

        if (!is_piece_movement_valid(list_id, board, current_player))
            continue;

        if (!is_piece_selected_alive(board, list_id, current_player))
        {
            printf("\nThis piece has been captured, choose one still alive");
            continue;
        }

        if (is_cell_occupied_by_ally(board, list_id, current_player))
        {
            printf("\nCannot move on top of your own piece");
            continue;
        }
        capture_enemy_piece(board, list_id, current_player);

        update_piece(board, current_player, list_id);
        print_board(board);
        if (current_player == WHITE)
            current_player = BLACK;
        else
            current_player = WHITE;
        turn_count++;
    }
}

bool is_piece_movement_valid(char *list_id, board_s *board, int current_player)
{
    for (int pid = PAWN0; pid < ROOK8; pid++)
    {
        if (list_id[0] == pid)
            return is_pawn_move_legal(board, list_id, current_player);
    }
    if (list_id[0] == ROOK8 || list_id[0] == ROOK9)
        return is_rook_move_legal(board, list_id, current_player);
    // if (list_id[0] == KNIGHT10 || list_id[0] == KNIGHT11)
    // return is_knight_move_legal(board, list_id, current_player);
    // if (list_id[0] == BISHOP12 || list_id[0] == BISHOP13)
    // return is_bishop_move_legal(board, list_id, current_player);
    // if (list_id[0] == QUEEN)
    // return is_queen_move_legal(board, list_id, current_player);
    // if (list_id[0] == KING)
    // return is_king_move_legal(board, list_id, current_player);
    return false;
}

// bool is_pawn_move_legal(board_s *board, char *list_id, int current_player)
// {
// if (list_id[1] == 1 && current_player == WHITE)
// {
// if ((list_id[1] == board->player[WHITE][(int)list_id[0]].pos.x + 1) ||
//  (list_id[1] == board->player[WHITE][(int)list_id[0]].pos.x + 2) &&
// !is_cell_occupied_by_enemy(board, list_id) && !is_col_blocked(board, list_id, current_player))
// return true;
// }
//
// if (list_id[1] == NUM_ROW - 1 && current_player == BLACK)
// {
// if ((list_id[1] == (board->player[BLACK][(int)list_id[0]].pos.x) - 1 ||
//  list_id[1] == (board->player[BLACK][(int)list_id[0]].pos.x) - 2) &&
// !is_cell_occupied_by_enemy(board, list_id) && !is_col_blocked(board, list_id, current_player))
// return true;
// }
//
// if (current_player == WHITE)
// {
// if (list_id[1] == (board->player[WHITE][(int)list_id[0]].pos.x) + 1 &&
// !is_cell_occupied_by_enemy(board, list_id) && !is_col_blocked(board, list_id, current_player))
// return true;
// }
//
// if (current_player == BLACK)
// {
// if (list_id[1] == (board->player[BLACK][(int)list_id[0]].pos.x) - 1 &&
// !is_cell_occupied_by_enemy(board, list_id) && !is_col_blocked(board, list_id, current_player))
// return true;
// }
// return is_pawn_able_to_capture(board, list_id, current_player);
// }

bool is_pawn_move_legal(board_s *board, char *list_id, int current_player)
{
    int pawn_id = (int)list_id[0];
    int target_row = list_id[1];
    int current_row = board->player[current_player][pawn_id].pos.x;
    int current_col = board->player[current_player][pawn_id].pos.y;

    int direction = (current_player == WHITE) ? 1 : -1;

    // Check if it's the pawn's first move
    bool is_first_move = (current_player == WHITE && current_row == 1) ||
                         (current_player == BLACK && current_row == 6);

    // Check for one square move
    if (target_row == current_row + direction)
    {
        return !is_cell_occupied_by_ally(board, list_id, current_player);
    }

    // Check for two square move (only on first move)
    if (is_first_move && target_row == current_row + (2 * direction))
    {
        int intermediate_row = current_row + direction;
        return !is_cell_occupied_by_ally(board, list_id, current_player) &&
               !is_cell_occupied_by_ally(board, list_id, current_player);
    }

    return false;
}

bool is_pawn_able_to_capture(board_s *board, char *list_id, int current_player)
{
    for (int pid = PAWN0; pid < NUM_PIECES - 1; pid++)
    {
        if (current_player == WHITE)
        {
            if (board->player[BLACK][pid].pos.x == list_id[1] + 1 &&
                (board->player[BLACK][pid].pos.y == list_id[2] + 1 ||
                 board->player[BLACK][pid].pos.y == list_id[2] - 1))
                return true;
        }
        if (current_player == BLACK)
        {
            if (board->player[WHITE][pid].pos.x == list_id[1] - 1 &&
                (board->player[WHITE][pid].pos.y == list_id[2] + 1 ||
                 board->player[WHITE][pid].pos.y == list_id[2] - 1))
                return true;
        }
    }
    return false;
}

bool is_rook_move_legal(board_s *board, char *list_id, int current_player)
{
    if (list_id[1] == board->player[current_player]->pos.x)
    {
        if (!is_col_blocked(board, list_id, current_player))
        {
            if (!is_cell_occupied_by_ally(board, list_id, current_player))
                return true;
        }
    }
    if (list_id[2] == board->player[current_player]->pos.y)
    {
        if (!is_row_blocked(board, list_id, current_player))
        {
            if (!is_cell_occupied_by_ally(board, list_id, current_player))
                return true;
        }
    }
    return false;
}

bool is_cell_occupied_by_enemy(board_s *board, char *list_id)
{
    for (int pid = PAWN0; pid < NUM_PIECES; pid++)
    {
        if (list_id[1] == board->player[WHITE][pid].pos.x &&
            list_id[2] == board->player[WHITE][pid].pos.y)
            return true;
    }
    return false;
}

bool is_row_blocked(board_s *board, char *list_id, int current_player)
{
    int direction;
    if (board->player[current_player][(int)list_id[0]].pos.x - list_id[1] > 0)
        direction = 1;
    else
        direction = 0;
    for (int pid = PAWN0; pid < NUM_PIECES; pid++)
    {
        int num_cell_path = abs(board->player[current_player][(int)list_id[0]].pos.x - list_id[1]);
        for (int row_movement = 0; row_movement < num_cell_path; row_movement++)
        {
            if (direction == 0)
            {
                if (board->player[WHITE][pid].pos.x == board->player[current_player][(int)list_id[0]].pos.x + row_movement)
                    return true;
                if (board->player[BLACK][pid].pos.x == board->player[current_player][(int)list_id[0]].pos.x + row_movement)
                    return true;
            }
            if (direction == 1)
            {
                if (board->player[WHITE][pid].pos.x == board->player[current_player][(int)list_id[0]].pos.x - row_movement)
                    return true;
                if (board->player[BLACK][pid].pos.x == board->player[current_player][(int)list_id[0]].pos.x - row_movement)
                    return true;
            }
        }
    }
    return false;
}

bool is_col_blocked(board_s *board, char *list_id, int current_player)
{
    int direction;
    if (board->player[current_player][(int)list_id[0]].pos.y - list_id[2] > 0)
        direction = 1;
    else
        direction = 0;
    for (int pid = PAWN0; pid < NUM_PIECES; pid++)
    {
        int num_cell_path = abs(board->player[current_player][(int)list_id[0]].pos.y - list_id[2]);
        for (int col_movement = 0; col_movement < num_cell_path; col_movement++)
        {
            if (direction == 0)
            {
                if (board->player[WHITE][pid].pos.x == board->player[current_player][(int)list_id[0]].pos.y + col_movement)
                    return true;
                if (board->player[BLACK][pid].pos.x == board->player[current_player][(int)list_id[0]].pos.y + col_movement)
                    return true;
            }
            if (direction == 1)
            {
                if (board->player[WHITE][pid].pos.x == board->player[current_player][(int)list_id[0]].pos.y - col_movement)
                    return true;
                if (board->player[BLACK][pid].pos.x == board->player[current_player][(int)list_id[0]].pos.y - col_movement)
                    return true;
            }
        }
    }
    return false;
}

bool is_piece_selected_alive(board_s *board, char *list_id, int current_player)
{
    if (board->player[current_player][(int)list_id[0]].is_alive == 0)
        return false;
    return true;
}

void capture_enemy_piece(board_s *board, char *list_id, int current_player)
{
    for (int pid = PAWN0; pid < NUM_PIECES - 1; pid++)
    {
        if (current_player == WHITE)
        {
            if (board->player[BLACK][pid].pos.x == list_id[1] &&
                board->player[BLACK][pid].pos.y == list_id[2])
                board->player[BLACK][pid].is_alive = 0;
        }
        if (current_player == BLACK)
        {
            if (board->player[WHITE][pid].pos.x == list_id[1] &&
                board->player[WHITE][pid].pos.y == list_id[2])
                board->player[WHITE][pid].is_alive = 0;
        }
    }
}

bool is_cell_occupied_by_ally(board_s *board, char *list_id, int current_player)
{

    for (int pid = 0; pid < NUM_PIECES; pid++)
    {
        if (board->player[current_player][pid].pos.x == list_id[1] &&
            board->player[current_player][pid].pos.y == list_id[2])
        {
            if ((int)list_id[0] != pid)
                return true;
        }
    }
    return false;
}

bool tokenise_player_choice(char *player_move, char *list_id)
{
    char *selected_piece, *row, *col;

    selected_piece = strtok(player_move, "-");
    if (strcmp(selected_piece, "0") == EXIT)
        return true;
    col = strtok(NULL, "-");
    row = strtok(NULL, "-");

    list_id[0] = get_piece_id(selected_piece);
    list_id[1] = atoi(row) - 1;
    list_id[2] = get_col_id(col);

    printf("\nPiece: %s, id: %d \nRow: %s w/ id: %d \nCol: %s w/ id: %d\n", selected_piece, list_id[0], row, list_id[1], col, list_id[2]);
    return false;
}

void update_piece(board_s *board, int current_player, char *list_id)
{
    board->player[current_player][(int)list_id[0]].pos.x = list_id[1];
    board->player[current_player][(int)list_id[0]].pos.y = list_id[2];
}

int get_piece_id(const char *name)
{
    for (int i = 0; i < NUM_PIECES; i++)
    {
        if (strcasecmp(name, piece_map[i].name) == 0)
        {
            return piece_map[i].piece_type;
        }
    }
    return -1;
}

int get_col_id(const char *name)
{
    for (int i = 0; i < NUM_ROW; i++)
    {
        if (strcasecmp(name, col_map[i].name) == 0)
        {
            return col_map[i].id;
        }
    }
    return -1;
}

void reset_board(board_s *board)
{
    for (int i = PAWN0; i < ROOK8; i++)
    {
        board->player[WHITE][i].piece_type = i;
        board->player[WHITE][i].pos.x = 1;
        board->player[WHITE][i].pos.y = i;

        board->player[BLACK][i].piece_type = i;
        board->player[BLACK][i].pos.x = NUM_ROW - 2;
        board->player[BLACK][i].pos.y = i;
    }

    board->player[WHITE][ROOK8].piece_type = ROOK8;
    board->player[WHITE][ROOK8].pos.x = 0;
    board->player[WHITE][ROOK8].pos.y = 0;
    board->player[WHITE][ROOK9].piece_type = ROOK9;
    board->player[WHITE][ROOK9].pos.x = 0;
    board->player[WHITE][ROOK9].pos.y = NUM_COL - 1;
    board->player[BLACK][ROOK8].piece_type = ROOK8;
    board->player[BLACK][ROOK8].pos.x = NUM_ROW - 1;
    board->player[BLACK][ROOK8].pos.y = 0;
    board->player[BLACK][ROOK9].piece_type = ROOK9;
    board->player[BLACK][ROOK9].pos.x = NUM_ROW - 1;
    board->player[BLACK][ROOK9].pos.y = NUM_COL - 1;

    board->player[WHITE][KNIGHT10].piece_type = KNIGHT10;
    board->player[WHITE][KNIGHT10].pos.x = 0;
    board->player[WHITE][KNIGHT10].pos.y = 1;
    board->player[WHITE][KNIGHT11].piece_type = KNIGHT11;
    board->player[WHITE][KNIGHT11].pos.x = 0;
    board->player[WHITE][KNIGHT11].pos.y = NUM_COL - 2;
    board->player[BLACK][KNIGHT10].piece_type = KNIGHT10;
    board->player[BLACK][KNIGHT10].pos.x = NUM_ROW - 1;
    board->player[BLACK][KNIGHT10].pos.y = 1;
    board->player[BLACK][KNIGHT11].piece_type = KNIGHT11;
    board->player[BLACK][KNIGHT11].pos.x = NUM_ROW - 1;
    board->player[BLACK][KNIGHT11].pos.y = NUM_COL - 2;

    board->player[WHITE][BISHOP12].piece_type = BISHOP12;
    board->player[WHITE][BISHOP12].pos.x = 0;
    board->player[WHITE][BISHOP12].pos.y = 2;
    board->player[WHITE][BISHOP13].piece_type = BISHOP13;
    board->player[WHITE][BISHOP13].pos.x = 0;
    board->player[WHITE][BISHOP13].pos.y = NUM_COL - 3;
    board->player[BLACK][BISHOP12].piece_type = BISHOP12;
    board->player[BLACK][BISHOP12].pos.x = NUM_ROW - 1;
    board->player[BLACK][BISHOP12].pos.y = 2;
    board->player[BLACK][BISHOP13].piece_type = BISHOP13;
    board->player[BLACK][BISHOP13].pos.x = NUM_ROW - 1;
    board->player[BLACK][BISHOP13].pos.y = NUM_COL - 3;

    board->player[WHITE][QUEEN].piece_type = QUEEN;
    board->player[WHITE][QUEEN].pos.x = 0;
    board->player[WHITE][QUEEN].pos.y = NUM_COL - 5;
    board->player[BLACK][QUEEN].piece_type = QUEEN;
    board->player[BLACK][QUEEN].pos.x = NUM_ROW - 1;
    board->player[BLACK][QUEEN].pos.y = NUM_COL - 5;

    board->player[WHITE][KING].piece_type = KING;
    board->player[WHITE][KING].pos.x = 0;
    board->player[WHITE][KING].pos.y = NUM_COL - 4;
    board->player[BLACK][KING].piece_type = KING;
    board->player[BLACK][KING].pos.x = NUM_ROW - 1;
    board->player[BLACK][KING].pos.y = NUM_COL - 4;

    for (int i = 0; i < NUM_PIECES; i++)
    {
        board->player[WHITE][i].is_alive = 1;
        board->player[BLACK][i].is_alive = 1;
    }

    for (int i = 0; i < NUM_COL; i++)
    {
        col_map[i].id = i;
    }
}

void load_game(board_s *board)
{
    print_hellowep();
}