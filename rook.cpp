#include "common_header.h"

int is_rook_move_valid(Board &board, string &moves, int flag)
{
    // check that rook moves in same direction or not
    if (!((moves[0] == moves[2]) || (moves[1] == moves[3])))
    {
        cout << "Please ! Enter valid rook move since rook move only in one direction " << endl;
        return 0;
    }
    // this is the case for capturing pieces
    //  if(is_piece_present_in_square(board,moves[2],moves[3])){
    //      return 5;
    //  }
    if (moves[0] == moves[2])
    {
        // if file is same then check is there any pieces in between source and destination
        int source = (moves[1] - 48);
        int destination = moves[3] - 48;
        cout << source << " " << destination << endl;

        if (source <= destination)
        {
            source = source + 1;
            destination = destination - 1;
            cout << "rook is moving upward " << endl;

            while (source <= destination)
            {
                Bitboard bit = 1;
                Bitboard from_mask = bit << ((source * 8) - (moves[0] - 97) - 1);
                // checking that is piece is present in particular box or not

                if (board.ouccupancy[both] & from_mask)
                {
                    cout << "No pieces should be present between source and destination" << endl;
                    return 0;
                }
                source++;
            }
        }
        else
        {
            source = source - 1;
            destination = destination + 1;
            cout << "rook is moving downward " << endl;
            while (source >= destination)
            {
                Bitboard bit = 1;
                Bitboard from_mask = bit << ((source * 8) - (moves[0] - 97) - 1);
                // checking that is piece is present in particular box or not

                if (board.ouccupancy[both] & from_mask)
                {
                    cout << "No pieces should be present between source and destination" << endl;
                    return 0;
                }
                source--;
            }
        }
    }
    if (moves[1] == moves[3])
    {
        // if rank  is same then check is there any pieces in between source and destination
        int source = (moves[0] - 97);
        int destination = moves[2] - 97;
        cout << source << " " << destination << endl;
        if (source <= destination)
        {
            source = source + 1;
            destination = destination - 1;
            cout << "rook is moving right " << endl;
            while (source <= destination)
            {
                Bitboard bit = 1;
                Bitboard from_mask = bit << (((moves[1] - 48) * 8) - source - 1);
                // checking that is piece is present in particular box or not
                if (board.ouccupancy[both] & from_mask)
                {
                    cout << "No pieces should be present between source and destination" << endl;
                    return 0;
                }
                source++;
            }
        }
        else
        {
            source -= 1;
            destination = destination + 1;
            cout << "rook is moving left" << endl;
            while (source >= destination)
            {

                Bitboard bit = 1;
                Bitboard from_mask = bit << (((moves[1] - 48) * 8) - source - 1);
                // checking that is piece is present in particular box or not
                if (board.ouccupancy[both] & from_mask)
                {
                    cout << "No pieces should be present between source and destination" << endl;
                    return 0;
                }
                source--;
            }
        }
    }
    return 1;
}