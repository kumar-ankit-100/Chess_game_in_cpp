#include "common_header.h"

int is_queen_move_valid(Board &board, string &moves, int flag)
{
    // Queen moves like both a bishop (diagonal) and a rook (straight).
    if (is_bishop_move_valid(board, moves, flag) || is_rook_move_valid(board, moves, flag))
    {
        return 1; // Move is valid either as a bishop or a rook move.
    }

    // If neither of bishop or rook moves are valid, return invalid.
    return 0;
}
