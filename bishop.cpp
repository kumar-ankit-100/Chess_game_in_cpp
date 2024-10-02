#include <bits/stdc++.h>
#include "common_header.h"
using namespace std;

int is_bishop_move_valid(Board &board, string &moves, int flag)
{
    int source_row = moves[1] - 48;
    int source_col = moves[0] - 97;
    int destination_col = moves[2] - 97;
    int destination_row = moves[3] - 48;

    if (!(abs(source_col - destination_col) == abs(source_row - destination_row)))
    {
        cout << "piece can only move in diagonally" << endl;
        return 0;
    }
    // Direction 1: Moving diagonally right-up

    if (destination_col > source_col && destination_row > source_row)
    {
        source_col += 1;
        source_row += 1;
        destination_col -= 1;
        destination_row -= 1;
        while (source_col <= destination_col && source_row <= destination_row)
        {
            Bitboard bit = 1;
            Bitboard from_mask = bit << ((source_row * 8) - source_col - 1);
            if (board.ouccupancy[both] & from_mask)
            {
                cout << "No pieces should be present between source and destination" << endl;
                return 0;
            }
            source_col++;
            source_row++;
        }
    }
    // Direction 2: Moving diagonally left-up
    else if (destination_col < source_col && destination_row > source_row)
    {
        source_col -= 1;
        source_row += 1;
        while (source_col > destination_col && source_row < destination_row)
        {
            Bitboard bit = 1;
            Bitboard from_mask = bit << ((source_row * 8) - source_col - 1);
            if (board.ouccupancy[both] & from_mask)
            {
                cout << "No pieces should be present between source and destination" << endl;
                return 0;
            }
            source_col--;
            source_row++;
        }
    }
    // Direction 3: Moving diagonally right-down
    else if (destination_col > source_col && destination_row < source_row)
    {
        source_col += 1;
        source_row -= 1;
        while (source_col < destination_col && source_row > destination_row)
        {
            Bitboard bit = 1;
            Bitboard from_mask = bit << ((source_row * 8) - source_col - 1);
            if (board.ouccupancy[both] & from_mask)
            {
                cout << "No pieces should be present between source and destination" << endl;
                return 0;
            }
            source_col++;
            source_row--;
        }
    }
    // Direction 4: Moving diagonally left-down
    else if (destination_col < source_col && destination_row < source_row)
    {
        source_col -= 1;
        source_row -= 1;
        while (source_col > destination_col && source_row > destination_row)
        {
            Bitboard bit = 1;
            Bitboard from_mask = bit << ((source_row * 8) - source_col - 1);
            if (board.ouccupancy[both] & from_mask)
            {
                cout << "No pieces should be present between source and destination" << endl;
                return 0;
            }
            source_col--;
            source_row--;
        }
    }

    return 1;
}
