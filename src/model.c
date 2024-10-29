#include "model.h"

bool is_pos_equal(position_s a, position_s b)
{
    return a.x == b.x && a.y == b.y;
}

bool has_live_piece_at(const pieces_s *p, position_s pos)
{
    return is_pos_equal(p->pos, pos) && p->is_alive;
}

const pieces_s piece_map[] = {
    {"pawn0", PAWN0},
    {"pawn1", PAWN1},
    {"pawn2", PAWN2},
    {"pawn3", PAWN3},
    {"pawn4", PAWN4},
    {"pawn5", PAWN5},
    {"pawn6", PAWN6},
    {"pawn7", PAWN7},
    {"rook8", ROOK8},
    {"rook9", ROOK9},
    {"knight10", KNIGHT10},
    {"knight11", KNIGHT11},
    {"bishop12", BISHOP12},
    {"bishop13", BISHOP13},
    {"queen", QUEEN},
    {"king", KING}};

row_s row_map[] = {
    {"a", a},
    {"b", b},
    {"c", c},
    {"d", d},
    {"e", e},
    {"f", f},
    {"g", g}};