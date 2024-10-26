#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h> // bool

#define NUM_ROW 8
#define NUM_COL 8

typedef enum player_e
{
    WHITE,
    BLACK,

    NUM_PLAYERS
} player_e;

typedef enum piece_type
{
    PAWN = 0,
    ROOK = 8,
    KNIGHT = 10,
    BISHOP = 12,
    QUEEN = 14,
    KING = 15,

    NUM_PIECES
} piece_type_e;

typedef struct pos
{
    unsigned int x;
    unsigned int y;
} position_s;

typedef struct pieces
{
    piece_type_e piece_type;
    position_s pos;
    bool is_alive;
} pieces_s;

typedef struct board
{
    pieces_s player[NUM_PLAYERS][NUM_PIECES];
} board_s;

#endif