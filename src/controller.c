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
    FILE file;
    int current_player = WHITE;
    while (true)
    {
        print_main_menu();

        switch (choose_main_menu())
        {
        case START:
            reset_board(&board);
            start_game(&board, &file, current_player);
            continue;

        case SAVE:
            save_game(&board, &file, current_player);
            continue;

        case LOAD:
            load_game(&board, &file, current_player);
            start_game(&board, &file, current_player);
            continue;

        case INVENT:
            print_hellowep();
            continue;

        case RULES:
            print_rules();
            continue;

        case SETTINGS:
            print_settings();
            continue;

        case EXIT:
            print_byecat();
            return;

        default:
            printf("\nInvalid input");
            continue;
        }
    }
}

void start_game(board_s *board, FILE *file, int current_player)
{
    print_board(board);
    int player_choice = START;
    while (1)
    {
        char player_move[20], list_id[3];
        get_player_choice(player_move);
        player_choice = tokenise_player_choice(player_move, list_id);
        if (player_choice == EXIT)
            break;
        if (player_choice == SAVE)
        {
            save_game(board, file, current_player);
            continue;
        }

        piece_movement_validity(list_id, board, current_player);
        if (!is_pawn_move_legal(list_id, board, current_player))
        {
            printf("\nPawn move invalid");
            continue;
        }

        if (!is_piece_selected_alive(board, list_id, current_player))
        {
            printf("\nThis piece has been captured, choose that is still alive");
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
    }
}

bool is_piece_selected_alive(board_s *board, char *list_id, int current_player)
{
    if (board->player[current_player][(int)list_id[0]].is_alive == 0)
        return false;
    return true;
}

void capture_enemy_piece(board_s *board, char *list_id, int current_player)
{
    for (int pid = PAWN0; pid < NUM_PIECES; pid++)
    {
        if (current_player == WHITE)
        {
            if (board->player[BLACK][pid].pos.x == list_id[1] &&
                board->player[BLACK][pid].pos.y == list_id[2])
                board->player[BLACK][pid].is_alive = 0;
        }
        else if (board->player[WHITE][pid].pos.x == list_id[1] &&
                 board->player[WHITE][pid].pos.y == list_id[2])
            board->player[WHITE][pid].is_alive = 0;
        {
        }
    }
}

bool is_cell_occupied_by_ally(board_s *board, char *list_id, int current_player)
{

    for (int pid = PAWN0; pid < NUM_PIECES; pid++)
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

int tokenise_player_choice(char *player_move, char *list_id)
{
    char *selected_piece, *row, *col;

    selected_piece = strtok(player_move, "-");
    if (strcmp(selected_piece, "exit") == 0)
        return EXIT;
    if (strcmp(selected_piece, "save") == 0)
        return SAVE;
    col = strtok(NULL, "-");
    row = strtok(NULL, "-");

    list_id[0] = get_piece_id(selected_piece);
    list_id[1] = atoi(row) - 1;
    list_id[2] = get_col_id(col);

    printf("\nPiece: %s, id: %d \nRow: %s w/ id: %d \nCol: %s w/ id: %d\n", selected_piece, list_id[0], row, list_id[1], col, list_id[2]);
    return 1;
}

void update_piece(board_s *board, int current_player, char *list_id)
{
    board->player[current_player][(int)list_id[0]].pos.x = list_id[1];
    board->player[current_player][(int)list_id[0]].pos.y = list_id[2];
}

int get_piece_id(const char *name)
{
    for (int i = PAWN0; i < NUM_PIECES; i++)
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

void save_game(board_s *board, FILE *file, int current_player)
{
    file = fopen("saved_game", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%d-", current_player);

    for (int piece = PAWN0; piece < NUM_PIECES; piece++)
    {
        fprintf(file, "%u-%u-%d-%d-",
                board->player[WHITE][piece].piece_type,
                board->player[WHITE][piece].pos.x,
                board->player[WHITE][piece].pos.y,
                board->player[WHITE][piece].is_alive);

        fprintf(file, "%u-%u-%d-%d-",
                board->player[BLACK][piece].piece_type,
                board->player[BLACK][piece].pos.x,
                board->player[BLACK][piece].pos.y,
                board->player[BLACK][piece].is_alive);
    }

    fclose(file);
}

void load_game(board_s *board, FILE *file, int current_player)
{
    file = fopen("saved_game", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    char buffer[500];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf(" %s", buffer);
    }
    printf("\nFormat is the following : piece_type-x-y-alive");

    set_board_from_save(board, buffer, &current_player);
    start_game(board, file, current_player);
    fclose(file);
}

void set_board_from_save(board_s *board, char *buffer, int *current_player)
{
    char *player_turn = strtok(buffer, "-");
    *current_player = atoi(player_turn);
    printf(" %s", player_turn);
    for (int piece = PAWN0; piece < NUM_PIECES; piece++)
    {
        board->player[WHITE][piece].piece_type = atoi(strtok(NULL, "-"));
        board->player[WHITE][piece].pos.x = atoi(strtok(NULL, "-"));
        board->player[WHITE][piece].pos.y = atoi(strtok(NULL, "-"));
        board->player[WHITE][piece].is_alive = atoi(strtok(NULL, "-"));

        board->player[BLACK][piece].piece_type = atoi(strtok(NULL, "-"));
        board->player[BLACK][piece].pos.x = atoi(strtok(NULL, "-"));
        board->player[BLACK][piece].pos.y = atoi(strtok(NULL, "-"));
        board->player[BLACK][piece].is_alive = atoi(strtok(NULL, "-"));
    }
}

void piece_movement_validity(char *list_id, board_s *board, int current_player)
{
    for (int pid = PAWN0; pid < ROOK8; pid++)
    {
        if (list_id[0] == pid)
            is_pawn_move_legal(list_id, board, current_player);
    }
    // if (list_id[0] == ROOK8 || list_id[0] == ROOK9)
    // return is_rook_move_legal(board, list_id, current_player);
    // if (list_id[0] == KNIGHT10 || list_id[0] == KNIGHT11)
    // return is_knight_move_legal(board, list_id, current_player);
    // if (list_id[0] == BISHOP12 || list_id[0] == BISHOP13)
    // return is_bishop_move_legal(board, list_id, current_player);
    // if (list_id[0] == QUEEN)
    // return is_queen_move_legal(board, list_id, current_player);
    // if (list_id[0] == KING)
    // return is_king_move_legal(board, list_id, current_player);
    return;
}

bool is_pawn_move_legal(char *list_id, board_s *board, int current_player)
{
    int pawn_id = (int)list_id[0];
    int target_row = list_id[1];
    int target_col = list_id[1];
    int current_row = board->player[current_player][pawn_id].pos.x;
    int current_col = board->player[current_player][pawn_id].pos.y;

    if (target_col != current_col)
    {
        printf("\nInvalid pawn move (cannot move column without capturing enemy)");
        return false;
    }

    int direction = (current_player == WHITE) ? 1 : -1;

    // Check if it's the pawn's first move
    bool is_first_move = (current_player == WHITE && current_row == 1) ||
                         (current_player == BLACK && current_row == 6);

    // Check for two square move (only on first move)
    if (is_first_move && target_row == current_row + (2 * direction))
    {
        if (!is_cell_occupied_by_ally(board, list_id, current_player) &&
            !is_cell_occupied_by_ally(board, list_id, current_player))
            return true;
    }

    // Check for one square move
    if (target_row == current_row + direction)
    {
        if (!is_cell_occupied_by_ally(board, list_id, current_player))
            return true;
    }

    return false;
}