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
        cout << "King can only move 1 ahead in any direction" << endl;
        return 0;
    }

    return 1;
}
vector<vector<string>> get_king_moves(Board &board, char col, int row)
{
    vector<vector<string>> possible_moves;
    vector<string> moveList;
    vector<string> captureList;
    Bitboard bit = 1;
    Bitboard from_mask = bit << ((row * 8) - (col - 97) - 1);
    int flag = board.ouccupancy[white] & from_mask ? white : black;
    int opponentColor = flag == white ? black : white;

    // Define all possible king moves (one square in any direction)
    int kingMoves[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // Vertical and horizontal
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // Diagonal
    };

    for (auto &move : kingMoves)
    {
        char new_col = col + move[1];
        int new_row = row + move[0];

        // Check if the new position is within the board boundaries
        if (new_col >= 'a' && new_col <= 'h' && new_row >= 1 && new_row <= 8)
        {
            Bitboard to_mask = bit << ((new_row * 8) - (new_col - 97) - 1);

            // Check if the square is occupied by an opponent's piece
            if (board.ouccupancy[opponentColor] & to_mask)
            {
                captureList.push_back(string(1, new_col) + to_string(new_row));
            }
            // Check if the square is empty
            else if (!(board.ouccupancy[both] & to_mask))
            {
                moveList.push_back(string(1, new_col) + to_string(new_row));
            }
        }
    }

    possible_moves.push_back(moveList);     // Normal moves
    possible_moves.push_back(captureList);  // Capture moves
    return possible_moves;
}
