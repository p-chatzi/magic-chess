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
    bool is_game_finished = false;
    while (1)
    {
        char player_move[20], *selected_piece, *row, *col;
        int x, y, piece_id;
        is_game_finished = get_player_choice(player_move);
        if (is_game_finished == true)
            break;

        selected_piece = strtok(player_move, "-");
        row = strtok(NULL, "-");
        col = strtok(NULL, "-");

        piece_id = get_piece_id(selected_piece);
        x = get_row_id(row);
        y = atoi(col);

        printf("Piece:%s with id:%d \nRow:%s w/ id:%d \nCol:%s w/ id:%d", selected_piece, piece_id, row, x, col, y);

        board->player[current_player][piece_id].pos.x = x;
        board->player[current_player][piece_id].pos.y = y;

        print_board(board);

        if (current_player == WHITE)
            current_player = BLACK;
        else
            current_player = WHITE;
    }
}

int get_piece_id(const char *name)
{
    for (int i = 0; i < NUM_PIECES; i++)
    {
        if (strcasecmp(name, piece_map[i].name) == 0)
        {
            return piece_map[i].id;
        }
    }
    return -1;
}

int get_row_id(const char *name)
{
    for (int i = 0; i < NUM_ROW; i++)
    {
        if (strcasecmp(name, row_map[i].name) == 0)
        {
            return row_map[i].id;
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

        sprintf(board->player[WHITE][i].name, "pawn%d", i);
        sprintf(board->player[BLACK][i].name, "pawn%d", i);
        board->player[WHITE][i].id = get_piece_id(board->player[WHITE][i].name);
        board->player[BLACK][i].id = get_piece_id(board->player[BLACK][i].name);
    }

    board->player[WHITE][ROOK8].piece_type = ROOK8;
    board->player[WHITE][ROOK8].pos.x = 0;
    board->player[WHITE][ROOK8].pos.y = 0;
    sprintf(board->player[WHITE][ROOK8].name, "rook8");
    board->player[WHITE][ROOK9].piece_type = ROOK9;
    board->player[WHITE][ROOK9].pos.x = 0;
    board->player[WHITE][ROOK9].pos.y = NUM_COL - 1;
    sprintf(board->player[WHITE][ROOK9].name, "rook9");
    board->player[BLACK][ROOK8].piece_type = ROOK8;
    board->player[BLACK][ROOK8].pos.x = NUM_ROW - 1;
    board->player[BLACK][ROOK8].pos.y = 0;
    sprintf(board->player[BLACK][ROOK8].name, "rook8");
    board->player[BLACK][ROOK9].piece_type = ROOK9;
    board->player[BLACK][ROOK9].pos.x = NUM_ROW - 1;
    board->player[BLACK][ROOK9].pos.y = NUM_COL - 1;
    sprintf(board->player[BLACK][ROOK9].name, "rook9");
    board->player[WHITE][ROOK8].id = get_piece_id(board->player[WHITE][ROOK8].name);
    board->player[WHITE][ROOK8].id = get_piece_id(board->player[WHITE][ROOK8].name);
    board->player[BLACK][ROOK9].id = get_piece_id(board->player[BLACK][ROOK9].name);
    board->player[BLACK][ROOK9].id = get_piece_id(board->player[BLACK][ROOK9].name);

    board->player[WHITE][KNIGHT10].piece_type = KNIGHT10;
    board->player[WHITE][KNIGHT10].pos.x = 0;
    board->player[WHITE][KNIGHT10].pos.y = 1;
    sprintf(board->player[WHITE][KNIGHT10].name, "knight10");
    board->player[WHITE][KNIGHT11].piece_type = KNIGHT11;
    board->player[WHITE][KNIGHT11].pos.x = 0;
    board->player[WHITE][KNIGHT11].pos.y = NUM_COL - 2;
    sprintf(board->player[WHITE][KNIGHT11].name, "knight11");
    board->player[BLACK][KNIGHT10].piece_type = KNIGHT10;
    board->player[BLACK][KNIGHT10].pos.x = NUM_ROW - 1;
    board->player[BLACK][KNIGHT10].pos.y = 1;
    sprintf(board->player[BLACK][KNIGHT10].name, "knight10");
    board->player[BLACK][KNIGHT11].piece_type = KNIGHT11;
    board->player[BLACK][KNIGHT11].pos.x = NUM_ROW - 1;
    board->player[BLACK][KNIGHT11].pos.y = NUM_COL - 2;
    sprintf(board->player[BLACK][KNIGHT11].name, "knight11");
    board->player[WHITE][KNIGHT10].id = get_piece_id(board->player[WHITE][KNIGHT10].name);
    board->player[WHITE][KNIGHT10].id = get_piece_id(board->player[WHITE][KNIGHT10].name);
    board->player[BLACK][KNIGHT11].id = get_piece_id(board->player[BLACK][KNIGHT11].name);
    board->player[BLACK][KNIGHT11].id = get_piece_id(board->player[BLACK][KNIGHT11].name);

    board->player[WHITE][BISHOP12].piece_type = BISHOP12;
    board->player[WHITE][BISHOP12].pos.x = 0;
    board->player[WHITE][BISHOP12].pos.y = 2;
    sprintf(board->player[WHITE][BISHOP12].name, "bishop12");
    board->player[WHITE][BISHOP13].piece_type = BISHOP13;
    board->player[WHITE][BISHOP13].pos.x = 0;
    board->player[WHITE][BISHOP13].pos.y = NUM_COL - 3;
    sprintf(board->player[WHITE][BISHOP13].name, "bishop13");
    board->player[BLACK][BISHOP12].piece_type = BISHOP12;
    board->player[BLACK][BISHOP12].pos.x = NUM_ROW - 1;
    board->player[BLACK][BISHOP12].pos.y = 2;
    sprintf(board->player[BLACK][BISHOP12].name, "bishop12");
    board->player[BLACK][BISHOP13].piece_type = BISHOP13;
    board->player[BLACK][BISHOP13].pos.x = NUM_ROW - 1;
    board->player[BLACK][BISHOP13].pos.y = NUM_COL - 3;
    sprintf(board->player[BLACK][BISHOP13].name, "bishop13");
    board->player[WHITE][BISHOP12].id = get_piece_id(board->player[WHITE][BISHOP12].name);
    board->player[WHITE][BISHOP12].id = get_piece_id(board->player[WHITE][BISHOP12].name);
    board->player[BLACK][BISHOP13].id = get_piece_id(board->player[BLACK][BISHOP13].name);
    board->player[BLACK][BISHOP13].id = get_piece_id(board->player[BLACK][BISHOP13].name);

    board->player[WHITE][QUEEN].piece_type = QUEEN;
    board->player[WHITE][QUEEN].pos.x = 0;
    board->player[WHITE][QUEEN].pos.y = NUM_COL - 5;
    board->player[BLACK][QUEEN].piece_type = QUEEN;
    board->player[BLACK][QUEEN].pos.x = NUM_ROW - 1;
    board->player[BLACK][QUEEN].pos.y = NUM_COL - 5;
    board->player[WHITE][QUEEN].id = get_piece_id(board->player[WHITE][QUEEN].name);
    board->player[BLACK][QUEEN].id = get_piece_id(board->player[BLACK][QUEEN].name);

    board->player[WHITE][KING].piece_type = KING;
    board->player[WHITE][KING].pos.x = 0;
    board->player[WHITE][KING].pos.y = NUM_COL - 4;
    board->player[BLACK][KING].piece_type = KING;
    board->player[BLACK][KING].pos.x = NUM_ROW - 1;
    board->player[BLACK][KING].pos.y = NUM_COL - 4;
    board->player[WHITE][KING].id = get_piece_id(board->player[WHITE][KING].name);
    board->player[BLACK][KING].id = get_piece_id(board->player[BLACK][KING].name);

    for (int i = 0; i < NUM_PIECES; i++)
    {
        board->player[WHITE][i].is_alive = 1;
        board->player[BLACK][i].is_alive = 1;
    }

    for (int i = 0; i < NUM_ROW; i++)
    {
        row_map[i].id = i;
    }
}

void load_game(board_s *board)
{
    print_hellowep();
}