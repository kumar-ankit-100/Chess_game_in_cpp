#include "common_header.h"

int is_knight_move_valid(Board &board, string &moves, int flag)
{
    int source_row = moves[1] - 48;
    int source_col = moves[0] - 97;
    int destination_col = moves[2] - 97;
    int destination_row = moves[3] - 48;

    if (abs(source_row - destination_row) == 1)
    {
        if (abs(source_col - destination_col) == 2)
        {
            return 1; //valid knight move
        }
    }
    if (abs(source_row - destination_row) == 2)
    {
        if (abs(source_col - destination_col) == 1)
        {
            return 1; //valid knight move
        }
    }
    return 0;//invalid knight move
}