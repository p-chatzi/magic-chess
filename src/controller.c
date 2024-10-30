#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "controller.h"
#include "view.h"
#include "model.h"

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

    int current_player = WHITE;
    while (1)
    {
        char player_move[20];
        get_player_choice(player_move);
        bool is_game_finished = player_moves_piece(board, current_player, player_move);
        if (is_game_finished == true)
            break;
        print_board(board);

        if (current_player == WHITE)
            current_player = BLACK;
        else
            current_player = WHITE;
    }
}

int player_moves_piece(board_s *board, int current_player, char player_move[20])
{
    char *selected_piece, *row, *col;
    int row_id, col_id, piece_id;

    selected_piece = strtok(player_move, "-");
    if (strcmp(selected_piece, "0") == EXIT)
        return true;
    col = strtok(NULL, "-");
    row = strtok(NULL, "-");

    piece_id = get_piece_id(selected_piece);
    col_id = get_col_id(col);
    row_id = atoi(row) - 1;

    printf("\nPiece: %s, id: %d \nRow: %s w/ id: %d \nCol: %s w/ id: %d\n", selected_piece, piece_id, row, row_id, col, col_id);

    board->player[current_player][piece_id].pos.x = row_id;
    board->player[current_player][piece_id].pos.y = col_id;

    return false;
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