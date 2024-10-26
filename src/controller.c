#include <stdbool.h> // bool
#include <stdio.h>   // printf()

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
        { // Enum for the cases
        case 1:
            start_game(&board);
            continue;

        case 2:
            load_game(&board);
            continue;

        case 3:
            print_rules();
            continue;

        case 4:
            print_settings();
            continue;

        case 5:
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
    printf("\nGAMMMMEEE ON!");
    // set_color(); // Player 1 is white for now, will only need later
    reset_board(board);
    print_board(board);
}

void reset_board(board_s *board)
{
    for (int i = PAWN; i < ROOK; i++)
    {
        board->player[WHITE][i].piece_type = PAWN;
        board->player[WHITE][i].pos.x = 1;
        board->player[WHITE][i].pos.y = i;

        board->player[BLACK][i].piece_type = PAWN;
        board->player[BLACK][i].pos.x = NUM_ROW - 1;
        board->player[BLACK][i].pos.y = i;
    }

    board->player[WHITE][ROOK].piece_type = ROOK;
    board->player[WHITE][ROOK].pos.x = 0;
    board->player[WHITE][ROOK].pos.y = 0;
    board->player[WHITE][ROOK + 1].piece_type = ROOK;
    board->player[WHITE][ROOK + 1].pos.x = 0;
    board->player[WHITE][ROOK + 1].pos.y = NUM_COL;
    board->player[BLACK][ROOK].piece_type = ROOK;
    board->player[BLACK][ROOK].pos.x = NUM_ROW;
    board->player[BLACK][ROOK].pos.y = 0;
    board->player[BLACK][ROOK + 1].piece_type = ROOK;
    board->player[BLACK][ROOK + 1].pos.x = NUM_ROW;
    board->player[BLACK][ROOK + 1].pos.y = NUM_COL;

    board->player[WHITE][KNIGHT].piece_type = KNIGHT;
    board->player[WHITE][KNIGHT].pos.x = 0;
    board->player[WHITE][KNIGHT].pos.y = 1;
    board->player[WHITE][KNIGHT + 1].piece_type = KNIGHT;
    board->player[WHITE][KNIGHT + 1].pos.x = 0;
    board->player[WHITE][KNIGHT + 1].pos.y = NUM_COL - 1;
    board->player[BLACK][KNIGHT].piece_type = KNIGHT;
    board->player[BLACK][KNIGHT].pos.x = NUM_ROW;
    board->player[BLACK][KNIGHT].pos.y = 1;
    board->player[BLACK][KNIGHT + 1].piece_type = KNIGHT;
    board->player[BLACK][KNIGHT + 1].pos.x = NUM_ROW;
    board->player[BLACK][KNIGHT + 1].pos.y = NUM_COL - 1;

    board->player[WHITE][BISHOP].piece_type = BISHOP;
    board->player[WHITE][BISHOP].pos.x = 0;
    board->player[WHITE][BISHOP].pos.y = 2;
    board->player[WHITE][BISHOP + 1].piece_type = BISHOP;
    board->player[WHITE][BISHOP + 1].pos.x = 0;
    board->player[WHITE][BISHOP + 1].pos.y = NUM_COL - 2;
    board->player[BLACK][BISHOP].piece_type = BISHOP;
    board->player[BLACK][BISHOP].pos.x = NUM_ROW;
    board->player[BLACK][BISHOP].pos.y = 2;
    board->player[BLACK][BISHOP + 1].piece_type = BISHOP;
    board->player[BLACK][BISHOP + 1].pos.x = NUM_ROW;
    board->player[BLACK][BISHOP + 1].pos.y = NUM_COL - 2;

    board->player[WHITE][QUEEN].piece_type = QUEEN;
    board->player[WHITE][QUEEN].pos.x = 0;
    board->player[WHITE][QUEEN].pos.y = NUM_COL - 4;
    board->player[BLACK][QUEEN].piece_type = QUEEN;
    board->player[BLACK][QUEEN].pos.x = NUM_ROW;
    board->player[BLACK][QUEEN].pos.y = NUM_COL - 4;

    board->player[WHITE][KING].piece_type = KING;
    board->player[WHITE][KING].pos.x = 0;
    board->player[WHITE][KING].pos.y = NUM_COL - 3;
    board->player[BLACK][KING].piece_type = KING;
    board->player[BLACK][KING].pos.x = NUM_ROW;
    board->player[BLACK][KING].pos.y = NUM_COL - 3;

    for (int i = 0; i < NUM_PIECES; i++)
    {
        board->player[WHITE][i].is_alive = 1;
        board->player[BLACK][i].is_alive = 1;
    }
}

// void set_color()
// {
//     char chosen_color = choose_color();
//     if (choose_color == 'w')
//     {

//     }
//     if (choose_color == 'b')
//     {

//     }
//     else
//     {
//         printf("\ninvalid entry");
//         return;
//     }
// }

/*WIP*/
void load_game(board_s *board)
{
    print_hellowep();
}