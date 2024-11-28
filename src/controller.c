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
        // What does the player want to do
        char player_move[20]; // piece-row-col or tie, ff, save
        int list_id[NB_INPUTS];
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
        if(player_choice == FF) {
            printf("\n%s forfeits the game", player[current_player]);
            printf("\n%s wins!", player[!current_player]);
            return;
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
        if(is_checkmate(board, list_id, !current_player)) {
            printf("\n%s wins by checkmate!", player[current_player]);
            return;
        } else if(!piece_movement_validity(board, list_id, current_player)) {
            printf("\nInvalid %s move", piece_map[list_id[PIECE_ID]].name);
            continue;
        }

        if(is_king_checked(board, list_id, current_player)) {
            printf("\n%s is in check, make a move to protect the king", player[current_player]);
            continue;
        }

        // Everything validated!
        capture_enemy_piece(board, list_id, current_player);
        update_piece(board, current_player, list_id);
        move_count(board, list_id, current_player);
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
bool is_piece_selected_alive(board_s* board, int* list_id, int current_player) {
    return board->player[current_player][list_id[PIECE_ID]].is_alive;
}

/*
    Deletes a piece when an oponnent places one of their piece on top
    No return.
*/
void capture_enemy_piece(board_s* board, int* list_id, int current_player) {
    for(int pid = PAWN0; pid < NB_PIECES; pid++) {
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
bool is_cell_occupied_by_ally(board_s* board, int* list_id, int current_player) {
    for(int pid = PAWN0; pid < NB_PIECES; pid++) {
        if(board->player[current_player][pid].pos.x == list_id[ROW_ID] &&
           board->player[current_player][pid].pos.y == list_id[COL_ID] &&
           board->player[current_player][pid].is_alive == true) {
            if(list_id[PIECE_ID] != pid) return true;
        }
    }
    return false;
}

/*
    Checks if a cell is occupied by an enemy
    Useful to check if path is occupied for example
    Return : True if cell is occupied, false if not
*/
bool is_cell_occupied_by_enemy(board_s* board, int* list_id, int current_player) {
    for(int pid = PAWN0; pid < NB_PIECES; pid++) {
        if(board->player[!current_player][pid].pos.x == list_id[ROW_ID] &&
           board->player[!current_player][pid].pos.y == list_id[COL_ID] &&
           board->player[!current_player][pid].is_alive == true) {
            return true;
        }
    }

    return false;
}

/*
    Parses the player's input
    Prints the parsed info (for testing sake)
    Return : TIE / SAVE or START based on users first input
*/
int tokenise_player_choice(char* player_move, int* list_id) {
    char *selected_piece, *row, *col;

    selected_piece = strtok(player_move, "-");
    if(strcmp(selected_piece, "tie") == 0) return TIE;
    if(strcmp(selected_piece, "save") == 0) return SAVE;
    if((strcmp(selected_piece, "ff") == 0) || strcmp(selected_piece, "forfeit") == 0) return FF;
    col = strtok(NULL, "-"); // In chess we say letter then number
    row = strtok(NULL, "-"); // Which is actually : column then row

    list_id[PIECE_ID] = (int)get_piece_id(selected_piece);
    list_id[ROW_ID] = atoi(row) - 1;
    list_id[COL_ID] = get_col_id(col);

    printf(
        "\nPiece: %s, id: %d \nRow: %s w/ id: %d \nCol: %s w/ id: %d\n",
        selected_piece,
        list_id[PIECE_ID],
        row,
        list_id[ROW_ID],
        col,
        list_id[COL_ID]);
    return START;
}

/*
    Updates the piece with the player's input
    No return.
*/
void update_piece(board_s* board, int current_player, int* list_id) {
    board->player[current_player][list_id[PIECE_ID]].pos.x = list_id[ROW_ID];
    board->player[current_player][list_id[PIECE_ID]].pos.y = list_id[COL_ID];
}

/*
    Compares the name of the piece with the name given
    Return : ID of the piece
*/
int get_piece_id(const char* name) {
    for(int i = PAWN0; i < NB_PIECES; i++) {
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
    for(int i = 0; i < NB_ROW; i++) {
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
        board->player[BLACK][i].pos.x = NB_ROW - 2;
        board->player[BLACK][i].pos.y = i;
    }

    board->player[WHITE][ROOK8].piece_type = ROOK8;
    board->player[WHITE][ROOK8].pos.x = 0;
    board->player[WHITE][ROOK8].pos.y = 0;
    board->player[WHITE][ROOK9].piece_type = ROOK9;
    board->player[WHITE][ROOK9].pos.x = 0;
    board->player[WHITE][ROOK9].pos.y = NB_COL - 1;
    board->player[BLACK][ROOK8].piece_type = ROOK8;
    board->player[BLACK][ROOK8].pos.x = NB_ROW - 1;
    board->player[BLACK][ROOK8].pos.y = 0;
    board->player[BLACK][ROOK9].piece_type = ROOK9;
    board->player[BLACK][ROOK9].pos.x = NB_ROW - 1;
    board->player[BLACK][ROOK9].pos.y = NB_COL - 1;

    board->player[WHITE][KNIGHT10].piece_type = KNIGHT10;
    board->player[WHITE][KNIGHT10].pos.x = 0;
    board->player[WHITE][KNIGHT10].pos.y = 1;
    board->player[WHITE][KNIGHT11].piece_type = KNIGHT11;
    board->player[WHITE][KNIGHT11].pos.x = 0;
    board->player[WHITE][KNIGHT11].pos.y = NB_COL - 2;
    board->player[BLACK][KNIGHT10].piece_type = KNIGHT10;
    board->player[BLACK][KNIGHT10].pos.x = NB_ROW - 1;
    board->player[BLACK][KNIGHT10].pos.y = 1;
    board->player[BLACK][KNIGHT11].piece_type = KNIGHT11;
    board->player[BLACK][KNIGHT11].pos.x = NB_ROW - 1;
    board->player[BLACK][KNIGHT11].pos.y = NB_COL - 2;

    board->player[WHITE][BISHOP12].piece_type = BISHOP12;
    board->player[WHITE][BISHOP12].pos.x = 0;
    board->player[WHITE][BISHOP12].pos.y = 2;
    board->player[WHITE][BISHOP13].piece_type = BISHOP13;
    board->player[WHITE][BISHOP13].pos.x = 0;
    board->player[WHITE][BISHOP13].pos.y = NB_COL - 3;
    board->player[BLACK][BISHOP12].piece_type = BISHOP12;
    board->player[BLACK][BISHOP12].pos.x = NB_ROW - 1;
    board->player[BLACK][BISHOP12].pos.y = 2;
    board->player[BLACK][BISHOP13].piece_type = BISHOP13;
    board->player[BLACK][BISHOP13].pos.x = NB_ROW - 1;
    board->player[BLACK][BISHOP13].pos.y = NB_COL - 3;

    board->player[WHITE][QUEEN].piece_type = QUEEN;
    board->player[WHITE][QUEEN].pos.x = 0;
    board->player[WHITE][QUEEN].pos.y = NB_COL - 5;
    board->player[BLACK][QUEEN].piece_type = QUEEN;
    board->player[BLACK][QUEEN].pos.x = NB_ROW - 1;
    board->player[BLACK][QUEEN].pos.y = NB_COL - 5;

    board->player[WHITE][KING].piece_type = KING;
    board->player[WHITE][KING].pos.x = 0;
    board->player[WHITE][KING].pos.y = NB_COL - 4;
    board->player[BLACK][KING].piece_type = KING;
    board->player[BLACK][KING].pos.x = NB_ROW - 1;
    board->player[BLACK][KING].pos.y = NB_COL - 4;

    for(int i = 0; i < NB_PIECES; i++) {
        board->player[WHITE][i].is_alive = 1;
        board->player[BLACK][i].is_alive = 1;
        board->player[WHITE][i].times_moved = 0;
        board->player[BLACK][i].times_moved = 0;
    }

    for(int i = 0; i < NB_COL; i++) {
        col_map[i].id = i;
    }
}

// Original reset board above - test reset board under
// void reset_board(board_s* board) {
//     for(int i = 0; i < NB_PIECES; i++) {
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
//     for(int i = 0; i < NB_COL; i++) {
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

    for(int piece = PAWN0; piece < NB_PIECES; piece++) {
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
    for(int piece = PAWN0; piece < NB_PIECES; piece++) {
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
bool is_destination_current_position(board_s* board, int* list_id, int current_player) {
    if(list_id[ROW_ID] == board->player[current_player][list_id[PIECE_ID]].pos.x &&
       list_id[COL_ID] == board->player[current_player][list_id[PIECE_ID]].pos.y) {
        printf("\nYour destination has to be different from your current position");
        return true;
    }
    return false;
}

/*
    Determines which piece is selected
    Calls the fonctions checking the validity of said piece
    Returns : The return of the validity fonctions
    Retruns : True if the move of said piece is legal, else false
*/
bool piece_movement_validity(board_s* board, int* list_id, int current_player) {
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

    return false;
}

bool is_checkmate(board_s* board, int* list_id, int current_player) {
    if(is_king_checked(board, list_id, current_player)) {
        if(!piece_movement_validity(board, list_id, current_player)) {
            printf("\nCheckmate!");
            return true;
        }
    }
    return false;
}

/*
    Checks if after the player's move, the king will be checked
    Helps determine legal moves
    Returns : True if the king will be checked
*/
bool will_king_be_checked(board_s* board, int* list_id, int current_player) {
    for(int id = PAWN0; id < NB_PIECES; id++) {
        // For every enemy piece
        int attacking_king[3] = {
            id,
            board->player[!current_player][id].pos.x,
            board->player[!current_player][id].pos.y};
        // Path the enemy piece takes to hit the landing position of king
        int path_to_king[3] = {id, list_id[ROW_ID], list_id[COL_ID]};
        // Can that enemy actually hit the king?
        if(is_cell_occupied_by_enemy(board, attacking_king, current_player)) {
            if(piece_movement_validity(board, path_to_king, current_player)) {
                return true;
            }
        }
    }
    return false;
}

/*
    Checks if the king is checked or not
    Returns : True if the king is in danger!
*/
bool is_king_checked(board_s* board, int* list_id, int current_player) {
    for(int id = PAWN0; id < NB_PIECES; id++) {
        // For every enemy piece
        int attacking_king[3] = {
            id,
            board->player[!current_player][id].pos.x,
            board->player[!current_player][id].pos.y};
        // Path the enemy piece takes to hit the current position of the king
        int path_to_king[3] = {
            id,
            board->player[current_player][KING].pos.x,
            board->player[current_player][KING].pos.y};
        // Can that enemy actually hit the king?
        if(is_cell_occupied_by_enemy(board, attacking_king, current_player)) {
            if(piece_movement_validity(board, path_to_king, current_player)) {
                return true;
            }
        }
    }
    return false;
}

/*
    Checks if castling is legal
    King moves 2 squares, king and rook haven't moved, 
    king is not and won't be checked, nothing in the path
    Returns : True if castling is legal
*/
bool is_castling_legal(board_s* board, int* list_id, int current_player) {
    // To the left (rook moves 3 squares)
    if(list_id[COL_ID] == c && list_id[PIECE_ID] == KING &&
       abs(list_id[COL_ID] - board->player[current_player][list_id[PIECE_ID]].pos.y) == 2 &&
       board->player[current_player][KING].times_moved == 0 &&
       board->player[current_player][ROOK8].times_moved == 0 &&
       !is_king_checked(board, list_id, current_player)) {
        int king_path[3] = {KING, 0, d}; // The king cannot be in check on it's path
        int full_path[3] = {ROOK8, 0, e}; // Check if path is blocked between king and rook
        if(!will_king_be_checked(board, list_id, current_player) &&
           !will_king_be_checked(board, king_path, current_player) &&
           !is_king_checked(board, list_id, current_player) &&
           !is_cell_occupied_by_ally(board, full_path, current_player) &&
           !is_cell_occupied_by_enemy(board, full_path, current_player)) {
            board->player[current_player][ROOK8].pos.y = d;
            return true;
        }
    }

    // To the right (rook moves 2 squares)
    if(list_id[COL_ID] == g && list_id[PIECE_ID] == KING &&
       abs(list_id[COL_ID] - board->player[current_player][list_id[PIECE_ID]].pos.y) == 2 &&
       board->player[current_player][KING].times_moved == 0 &&
       board->player[current_player][ROOK9].times_moved == 0 &&
       !is_king_checked(board, list_id, current_player)) {
        int king_path[3] = {KING, 0, f}; // The king cannot be in check on it's path
        int full_path[3] = {ROOK8, 0, e}; // Check if path is blocked between king and rook
        if(!will_king_be_checked(board, list_id, current_player) &&
           !will_king_be_checked(board, king_path, current_player) &&
           !is_king_checked(board, list_id, current_player) &&
           !is_cell_occupied_by_ally(board, full_path, current_player) &&
           !is_cell_occupied_by_enemy(board, full_path, current_player)) {
            board->player[current_player][ROOK9].pos.y = f;
            return true;
        }
    }
    return false;
}

/*
    Checks if the move of the king is legal
    Pattern ok, destination empty/enemy, or castling is valid 
    Returns : True if the move of the king is legal
*/
bool is_king_move_legal(board_s* board, int* list_id, int current_player) {
    if((is_king_pattern_valid(board, list_id, current_player) &&
        !is_cell_occupied_by_ally(board, list_id, current_player) &&
        !will_king_be_checked(board, list_id, current_player)) ||
       is_castling_legal(board, list_id, current_player)) {
        return true;
    }
    printf("\nillegal king move");
    return false;
}

/*
    Checks if the pattern of the king is valid
    Returns : True if the pattern is valid
*/
bool is_king_pattern_valid(board_s* board, int* list_id, int current_player) {
    if(abs(list_id[ROW_ID] - board->player[current_player][list_id[PIECE_ID]].pos.x) == 1 ||
       abs(list_id[COL_ID] - board->player[current_player][list_id[PIECE_ID]].pos.y) == 1) {
        return true;
    }
    return false;
}

/*
    Checks if the move of the knight is legal
    Pattern ok, destination empty or enemy
    Returns : True if the move of the knight is legal
*/
bool is_knight_move_legal(board_s* board, int* list_id, int current_player) {
    if(is_knight_pattern_valid(board, list_id, current_player) &&
       !is_cell_occupied_by_ally(board, list_id, current_player) &&
       !will_king_be_checked(board, list_id, current_player)) {
        return true;
    }
    printf("\nillegal knight move");
    return false;
}

/*
    Checks if the pattern of the knight is valid
    Returns : True if the pattern is valid
*/
bool is_knight_pattern_valid(board_s* board, int* list_id, int current_player) {
    if((abs(list_id[ROW_ID] - board->player[current_player][list_id[PIECE_ID]].pos.x) == 2 &&
        abs(list_id[COL_ID] - board->player[current_player][list_id[PIECE_ID]].pos.y) == 1) ||
       (abs(list_id[ROW_ID] - board->player[current_player][list_id[PIECE_ID]].pos.x) == 1 &&
        abs(list_id[COL_ID] - board->player[current_player][list_id[PIECE_ID]].pos.y) == 2)) {
        return true;
    }
    return false;
}

/*
    Checks if the move of the queen is legal, using rook and bishop pattern
    Pattern ok, path clear, destination empty/enemy
    Returns : True if the move of the queen is legal
*/
bool is_queen_move_legal(board_s* board, int* list_id, int current_player) {
    if((is_rook_pattern_valid(board, list_id, current_player) ||
        is_bishop_pattern_valid(board, list_id, current_player)) &&
       !is_cell_occupied_by_ally(board, list_id, current_player) &&
       !will_king_be_checked(board, list_id, current_player)) {
        return true;
    }
    printf("\nillegal queen move");
    return false;
}

/*
    Checks if the move of the bishop is legal
    Pattern ok, path clear, destination empty/enemy
    Returns : True if the move of the bishop is legal
*/
bool is_bishop_move_legal(board_s* board, int* list_id, int current_player) {
    if(is_bishop_pattern_valid(board, list_id, current_player) &&
       !is_cell_occupied_by_ally(board, list_id, current_player) &&
       !will_king_be_checked(board, list_id, current_player)) {
        return true;
    }
    printf("\nillegal bishop move");
    return false;
}

/*
    Checks if the pattern of the bishop is valid (including if path clear)
    Returns : True if the pattern is valid
*/
bool is_bishop_pattern_valid(board_s* board, int* list_id, int current_player) {
    // Moving on the same diagonal
    if(abs(list_id[ROW_ID] - board->player[current_player][list_id[PIECE_ID]].pos.x) ==
           abs(list_id[COL_ID] - board->player[current_player][list_id[PIECE_ID]].pos.y) &&
       !is_diagonal_blocked(board, list_id, current_player)) {
        return true;
    }
    return false;
}

/*
    Checks if the move of the rook is legal
    Pattern ok, path clear, destination empty/enemy, or castling valid
    Returns : True if the move of the rook is legal
*/
bool is_rook_move_legal(board_s* board, int* list_id, int current_player) {
    if((is_rook_pattern_valid(board, list_id, current_player) &&
        !is_cell_occupied_by_ally(board, list_id, current_player) &&
        !will_king_be_checked(board, list_id, current_player)) ||
       is_castling_legal(board, list_id, current_player)) {
        return true;
    }
    printf("\nillegal rook move");
    return false;
}

/*
    Checks if the patter of the rook is valid
    Returns : True if it is valid
*/
bool is_rook_pattern_valid(board_s* board, int* list_id, int current_player) {
    // Moving on the same row
    if(list_id[ROW_ID] == board->player[current_player][list_id[PIECE_ID]].pos.x &&
       list_id[COL_ID] != board->player[current_player][list_id[PIECE_ID]].pos.y &&
       !is_row_blocked(board, list_id, current_player)) {
        return true;

        // Moving on the same column
    } else if(
        list_id[ROW_ID] != board->player[current_player][list_id[PIECE_ID]].pos.x &&
        list_id[COL_ID] == board->player[current_player][list_id[PIECE_ID]].pos.y &&
        !is_col_blocked(board, list_id, current_player)) {
        return true;
    }
    return false;
}

/*
    Checks if the move of the pawn is legal
    Returns : True if the move of the pawn is legal
*/
bool is_pawn_move_legal(board_s* board, int* list_id, int current_player) {
    if((can_pawn_advance(board, list_id, current_player) ||
        can_pawn_eat(board, list_id, current_player) ||
        is_en_passant_legal(board, list_id, current_player)) &&
       !will_king_be_checked(board, list_id, current_player)) {
        return true;
    }
    printf("\nillegal pawn move");
    return false;
}

/*
    Checks if the pawn can advance
    Returns : True if the pawn can advance
*/
bool can_pawn_advance(board_s* board, int* list_id, int current_player) {
    if(board->player[current_player][list_id[PIECE_ID]].times_moved == 0 &&
       current_player == WHITE) {
        // White can move twice on first turn
        if((list_id[ROW_ID] == board->player[current_player][list_id[PIECE_ID]].pos.x + 1 ||
            list_id[ROW_ID] == board->player[current_player][list_id[PIECE_ID]].pos.x + 2) &&
           list_id[COL_ID] == board->player[current_player][list_id[PIECE_ID]].pos.y &&
           !is_cell_occupied_by_enemy(board, list_id, current_player) &&
           !is_cell_occupied_by_ally(board, list_id, current_player) &&
           !is_col_blocked(board, list_id, current_player)) {
            return true;
        }
    } else if(
        board->player[current_player][list_id[PIECE_ID]].times_moved != 0 &&
        current_player == WHITE) {
        if(list_id[ROW_ID] == board->player[current_player][list_id[PIECE_ID]].pos.x + 1 &&
           list_id[COL_ID] == board->player[current_player][list_id[PIECE_ID]].pos.y &&
           !is_cell_occupied_by_enemy(board, list_id, current_player) &&
           !is_cell_occupied_by_ally(board, list_id, current_player)) {
            return true;
        }
    }
    if(board->player[current_player][list_id[PIECE_ID]].times_moved == 0 &&
       current_player == BLACK) {
        // Black can move twice on first turn
        if((list_id[ROW_ID] == board->player[current_player][list_id[PIECE_ID]].pos.x - 1 ||
            list_id[ROW_ID] == board->player[current_player][list_id[PIECE_ID]].pos.x - 2) &&
           list_id[COL_ID] == board->player[current_player][list_id[PIECE_ID]].pos.y &&
           !is_cell_occupied_by_enemy(board, list_id, current_player) &&
           !is_cell_occupied_by_ally(board, list_id, current_player) &&
           !is_col_blocked(board, list_id, current_player)) {
            return true;
        }
    } else if(
        board->player[current_player][list_id[PIECE_ID]].times_moved != 0 &&
        current_player == BLACK) {
        if(list_id[ROW_ID] == board->player[current_player][list_id[PIECE_ID]].pos.x - 1 &&
           list_id[COL_ID] == board->player[current_player][list_id[PIECE_ID]].pos.y &&
           !is_cell_occupied_by_enemy(board, list_id, current_player) &&
           !is_cell_occupied_by_ally(board, list_id, current_player)) {
            return true;
        }
    }
    return false;
}

/*
    Checks if the pawn can eat an enemy piece
    Returns : True if the pawn can eat an enemy piece
*/
bool can_pawn_eat(board_s* board, int* list_id, int current_player) {
    if(current_player == WHITE) {
        if(list_id[ROW_ID] == board->player[current_player][list_id[PIECE_ID]].pos.x + 1 &&
           (list_id[COL_ID] == board->player[current_player][list_id[PIECE_ID]].pos.y + 1 ||
            list_id[COL_ID] == board->player[current_player][list_id[PIECE_ID]].pos.y - 1) &&
           is_cell_occupied_by_enemy(board, list_id, current_player)) {
            return true;
        }
    }
    if(current_player == BLACK) {
        if(list_id[ROW_ID] == board->player[current_player][list_id[PIECE_ID]].pos.x - 1 &&
           (list_id[COL_ID] == board->player[current_player][list_id[PIECE_ID]].pos.y + 1 ||
            list_id[COL_ID] == board->player[current_player][list_id[PIECE_ID]].pos.y - 1) &&
           is_cell_occupied_by_enemy(board, list_id, current_player)) {
            return true;
        }
    }

    return false;
}

/*
    Checks if the enemy pawn is in a position to be eaten by en passant
    Checks if the player's pawn can eat the enemy pawn
    If all ok - applies en passant
    Returns : True if there is an en passant
*/
bool is_en_passant_legal(board_s* board, int* list_id, int current_player) {
    // White pawn is in position to eat en passant
    if(board->player[WHITE][list_id[PIECE_ID]].pos.x == 4 && list_id[ROW_ID] == 5) {
        // Black is in position to be eaten en passant
        if(board->player[BLACK][list_id[PIECE_ID]].pos.y == list_id[COL_ID] &&
           board->player[BLACK][list_id[PIECE_ID]].times_moved == 1) {
            // Enjoy your meal
            board->player[BLACK][list_id[PIECE_ID]].is_alive = 0;
            return true;
        }
        // Black pawn is in position to eat en passant
    } else if(board->player[BLACK][list_id[PIECE_ID]].pos.x == 3 && list_id[ROW_ID] == 2) {
        // White is in position to be eaten en passant
        if(board->player[WHITE][list_id[PIECE_ID]].pos.y == list_id[COL_ID] &&
           board->player[WHITE][list_id[PIECE_ID]].times_moved == 1) {
            // Enjoy your meal
            board->player[WHITE][list_id[PIECE_ID]].is_alive = 0;
            return true;
        }
    }

    return false;
}

/*
    Counts how many times a piece has moved since the start of the game
    For en passant and castling
*/
void move_count(board_s* board, int* list_id, int current_player) {
    board->player[current_player][list_id[PIECE_ID]].times_moved++;
}

/*
    Determines if a piece exists along the row path;
    between the destination and current position
    Returns : True if there is a piece blocking the path
    Returns : False if nothing blocks the path
*/
bool is_row_blocked(board_s* board, int* list_id, int current_player) {
    int target_row = list_id[ROW_ID];
    int target_col = list_id[COL_ID];
    int current_row = board->player[current_player][list_id[PIECE_ID]].pos.x;

    for(int i = PAWN0; i < NB_PIECES; i++) {
        int direction = (target_row > current_row) ? 1 : -1;
        for(int row_i = 1; row_i < (direction * target_row) - (direction * current_row); row_i++) {
            if(board->player[WHITE][i].pos.x == current_row + (direction * row_i) &&
               target_col == board->player[WHITE][list_id[PIECE_ID]].pos.y) {
                printf("\nRow blocked by white");
                return true;
            }
            if(board->player[BLACK][i].pos.x == current_row + (direction * row_i) &&
               target_col == board->player[BLACK][list_id[PIECE_ID]].pos.y) {
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
bool is_col_blocked(board_s* board, int* list_id, int current_player) {
    int target_row = list_id[ROW_ID];
    int target_col = list_id[COL_ID];
    int current_col = board->player[current_player][list_id[PIECE_ID]].pos.y;

    for(int i = PAWN0; i < NB_PIECES; i++) {
        int direction = (target_col > current_col) ? 1 : -1;
        for(int col_move = 1; col_move < (direction * target_col) - (direction * current_col);
            col_move++) {
            if(board->player[WHITE][i].pos.x == current_col + (direction * col_move) &&
               target_row == board->player[WHITE][list_id[PIECE_ID]].pos.y) {
                printf("\nCol blocked by white");
                return true;
            }
            if(board->player[BLACK][i].pos.x == current_col + (direction * col_move) &&
               target_row == board->player[BLACK][list_id[PIECE_ID]].pos.y) {
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
bool is_diagonal_blocked(board_s* board, int* list_id, int current_player) {
    int target_row = list_id[ROW_ID];
    int target_col = list_id[COL_ID];
    int current_row = board->player[current_player][list_id[PIECE_ID]].pos.x;
    int current_col = board->player[current_player][list_id[PIECE_ID]].pos.y;

    for(int piece = PAWN0; piece < NB_PIECES; piece++) {
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
