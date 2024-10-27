#include "model.h"

bool is_pos_equal(position_s a, position_s b)
{
    return a.x == b.x && a.y == b.y;
}

bool has_live_piece_at(const pieces_s *p, position_s pos)
{
    return is_pos_equal(p->pos, pos) && p->is_alive;
}