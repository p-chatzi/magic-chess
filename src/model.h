#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

#define NB_COL 8

typedef enum player {
    WHITE,
    BLACK,

    NUM_PLAYERS
} player_e;

typedef enum menu {
    EXIT,
    START,
    LOAD,
    INVENT,
    RULES,
    SETTINGS,
    SAVE,
    TIE,
    FF
} menu_options_e;

typedef enum piece_type {
    PAWN0,
    PAWN1,
    PAWN2,
    PAWN3,
    PAWN4,
    PAWN5,
    PAWN6,
    PAWN7,
    ROOK8,
    ROOK9,
    KNIGHT10,
    KNIGHT11,
    BISHOP12,
    BISHOP13,
    QUEEN,
    KING,

    NB_PIECES
} piece_type_e;

typedef enum player_choice {
    PIECE_ID,
    ROW_ID,
    COL_ID,

    NB_INPUTS
} player_choice_e;

typedef enum col_name {
    a,
    b,
    c,
    d,
    e,
    f,
    g,
    h,

    NB_ROW
} col_name_e;

typedef struct col {
    char name[2];
    int id;
} col_s;

typedef struct pos {
    unsigned int x;
    unsigned int y;
} position_s;

bool is_pos_equal(position_s a, position_s b);

typedef struct pieces {
    char name[9];
    piece_type_e piece_type;
    position_s pos;
    int times_moved;
    bool is_alive;
} pieces_s;

bool has_live_piece_at(const pieces_s* p, position_s pos);

typedef struct board {
    pieces_s player[NUM_PLAYERS][NB_PIECES];
} board_s;

extern const pieces_s piece_map[];

extern col_s col_map[];

#endif
