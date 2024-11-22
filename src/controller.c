#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "controller.h"
#include "view.h"
#include "model.h"

/*
    Called at start
    Redirects to fonctions needed based on user's choice
    No return
*/
void boot_magic_chess() {
    board_s board;
    FILE file;
    int current_player = WHITE;
    while(true) {
        print_main_menu();

        switch(choose_main_menu()) {
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
            print_wip();
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

/*
    The game loop, calls everything needed to make chess work
    No return
*/
void start_game(board_s* board, FILE* file, int current_player) {
    clear_terminal();
    print_board(board);
    int player_choice = START;
    bool tie_offer = false;
    const char* player[] = {"White", "Black"};

    while(1) {
        // King in danger!
        if(is_my_king_checked(board, current_player))
            printf("\n%s's king in danger!", player[current_player]);

        // What does the player want to do
        char player_move[20], list_id[NB_INPUTS];
        get_player_choice(player_move);
        player_choice = tokenise_player_choice(player_move, list_id);
        if(player_choice == TIE) {
            if(tie_offer == true) {
                printf("\nThe game ended in a tie");
                return;
            }
            printf(
                "\n%s offers to tie, to end the game in a tie, enter tie", player[current_player]);
            tie_offer = true;
            continue;
        }
        if(player_choice == SAVE) {
            save_game(board, file, current_player);
            continue;
        }

        // You are not allowed to move at your current position (no skipping turns!)
        if(is_destination_current_position(board, list_id, current_player)) continue;

        // These pieces don't exist anymore, let them rest in peace
        if(!is_piece_selected_alive(board, list_id, current_player)) {
            printf("\nThis piece has been captured, choose that is still alive");
            continue;
        }

        // Don't eat your allies!
        if(is_cell_occupied_by_ally(board, list_id, current_player)) {
            printf("\nCannot move on top of your own piece");
            continue;
        }

        // Is the player's choice even a legal move?
        if(!piece_movement_validity(board, list_id, current_player)) {
            printf("\nInvalid %s move", piece_map[(int)list_id[PIECE_ID]].name);
            continue;
        }

        // Everything validated!
        capture_enemy_piece(board, list_id, current_player);
        update_piece(board, current_player, list_id);
        print_board(board);
        if(current_player == WHITE)
            current_player = BLACK;
        else
            current_player = WHITE;
    }
}

/*
    Checks if the piece selected is alive or not
    Since you cannot play a piece that's been captured ;)
    Returns : True if the piece is alive
*/
bool is_piece_selected_alive(board_s* board, char* list_id, int current_player) {
    return board->player[current_player][(int)list_id[PIECE_ID]].is_alive;
}

/*
    Deletes a piece when an oponnent places one of their piece on top
    No return.
*/
void capture_enemy_piece(board_s* board, char* list_id, int current_player) {
    for(int pid = PAWN0; pid < NUM_PIECES; pid++) {
        if(current_player == WHITE) {
            if(board->player[BLACK][pid].pos.x == list_id[ROW_ID] &&
               board->player[BLACK][pid].pos.y == list_id[COL_ID])
                board->player[BLACK][pid].is_alive = 0;
        } else if(
            board->player[WHITE][pid].pos.x == list_id[ROW_ID] &&
            board->player[WHITE][pid].pos.y == list_id[COL_ID])
            board->player[WHITE][pid].is_alive = 0;
        {}
    }
}

/*
    Checks if a cell is occupied by ally
    Since you cannot move ontop of ally
    Return : True if cell is occupied, false if not
*/
bool is_cell_occupied_by_ally(board_s* board, char* list_id, int current_player) {
    for(int pid = PAWN0; pid < NUM_PIECES; pid++) {
        if(board->player[current_player][pid].pos.x == list_id[ROW_ID] &&
           board->player[current_player][pid].pos.y == list_id[COL_ID]) {
            if((int)list_id[PIECE_ID] != pid) return true;
        }
    }
    return false;
}

/*
    Checks if a cell is occupied by an enemy
    Useful to check if path is occupied for example
    Return : True if cell is occupied, false if not
*/
bool is_cell_occupied_by_enemy(board_s* board, char* list_id, int current_player) {
    if(current_player == BLACK) {
        for(int pid = PAWN0; pid < NUM_PIECES; pid++) {
            if(board->player[WHITE][pid].pos.x == list_id[ROW_ID] &&
               board->player[WHITE][pid].pos.y == list_id[COL_ID]) {
                if((int)list_id[PIECE_ID] != pid) return true;
            }
        }
    }
    if(current_player == WHITE) {
        for(int pid = PAWN0; pid < NUM_PIECES; pid++) {
            if(board->player[BLACK][pid].pos.x == list_id[ROW_ID] &&
               board->player[BLACK][pid].pos.y == list_id[COL_ID]) {
                if((int)list_id[PIECE_ID] != pid) return true;
            }
        }
    }
    return false;
}

/*
    Parses the player's input
    Prints the parsed info (for testing sake)
    Return : TIE / SAVE or START based on users first input
*/
int tokenise_player_choice(char* player_move, char* list_id) {
    char *selected_piece, *row, *col;

    selected_piece = strtok(player_move, "-");
    if(strcmp(selected_piece, "tie") == 0) return TIE;
    if(strcmp(selected_piece, "save") == 0) return SAVE;
    col = strtok(NULL, "-"); // In chess we say letter then number
    row = strtok(NULL, "-"); // Which is actually : column then row

    list_id[PIECE_ID] = get_piece_id(selected_piece);
    list_id[ROW_ID] = atoi(row) - 1;
    list_id[COL_ID] = get_col_id(col);

    printf(
        "\nPiece: %s, id: %d \nRow: %s w/ id: %d \nCol: %s w/ id: %d\n",
        selected_piece,
        list_id[0],
        row,
        list_id[1],
        col,
        list_id[2]);
    return START;
}

/*
    Updates the piece with the player's input
    No return.
*/
void update_piece(board_s* board, int current_player, char* list_id) {
    board->player[current_player][(int)list_id[PIECE_ID]].pos.x = list_id[ROW_ID];
    board->player[current_player][(int)list_id[PIECE_ID]].pos.y = list_id[COL_ID];
}

/*
    Compares the name of the piece with the name given
    Return : ID of the piece
*/
int get_piece_id(const char* name) {
    for(int i = PAWN0; i < NUM_PIECES; i++) {
        if(strcasecmp(name, piece_map[i].name) == 0) {
            return piece_map[i].piece_type;
        }
    }
    return -1;
}

/*
    Compares the name of the column with the name given
    Return : ID of the column
*/
int get_col_id(const char* name) {
    for(int i = 0; i < NUM_ROW; i++) {
        if(strcasecmp(name, col_map[i].name) == 0) {
            return col_map[i].id;
        }
    }
    return -1;
}

/*
    Sets all the pieces to their starting position and alive
    No return
*/
void reset_board(board_s* board) {
    for(int i = PAWN0; i < ROOK8; i++) {
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

    for(int i = 0; i < NUM_PIECES; i++) {
        board->player[WHITE][i].is_alive = 1;
        board->player[BLACK][i].is_alive = 1;
    }

    for(int i = 0; i < NUM_COL; i++) {
        col_map[i].id = i;
    }
}

// Original reset board above - test reset board under
// void reset_board(board_s* board) {
//     for(int i = 0; i < NUM_PIECES; i++) {
//         board->player[WHITE][i].is_alive = false;
//         board->player[BLACK][i].is_alive = false;
//     }

//     board->player[WHITE][KNIGHT10].piece_type = KNIGHT10;
//     board->player[WHITE][KNIGHT10].pos.x = 5;
//     board->player[WHITE][KNIGHT10].pos.y = d;
//     board->player[WHITE][KNIGHT10].is_alive = true;

//     board->player[BLACK][KNIGHT11].piece_type = KNIGHT11;
//     board->player[BLACK][KNIGHT11].pos.x = 1;
//     board->player[BLACK][KNIGHT11].pos.y = d;
//     board->player[BLACK][KNIGHT11].is_alive = true;

//     board->player[WHITE][QUEEN].is_alive = false;
//     board->player[BLACK][QUEEN].is_alive = false;

//     board->player[WHITE][KING].piece_type = KING;
//     board->player[WHITE][KING].pos.x = 2;
//     board->player[WHITE][KING].pos.y = a;
//     board->player[WHITE][KING].is_alive = true;

//     board->player[BLACK][KING].piece_type = KING;
//     board->player[BLACK][KING].pos.x = 4;
//     board->player[BLACK][KING].pos.y = a;
//     board->player[BLACK][KING].is_alive = true;
//     for(int i = 0; i < NUM_COL; i++) {
//         col_map[i].id = i;
//     }
// }

/*
    Stores information of every piece in a txt file (opens/creates the file)
    No return
*/
void save_game(board_s* board, FILE* file, int current_player) {
    file = fopen("saved_game", "w");
    if(file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%d-", current_player);

    for(int piece = PAWN0; piece < NUM_PIECES; piece++) {
        fprintf(
            file,
            "%u-%u-%d-%d-",
            board->player[WHITE][piece].piece_type,
            board->player[WHITE][piece].pos.x,
            board->player[WHITE][piece].pos.y,
            board->player[WHITE][piece].is_alive);

        fprintf(
            file,
            "%u-%u-%d-%d-",
            board->player[BLACK][piece].piece_type,
            board->player[BLACK][piece].pos.x,
            board->player[BLACK][piece].pos.y,
            board->player[BLACK][piece].is_alive);
    }

    fclose(file);
}

/*
    Retrieves the the txt save file (closes the file afterwards)
    Stores the string of information into a buffer
    Return : (by*)Buffer
*/
void load_game(board_s* board, FILE* file, int current_player) {
    file = fopen("saved_game", "r");
    if(file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char buffer[500];
    while(fgets(buffer, sizeof(buffer), file) != NULL) {
        printf(" %s", buffer);
    }
    printf("\nFormat is the following : piece_type-x-y-alive");

    set_board_from_save(board, buffer, &current_player);
    start_game(board, file, current_player);
    fclose(file);
}

/*
    Parses the given buffer
    Sets board's pieces based on the parsed information
    No return
*/
void set_board_from_save(board_s* board, char* buffer, int* current_player) {
    char* player_turn = strtok(buffer, "-");
    *current_player = atoi(player_turn);
    printf(" %s", player_turn);
    for(int piece = PAWN0; piece < NUM_PIECES; piece++) {
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

/*
    Checks if the destination is the same as current position
    That would result in skipping a turn, illegal move!
    Return : True if it is the same position, false if not
*/
bool is_destination_current_position(board_s* board, char* list_id, int current_player) {
    if(list_id[ROW_ID] == board->player[current_player][(int)list_id[PIECE_ID]].pos.x &&
       list_id[COL_ID] == board->player[current_player][(int)list_id[PIECE_ID]].pos.y) {
        printf("\nYour destination has to be different from your current position");
        return true;
    }
    return false;
}

/*
    Checks if the king is checked by any opponent piece
    Kings are not allowed to check each other
    (this fonction does not check if king is checking opponnent)
    Return : true if king in check, false if not
*/
bool is_my_king_checked(board_s* board, int current_player) {
    int opponent = WHITE;
    if(current_player == WHITE) opponent = BLACK;

    int king_x = board->player[current_player][KING].pos.x;
    int king_y = board->player[current_player][KING].pos.y;

    // Checks if checked by pawn
    for(int pawn = PAWN0; pawn <= PAWN7; pawn++) {
        if(board->player[opponent][pawn].pos.x == king_x + 1 &&
           abs(king_y - board->player[opponent][pawn].pos.y) == 1) {
            printf("\nKing checked by black pawn");
            return true;
        }
        if(board->player[opponent][pawn].pos.x == king_x - 1 &&
           abs(king_y - board->player[opponent][pawn].pos.y) == 1) {
            printf("\nKing checked by white pawn");
            return true;
        }
    }

    // Checks if checked by rook or by queen (row and col)
    int pieces[] = {ROOK8, ROOK9, QUEEN};
    for(int i = 0; i < 3; i++) {
        char pieces_to_king[] = {
            pieces[i], board->player[opponent][i].pos.x, board->player[opponent][i].pos.y};

        if(board->player[opponent][pieces[i]].pos.x == king_x &&
           !is_row_blocked(board, pieces_to_king, current_player)) {
            printf("\nKing checked by opponent's %s (row)", piece_map[pieces[i]].name);
            return true;
        }
        if(board->player[opponent][pieces[i]].pos.y == king_y &&
           !is_col_blocked(board, pieces_to_king, current_player)) {
            printf("\nKing checked by opponent's %s (col)", piece_map[pieces[i]].name);
            return true;
        }
    }

    // Checks if checked by bishop or by queen (diagonal)
    for(int id = BISHOP12; id <= QUEEN; id++) {
        char piece_to_king[] = {
            id, board->player[opponent][id].pos.x, board->player[opponent][id].pos.y};

        if((abs(board->player[opponent][id].pos.x - king_x) ==
            abs(board->player[opponent][id].pos.y - king_y)) &&
           !is_diagonal_blocked(board, piece_to_king, opponent)) {
            printf("\nKing checked by opponent's %s (diagonal)", piece_map[id].name);
            return true;
        }
    }

    // Checks if checked by knight
    for(int knight = KNIGHT10; knight <= KNIGHT11; knight++) {
        int knight_x = board->player[opponent][knight].pos.x;
        int knight_y = board->player[opponent][knight].pos.y;
        if(abs(knight_x - king_x) == 2 && abs(knight_y - king_y) == 1) {
            printf("\nKing checked by opponent's %s", piece_map[knight].name);
            return true;
        }

        if(abs(knight_x - king_x) == 1 && abs(knight_y - king_y) == 2) {
            printf("\nKing checked by opponent's %s", piece_map[knight].name);
            return true;
        }
    }
    return false;
}

/*
    Determines which piece is selected
    Calls the fonctions checking the validity of said piece
    Returns : The return of the validity fonctions
    Retruns : True if the move of said piece is legal, else false
*/
bool piece_movement_validity(board_s* board, char* list_id, int current_player) {
    for(int pid = PAWN0; pid < ROOK8; pid++) {
        if(list_id[PIECE_ID] == pid) return is_pawn_move_legal(board, list_id, current_player);
    }
    if(list_id[PIECE_ID] == ROOK8 || list_id[PIECE_ID] == ROOK9)
        return is_rook_move_legal(board, list_id, current_player);
    if(list_id[PIECE_ID] == KNIGHT10 || list_id[PIECE_ID] == KNIGHT11)
        return is_knight_move_legal(board, list_id, current_player);
    if(list_id[PIECE_ID] == BISHOP12 || list_id[PIECE_ID] == BISHOP13)
        return is_bishop_move_legal(board, list_id, current_player);
    if(list_id[PIECE_ID] == QUEEN) return is_queen_move_legal(board, list_id, current_player);
    if(list_id[PIECE_ID] == KING) return is_king_move_legal(board, list_id, current_player);

    printf("\nValidity could not be verified");
    return false;
}

/*
    Determines if a piece exists along the row path;
    between the destination and current position
    Returns : True if there is a piece blocking the path
    Returns : False if nothing blocks the path
*/
bool is_row_blocked(board_s* board, char* list_id, int current_player) {
    int target_row = list_id[ROW_ID];
    int target_col = list_id[COL_ID];
    int current_row = board->player[current_player][(int)list_id[PIECE_ID]].pos.x;

    for(int i = PAWN0; i < NUM_PIECES; i++) {
        int direction = (target_row > current_row) ? 1 : -1;
        for(int row_i = 1; row_i < (direction * target_row) - (direction * current_row); row_i++) {
            if(board->player[WHITE][i].pos.x == current_row + (direction * row_i) &&
               target_col == board->player[WHITE][(int)list_id[PIECE_ID]].pos.y) {
                printf("\nRow blocked by white");
                return true;
            }
            if(board->player[BLACK][i].pos.x == current_row + (direction * row_i) &&
               target_col == board->player[BLACK][(int)list_id[PIECE_ID]].pos.y) {
                printf("\nRow blocked by black");
                return true;
            }
        }
    }
    return false;
}

/*
    Determines if a piece exists along the column path;
    between the destination and current position
    Returns : True if there is a piece blocking the path
    Returns : False if nothing blocks the path
*/
bool is_col_blocked(board_s* board, char* list_id, int current_player) {
    int target_row = list_id[ROW_ID];
    int target_col = list_id[COL_ID];
    int current_col = board->player[current_player][(int)list_id[PIECE_ID]].pos.y;

    for(int i = PAWN0; i < NUM_PIECES; i++) {
        int direction = (target_col > current_col) ? 1 : -1;
        for(int col_move = 1; col_move < (direction * target_col) - (direction * current_col);
            col_move++) {
            if(board->player[WHITE][i].pos.x == current_col + (direction * col_move) &&
               target_row == board->player[WHITE][(int)list_id[PIECE_ID]].pos.y) {
                printf("\nCol blocked by white");
                return true;
            }
            if(board->player[BLACK][i].pos.x == current_col + (direction * col_move) &&
               target_row == board->player[BLACK][(int)list_id[PIECE_ID]].pos.y) {
                printf("\nCol blocked by black");
                return true;
            }
        }
    }
    return false;
}

/*
    Determines if a piece exists along the diagnol path;
    between the destination and current position
    Returns : True if there is a piece blocking the path
*/
bool is_diagonal_blocked(board_s* board, char* list_id, int current_player) {
    int target_row = list_id[ROW_ID];
    int target_col = list_id[COL_ID];
    int current_row = board->player[current_player][(int)list_id[PIECE_ID]].pos.x;
    int current_col = board->player[current_player][(int)list_id[PIECE_ID]].pos.y;

    for(int piece = PAWN0; piece < NUM_PIECES; piece++) {
        int distance = abs(target_row - current_row);
        if(distance == 1) return false;
        for(int cells_to_check = 1; cells_to_check < abs(target_row - current_row);
            cells_to_check++) {
            int row_direction = (target_row > current_row) ? 1 : -1;
            int col_direction = (target_col > current_col) ? 1 : -1;
            if(board->player[WHITE][piece].pos.x ==
                   current_row + (cells_to_check * row_direction) &&
               board->player[WHITE][piece].pos.y ==
                   current_col + (cells_to_check * col_direction)) {
                return true;
            }
            if(board->player[BLACK][piece].pos.x ==
                   current_row + (cells_to_check * row_direction) &&
               board->player[BLACK][piece].pos.y ==
                   current_col + (cells_to_check * col_direction)) {
                return true;
            }
        }
    }
    return false;
}

/*
    Checks how the king moves and to where.
    Determines if the move is legal or not
    Returns : True if the king is allowed to move to its destination
*/
bool is_king_move_legal(board_s* board, char* list_id, int current_player) {
    int target_row = list_id[ROW_ID];
    int target_col = list_id[COL_ID];
    int current_row = board->player[current_player][(int)list_id[0]].pos.x;
    int current_col = board->player[current_player][(int)list_id[0]].pos.y;

    if((abs(target_col - current_col) == 1 || target_col - current_col == 0) &&
       (abs(target_row - current_row) == 1 || target_row - current_row == 0))
        return true;
    return false;
}

/*
    Checks how the queen moves and to where.
    Determines if the move is legal or not
    Returns : True if the queen is allowed to move to its destination
*/
bool is_queen_move_legal(board_s* board, char* list_id, int current_player) {
    int target_row = list_id[ROW_ID];
    int target_col = list_id[COL_ID];
    int current_row = board->player[current_player][(int)list_id[0]].pos.x;
    int current_col = board->player[current_player][(int)list_id[0]].pos.y;

    if(is_rook_move_legal(board, list_id, current_player) &&
       ((target_row == current_row && !is_row_blocked(board, list_id, current_player)) ||
        (target_col == current_col && !is_col_blocked(board, list_id, current_player))))
        return true;

    if(is_bishop_move_legal(board, list_id, current_player) &&
       !is_diagonal_blocked(board, list_id, current_player))
        return true;

    return false;
}

/*
    Checks how the Bishop moves and to where.
    Determines if the move is legal or not
    Returns : True if the bishop is allowed to move to its destination
*/
bool is_bishop_move_legal(board_s* board, char* list_id, int current_player) {
    int target_row = list_id[ROW_ID];
    int target_col = list_id[COL_ID];
    int current_row = board->player[current_player][(int)list_id[PIECE_ID]].pos.x;
    int current_col = board->player[current_player][(int)list_id[PIECE_ID]].pos.y;

    for(int piece = PAWN0; piece < NUM_PIECES; piece++) {
        int cells_moved = abs(target_col - current_col);
        int col_direction = (target_col > current_col) ? 1 : -1;
        int row_direction = (target_row > current_row) ? 1 : -1;

        if(target_col == current_col + cells_moved * col_direction &&
           target_row == current_row + cells_moved * row_direction && target_col >= a &&
           target_col <= NUM_COL - 1 && target_row >= 0 && target_row <= NUM_ROW - 1 &&
           !is_diagonal_blocked(board, list_id, current_player))
            return true;
    }
    return false;
}

/*
    Checks how the Knight moves and to where.
    Determines if the move is legal or not
    Returns : True if the Knight is allowed to move to its destination
*/
bool is_knight_move_legal(board_s* board, char* list_id, int current_player) {
    int knight_id = (int)list_id[PIECE_ID];
    int target_row = list_id[ROW_ID];
    int target_col = list_id[COL_ID];
    int current_row = board->player[current_player][knight_id].pos.x;
    int current_col = board->player[current_player][knight_id].pos.y;

    if((abs(target_row - current_row) == 2 && abs(target_col - current_col) == 1) ||
       (abs(target_row - current_row) == 1 && abs(target_col - current_col) == 2))
        if(!is_cell_occupied_by_ally(board, list_id, current_player)) return true;

    return false;
}

/*
    Checks how the Rook moves and to where.
    Determines if the move is legal or not
    Returns : True if the Rook is allowed to move to its destination
    Note : TEST THE HECK OF OUT THIS - NOT SURE IT'S GOING TO WORK AS INTENDED
*/
bool is_rook_move_legal(board_s* board, char* list_id, int current_player) {
    int rook_id = (int)list_id[PIECE_ID];
    int target_row = list_id[ROW_ID];
    int target_col = list_id[COL_ID];
    int current_row = board->player[current_player][rook_id].pos.x;
    int current_col = board->player[current_player][rook_id].pos.y;

    // It's a trap! If you stay on same col/row -> it means that you moved in that col/row
    if(!is_col_blocked(board, list_id, current_player)) {
        if(target_col == current_col && target_row != current_row) {
            if(!is_cell_occupied_by_ally(board, list_id, current_player) &&
               !is_cell_occupied_by_enemy(board, list_id, current_player))
                return true;
        }
    }
    if(!is_row_blocked(board, list_id, current_player)) {
        if(target_row == current_row && target_col != current_col) {
            if(!is_cell_occupied_by_ally(board, list_id, current_player) &&
               !is_cell_occupied_by_enemy(board, list_id, current_player))
                return true;
        }
    }
    return false;
}

/*
    Checks how the Pawn moves and to where.
    Determines if the move is legal or not
    Returns : True if the Pawn is allowed to move to its destination
*/
bool is_pawn_move_legal(board_s* board, char* list_id, int current_player) {
    int pawn_id = (int)list_id[PIECE_ID];
    int target_row = list_id[ROW_ID];
    int target_col = list_id[COL_ID];
    int current_row = board->player[current_player][pawn_id].pos.x;
    int current_col = board->player[current_player][pawn_id].pos.y;
    int direction = (current_player == WHITE) ? 1 : -1;

    // dinner time!
    if((target_col == current_col - 1 && target_col >= a &&
        target_row == current_row + direction) ||
       (target_col == current_col + 1 && target_col <= NUM_COL - 1 &&
        target_row == current_row + direction))
        if(!is_cell_occupied_by_ally(board, list_id, current_player)) return true;
    if(target_col != current_col) {
        printf("\nInvalid pawn move (cannot move column without capturing enemy)");
        return false;
    }

    // Check if it's the pawn's first move
    bool is_first_move = (current_player == WHITE && current_row == 1) ||
                         (current_player == BLACK && current_row == NUM_ROW - 2);

    // Check for two square move (only on first move)
    if(is_first_move && target_row == current_row + (2 * direction)) {
        if(!is_cell_occupied_by_ally(board, list_id, current_player) &&
           !is_cell_occupied_by_enemy(board, list_id, current_player))
            return true;
    }

    // Check for one square move
    if(target_row == current_row + direction) {
        if(!is_cell_occupied_by_ally(board, list_id, current_player) &&
           !is_cell_occupied_by_enemy(board, list_id, current_player))
            return true;
    }
    return false;
}
