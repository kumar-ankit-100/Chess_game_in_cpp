#include "common_header.h"

bool is_king_in_check(Board &board, int kingColor)
{
    char kingCol;
    int kingRow;
    // Find the king's position
    Bitboard kingPosition = board.pices[kingColor][king];
    for (int i = 0; i < 64; ++i)
    {
        if (kingPosition & (1ULL << i))
        {

            kingCol = 'h' - (i % 8);
            kingRow = 1 + (i / 8);
            cout << "king row and col and i" << kingRow << " " << kingCol << " " << i << endl;
            break;
        }
    }
    int opponentColor = (kingColor == white) ? black : white;
    // if (is_square_under_attack(board, kingCol, kingRow, opponentColor))
    // {
    //     cout << "Allowed Empty move: " << endl;
    //     for (auto moves : board.emptySquares)
    //         cout << moves << " ";
    //     cout << endl;
    //     cout << "Size of emptySquares: " << board.emptySquares.size() << endl;
    //     cout<<"name : "<<board.name<<endl;
    // }
    return is_square_under_attack(board, kingCol, kingRow, opponentColor);
}

bool is_move_legal(Board &board, string move, int currentPlayer)
{
    // Create a copy of the board to simulate the move
    Board tempBoard = board;

    // Apply the move to the temporary board
    if (!moveGeneration(move, tempBoard))
    {
        return false; // Invalid move
    }

    // Check if the current player's king is in check after the move
    return !is_king_in_check(tempBoard, currentPlayer);
}

bool is_checkmate(Board &board, int currentPlayer)
{
    // If the king is not in check, it's not checkmate
    if (!is_king_in_check(board, currentPlayer))
    {
        return false;
    }

    // Generate all possible moves for the current player
    vector<string> allMoves;
    for (char col = 'a'; col <= 'h'; ++col)
    {
        for (int row = 1; row <= 8; ++row)
        {
            Bitboard from_mask = 1ULL << ((row * 8) - (col - 'a') - 1);
            if (board.ouccupancy[currentPlayer] & from_mask)
            {
                vector<vector<string>> pieceMoves;
                if (board.pices[currentPlayer][pawn] & from_mask)
                {
                    pieceMoves = get_pawn_moves(board, col, row);
                }
                else if (board.pices[currentPlayer][rook] & from_mask)
                {
                    pieceMoves = get_rook_moves(board, col, row);
                }
                else if (board.pices[currentPlayer][knight] & from_mask)
                {
                    pieceMoves = get_knight_moves(board, col, row);
                }
                else if (board.pices[currentPlayer][bishop] & from_mask)
                {
                    pieceMoves = get_bishop_moves(board, col, row);
                }
                else if (board.pices[currentPlayer][queen] & from_mask)
                {
                    pieceMoves = get_queen_moves(board, col, row);
                }
                else if (board.pices[currentPlayer][king] & from_mask)
                {
                    pieceMoves = get_king_moves(board, col, row);
                }

                for (const auto &moveList : pieceMoves)
                {
                    for (const auto &dest : moveList)
                    {
                        allMoves.push_back(string(1, col) + to_string(row) + dest);
                    }
                }
            }
        }
    }

    // Check if any of the moves can get the king out of check
    for (const auto &move : allMoves)
    {
        if (is_move_legal(board, move, currentPlayer))
        {
            return false; // Found a legal move, not checkmate
        }
    }

    return true; // No legal moves found, it's checkmate
}

bool is_stalemate(Board &board, int currentPlayer)
{
    // If the king is in check, it's not stalemate
    if (is_king_in_check(board, currentPlayer))
    {
        return false;
    }

    // Generate all possible moves for the current player
    vector<string> allMoves;
    for (char col = 'a'; col <= 'h'; ++col)
    {
        for (int row = 1; row <= 8; ++row)
        {
            Bitboard from_mask = 1ULL << ((row * 8) - (col - 'a') - 1);
            if (board.ouccupancy[currentPlayer] & from_mask)
            {
                vector<vector<string>> pieceMoves;
                if (board.pices[currentPlayer][pawn] & from_mask)
                {
                    pieceMoves = get_pawn_moves(board, col, row);
                }
                else if (board.pices[currentPlayer][rook] & from_mask)
                {
                    pieceMoves = get_rook_moves(board, col, row);
                }
                else if (board.pices[currentPlayer][knight] & from_mask)
                {
                    pieceMoves = get_knight_moves(board, col, row);
                }
                else if (board.pices[currentPlayer][bishop] & from_mask)
                {
                    pieceMoves = get_bishop_moves(board, col, row);
                }
                else if (board.pices[currentPlayer][queen] & from_mask)
                {
                    pieceMoves = get_queen_moves(board, col, row);
                }
                else if (board.pices[currentPlayer][king] & from_mask)
                {
                    pieceMoves = get_king_moves(board, col, row);
                }

                for (const auto &moveList : pieceMoves)
                {
                    for (const auto &dest : moveList)
                    {
                        allMoves.push_back(string(1, col) + to_string(row) + dest);
                    }
                }
            }
        }
    }

    // Check if any of the moves are legal
    for (const auto &move : allMoves)
    {
        if (is_move_legal(board, move, currentPlayer))
        {
            return false; // Found a legal move, not stalemate
        }
    }

    return true; // No legal moves found, it's stalemate
}