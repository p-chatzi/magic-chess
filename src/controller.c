#include <stdbool.h>
#include <stdio.h>

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

    int player_move[4] = {0};
    get_player_choice(&player_move[0]);
    decipher_move(board, player_move);

    print_board(board);
}

void reset_board(board_s *board)
{
    for (int i = PAWN; i < ROOK; i++)
    {
        board->player[WHITE][i].piece_type = PAWN + i;
        board->player[WHITE][i].pos.x = 1;
        board->player[WHITE][i].pos.y = i;

        board->player[BLACK][i].piece_type = PAWN + i;
        board->player[BLACK][i].pos.x = NUM_ROW - 2;
        board->player[BLACK][i].pos.y = i;
    }

    board->player[WHITE][ROOK].piece_type = ROOK;
    board->player[WHITE][ROOK].pos.x = 0;
    board->player[WHITE][ROOK].pos.y = 0;
    board->player[WHITE][ROOK + 1].piece_type = ROOK + 1;
    board->player[WHITE][ROOK + 1].pos.x = 0;
    board->player[WHITE][ROOK + 1].pos.y = NUM_COL - 1;
    board->player[BLACK][ROOK].piece_type = ROOK;
    board->player[BLACK][ROOK].pos.x = NUM_ROW - 1;
    board->player[BLACK][ROOK].pos.y = 0;
    board->player[BLACK][ROOK + 1].piece_type = ROOK + 1;
    board->player[BLACK][ROOK + 1].pos.x = NUM_ROW - 1;
    board->player[BLACK][ROOK + 1].pos.y = NUM_COL - 1;

    board->player[WHITE][KNIGHT].piece_type = KNIGHT;
    board->player[WHITE][KNIGHT].pos.x = 0;
    board->player[WHITE][KNIGHT].pos.y = 1;
    board->player[WHITE][KNIGHT + 1].piece_type = KNIGHT + 1;
    board->player[WHITE][KNIGHT + 1].pos.x = 0;
    board->player[WHITE][KNIGHT + 1].pos.y = NUM_COL - 2;
    board->player[BLACK][KNIGHT].piece_type = KNIGHT;
    board->player[BLACK][KNIGHT].pos.x = NUM_ROW - 1;
    board->player[BLACK][KNIGHT].pos.y = 1;
    board->player[BLACK][KNIGHT + 1].piece_type = KNIGHT + 1;
    board->player[BLACK][KNIGHT + 1].pos.x = NUM_ROW - 1;
    board->player[BLACK][KNIGHT + 1].pos.y = NUM_COL - 2;

    board->player[WHITE][BISHOP].piece_type = BISHOP;
    board->player[WHITE][BISHOP].pos.x = 0;
    board->player[WHITE][BISHOP].pos.y = 2;
    board->player[WHITE][BISHOP + 1].piece_type = BISHOP + 1;
    board->player[WHITE][BISHOP + 1].pos.x = 0;
    board->player[WHITE][BISHOP + 1].pos.y = NUM_COL - 3;
    board->player[BLACK][BISHOP].piece_type = BISHOP;
    board->player[BLACK][BISHOP].pos.x = NUM_ROW - 1;
    board->player[BLACK][BISHOP].pos.y = 2;
    board->player[BLACK][BISHOP + 1].piece_type = BISHOP + 1;
    board->player[BLACK][BISHOP + 1].pos.x = NUM_ROW - 1;
    board->player[BLACK][BISHOP + 1].pos.y = NUM_COL - 3;

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
}

void decipher_move(board_s *board, int *player_move)
{
    int color = player_move[0];
    int piece = player_move[1];
    int x = player_move[2];
    int y = player_move[3];

    board->player[color][piece].pos.x = x;
    board->player[color][piece].pos.y = y;
}

void load_game(board_s *board)
{
    print_hellowep();
}