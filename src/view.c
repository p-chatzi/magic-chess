#include <stdio.h> // printf()

#include "view.h"
#include "model.h"

// =======================================================================
// io
int choose_main_menu()
{
    int choice = -1;
    scanf("%d", &choice);
    return choice;
}

// int choose_color()
// {
//     char color;
//     printf("\nPlayer, now is your time to pick your color (w: white | b:black):");
//     scanf("%c", &color);
//     return color;
// }

// =======================================================================
// Purely outputs
void print_board(board_s *board)
{
    print_test_pieces(&board->player[BLACK], BLACK);
    print_test_pieces(&board->player[WHITE], WHITE);
}

void print_test_pieces(pieces_s *pieces, player_e player)
{
    for (int i = 0; i < NUM_PIECES; i++)
    {
        printf("\n\nPlayer %d, piece % d, type = %d", player, i, pieces[i].piece_type);
        printf("\nPlayer %d, piece % d, x = %d", player, i, pieces[i].pos.x);
        printf("\nPlayer %d, piece % d, y = %d", player, i, pieces[i].pos.y);
        printf("\nPlayer %d, piece % d, alive = %d", player, i, pieces[i].is_alive);
    }
}

void print_main_menu()
{
    printf("\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿          Magic CHESS          ⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿        1. Start New Game      ⣿\n");
    printf("⣿        2. Invent Pieces       ⣿\n");
    printf("⣿        3. Rules               ⣿\n");
    printf("⣿        4. Settings            ⣿\n");
    printf("⣿        5. Exit                ⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("  Please choose 1 to 5: \n");
}

void print_byecat()
{
    printf("\n  Bye bye, see you soon :3   \n");
    printf("       					 ⢀⡀⠀⠀⠀⠀ \n");
    printf("⠀⠀⠀⠀⢀⡴⣆⠀⠀⠀⠀⠀⣠⡀⠀⠀⠀⠀⠀⠀⣼⣿⡗⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⣠⠟⠀⠘⠷⠶⠶⠶⠾⠉⢳⡄⠀⠀⠀⠀⠀⣧⣿⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⣰⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣤⣤⣤⣤⣤⣿⢿⣄⠀⠀⠀⠀\n");
    printf("⠀⠀⡇⠀⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣧⠀⠀⠀⠀⠀⠀⠙⣷⡴⠶⣦\n");
    printf("⠀⠀⢱⡀⠀⠉⠉⠀⠀⠀⠀⠛⠃⠀⢠⡟⠂⠀⠀⢀⣀⣠⣤⠿⠞⠛⠋\n");
    printf("⣠⠾⠋⠙⣶⣤⣤⣤⣤⣤⣀⣠⣤⣾⣿⠴⠶⠚⠋⠉⠁⠀⠀⠀⠀⠀⠀\n");
    printf("⠛⠒⠛⠉⠉⠀⠀⠀⣴⠟⣣⡴⠛⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠛⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀			 \n");
}

void print_hellowep()
{
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   	  \n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡶⠟⠛⠻⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    \n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠞⠁⠀⠀⠀⠀⢹⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡞⠁⣀⣠⣤⣤⣄⡼⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⠀⠀⣠⡾⠏⠀⡿⣿⣧⡶⣃⣾⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⢿⡇⢀⣄⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⢀⣀⣀⣀⣀⣀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣣⣾⣡⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣷⡀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡿⠁⣾⣧⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⠆⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⣦⡀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠈⠻⣾⣯⣿⡏⣲⣿⣿⣿⣿⣿⣿⣿⣿⠟⣽⢏⣾⠿⡾⠁⢸⡿⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣏⠉⡛⡆\n");
    printf("⠀⠀⠀⠀⠀⠀⣀⡀⠀⣠⣿⣶⣿⣿⣿⣿⢹⡾⣿⡟⠃⠀⢇⣿⠃⠀⠀⠀⢸⡇⣼⣿⣿⣿⣿⣿⣟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⣤⡄\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⣠⣾⣯⣿⣿⣿⣿⣿⣿⣿⣷⠟⠳⠶⠾⠛⠁⠀⠀⠀⠀⢸⡇⠈⢿⣿⣿⣿⣿⣿⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⢀⣼⠿⠛⣽⣿⣿⣿⣿⣿⣿⣿⡏⠀⣠⣴⣾⣿⣃⠀⠀⠀⠀⢸⡇⠀⢘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣤⣤\n");
    printf("⠀⠀⠀⠀⠀⠘⠁⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⢠⡾⢻⢋⣿⣶⣻⠀⠀⠀⠀⠈⣷⡀⠸⠋⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠉⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⡇⢺⡇⢸⣿⣽⣿⡇⠀⠀⠀⠀⠀⠘⢿⣄⣷⣾⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣿⣿⣿⣿⢿⣷⠀⠑⠈⠛⣿⠛⠀⠀⠀⠀⠀⠀⠀⠀⠙⠻⡿⣿⢭⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⡆⣹⡗⠙⢢⠀⠉⠀⠀⠀⠰⠆⠀⠀⠀⠀⠀⠀⠉⠋⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠹⠿⠋⢻⣿⣿⣾⡇⠀⠈⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⣀⣾⠟⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡤⠤⢤⣽⣿⣿⣷⡀⠀⢹⡄⠀⠀⣀⣀⡤⠤⠴⠚⠉⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡼⠁⠀⢀⣴⣉⣙⣻⣿⣦⣀⠙⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⡿⠉⠿⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⢀⡴⠻⠿⠗⠒⠊⢉⣠⠤⠀⠀⠀⠙⠛⢿⡒⢭⣤⣄⣀⡀⢀⣀⣀⣤⣶⣾⡿⢿⡋⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⡞⠀⠀⠀⢀⡤⠚⠉⠀⠀⠀⠀⠀⠀⠀⠀⠙⢦⡀⠀⠈⠉⠁⡼⣿⡿⢋⡽⠀⠀⣟⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⣀⣀⣀⣀⣀⣀⣼⠁⣀⣤⡶⠛⠒⠒⠲⠤⣄⡀⠀⠀⢀⣀⠀⠀⠀⠙⢦⡀⠀⠀⣿⣋⡴⠋⠀⢀⣠⠟⢮⣣⢀⣀⣀⣀⣀      \n");
}

void print_rules()
{
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿                            Piece Movements                                  ⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿ K - King:   1 square any direction                                          ⣿\n");
    printf("⣿ Q - Queen:  Any number of squares diagonally, horizontally, or vertically   ⣿\n");
    printf("⣿ R - Rook:   Any number of squares horizontally or vertically                ⣿\n");
    printf("⣿ B - Bishop: Any number of squares diagonally                                ⣿\n");
    printf("⣿ N - Knight: L-shape (2 squares in one direction, then 1 perpendicular)      ⣿\n");
    printf("⣿ P - Pawn:   1 square forward (2 on first move), captures diagonally         ⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿                            Special Rules                                    ⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿   Castling: King + Rook swap positions under certain conditions             ⣿\n");
    printf("⣿   En Passant: Special pawn capture                                          ⣿\n");
    printf("⣿   Pawn Promotion: Pawn reaching opposite end becomes any piece except King  ⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿                             Winning                                         ⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿   Checkmate: King is in check and cannot escape                             ⣿\n");
    printf("⣿   Resignation: Player forfeits                                              ⣿\n");
    printf("⣿   Stalemate: No legal moves, but not in check (draw)                        ⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿                           Other Draw Conditions                             ⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿   Mutual agreement                                                          ⣿\n");
    printf("⣿   Insufficient material                                                     ⣿\n");
    printf("⣿   Threefold repetition                                                      ⣿\n");
    printf("⣿   50-move rule                                                              ⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
}
/*WIP*/
void print_settings()
{
    print_wip();
}

void print_wip()
{
    printf(
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⠟⠻⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⠀⠀⠈⠻⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⣶⣦⡀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡿⡇⠀⠀⠀⠀⠈⠙⢷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   ⣀⣤⠾⠋⠁⢸⣿⡇⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣇⡇⠀⠀⠀⠀⠀⠀⠀⠙⢷⣆⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⡾⠛⠁⠀⠀⠀⣿⣼⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡏⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⡾⠛⠁⠀⠀⠀⠀⠀⣸⡿⣿⠂⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣇⣿⠀⠀⠀⠀⠀⠶⠶⠶⠶⠶⠶⠿⠷⠶⠶⠤⣤⣤⣀⣀⡀⢀⣤⡾⠛⠁⠀⠀⠀⠀⠀⠀⠀⢠⣿⢣⡟⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⣽⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⡷⣸⠇⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⢣⡿⠁⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀W.I.P⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣼⠃⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⠇⠀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡏⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣿⣿⡾⠛⠉⣉⣽⣿⣶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⠶⠛⢛⣿⣿⣷⣶⣤⣀⠀⠀⠀⠀⠀⠀⢸⣿⡀⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⢰⣾⠛⢉⣵⡟⣃⣤⣶⣿⣿⣿⣿⣿⣿⣷⡄⠀⠀⠀⠀⠀⣠⣾⠏⣡⣴⣾⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀⠀⠀⠀⢈⡹⣇⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⠀⠀⠙⣷⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣀⣀⣀⣀⣀⣰⣿⣷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠶⠖⠲⠾⣿⣿⣦⠀⠀⠀⠀⠀\n"
        "⠀⠀⠀⠀⣠⣴⡾⠋⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠛⠻⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⠀⠀⠀⠀⠈⠙⢿⣄⠀⠀⠀⠀\n"
        "⠀⠀⣿⡛⠉⠁⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢷⣄⠀⠀\n"
        "⠀⠀⣾⣷⣦⣀⠀⠀⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠏⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣧⠀\n"
        "⠀⡀⠈⠻⣿⣿⣷⠆⠀⠙⠻⠿⣿⣿⡿⢿⣿⠋⠀⠀⠀⣴⠇⠀⠀⠀⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⡆\n"
        "⠀⠻⣟⠛⠛⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠿⣿⣆⣀⣠⣼⢿⣧⠀⠀⠀⢀⣿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⠿⣛⠹⣮⣿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⣷\n"
        "⠀⠀⠈⠻⢦⣤⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⢩⠿⠻⣯⢻⣷⣶⣿⡿⠋⠀⠀⠀⠉⠉⠉⠉⠁⠀⣐⣭⣾⡿⠋⢻⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿\n"
        "⠀⠀⠀⢀⣰⣿⣻⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⡿⠛⣍⠡⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡟\n"
        "⠀⠀⠀⠛⣿⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⡾⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡿⠁\n"
        "⠀⠀⠀⢐⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠟⠀⠀\n"
        "⠀⠀⠀⣼⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡾⠃⠀⠀⠀\n"
        "⠀⠀⠀⣸⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣶⡟⠀⠀⠀⠀⠀\n"
        "⠀⠀⣰⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⠛⠀⠀⠀⠀⠀⠀\n"
        "⢠⣾⢿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⡏⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⣰⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣶⣿⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⣾⢿⣾⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠛⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⢀⣾⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
}

void clear_terminal()
{
    printf("\033[2J\033[H");
}

// Pieces drawn in ascii
/*
void print_queen()
{
    printf(
        "	  o   o      "\n
        " o   /\ /\  o   "\n
        " \`.'  `  `'/   "\n
        "  \    Q   /     "\n
        "	\_.--._/       "\n
        "	'.____.'       "\n);
}
  _   _
 | |_| |
 |  R  |
 '-----'
 |     |
/_.---._\
'._____.'


    .-"-.
    `.P.'
    .' '.
   .'___'.
   `-----'

          / \
         ( B )
          \ /
          | |
        ._' '_.
        '-----'


            _
           (+)
        .-. ^ .-.
       :   `.'   :
       `.  King .'
        )_.---._(
        `._____.'


      |\.
     /   '.
    /_.'- \
     / Kn |
    /_____|
    `.___.'



*/