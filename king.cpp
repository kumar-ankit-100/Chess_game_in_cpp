#include "common_header.h"

int is_king_move_valid(Board &board, string &moves, int flag)
{

    // check for is king move valid
    int source_row = moves[1] - 48;
    int source_col = moves[0] - 97;
    int destination_col = moves[2] - 97;
    int destination_row = moves[3] - 48;
    if (!(abs(destination_col - source_col) <= 1 && abs(destination_row - source_row) <= 1))
    {
        cout<<"King can only move 1 ahead in any direction"<<endl;
        return 0;
    }

    return 1;
}
