#include <bits/stdc++.h>
#include "common_header.h"
using namespace std;

int is_piece_present_in_square(Board &board, char col, int row)
{
    Bitboard bit = 1;
    Bitboard to_mask = bit << ((row * 8) - (col - 97) - 1);
    return board.ouccupancy[both] & to_mask ? 1 : 0;
}

int is_this_pawn_first_move(Board &board, char move, int flag)
{
    // if flag = 1 then white makes pawn move so start row will be 2 for white
    // and 7 for black
    Bitboard bit = 1;
    if (flag)
    {
        Bitboard mask = bit << ((2 * 8) - (move - 97) - 1);
        return board.pices[white][pawn] & mask ? 1 : 0;
    }
    else
    {
        Bitboard mask = bit << ((7 * 8) - (move - 97) - 1);
        return board.pices[black][pawn] & mask ? 1 : 0;
    }
}

int is_pawn_move_valid(Board &board, string &moves, int flag)
{
    // check if pawn try to capture other pices
    if (abs((moves[0] - 97) - (moves[2] - 97)) == 1 && abs((moves[1] - 48) - (moves[3] - 48)) == 1)
    {
        if (is_piece_present_in_square(board, moves[2], moves[3]))
        {
            // cout<<"pice cannot be captured since piece is not available"<<endl;

            return 5;
        }
    }

    // check if pawn not move  in straight
    if (moves[0] != moves[2])
        return 0;

    // check that pawn move up for white and down for black
    if (flag)
    {
        if (((moves[3] - 48) - (moves[1] - 48)) < 0)
            return 0;
    }
    else
    {
        if (((moves[1] - 48) - (moves[3] - 48)) < 0)
            return 0;
    }

    // check if pawn move more than 2  steps in first and more than
    //  1 step in second move onward
    if (is_this_pawn_first_move(board, moves[0], flag))
    {
        if (!(abs(moves[1] - moves[3]) == 2 || abs(moves[1] - moves[3]) == 1))
        {

            return 0;
        }
        else
        {
            if (abs(moves[1] - moves[3]) == 2)
            {
                // flag =1 for white so we need to add 1 or to check wether pices is present or not
                // flag = 0 for black so we need to subtract
                if (flag)
                    return is_piece_present_in_square(board, moves[0], (moves[1] - 48 + 1)) || is_piece_present_in_square(board, moves[0], (moves[1] - 48 + 2)) ? 0 : 1;
                else
                    return is_piece_present_in_square(board, moves[0], (moves[1] - 48 - 1)) || is_piece_present_in_square(board, moves[0], (moves[1] - 48 - 2)) ? 0 : 1;
            }
            else
            {
                if (flag)
                    return is_piece_present_in_square(board, moves[0], (moves[1] - 48 + 1)) ? 0 : 1;
                else
                    return is_piece_present_in_square(board, moves[0], (moves[1] - 48 - 1)) ? 0 : 1;
            }
        }
    }
    else
    {
        if (!(abs(moves[1] - moves[3]) == 1))
        {
            return 0;
        }
        else
        {
            if (flag)
                return is_piece_present_in_square(board, moves[0], (moves[1] - 48 + 1)) ? 0 : 1;
            else
                return is_piece_present_in_square(board, moves[0], (moves[1] - 48 - 1)) ? 0 : 1;
        }
    }
    return 1;
}

// int capture_pices_by_pawn(Board &board, string &moves)
// {
// }