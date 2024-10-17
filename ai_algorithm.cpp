#include <algorithm>
#include <climits>
#include <vector>
#include <string>
#include <cmath>
#include "common_header.h"

// Piece values for evaluation
const int PAWN_VALUE = 1;
const int KNIGHT_VALUE = 3;
const int BISHOP_VALUE = 4;
const int ROOK_VALUE = 5;
const int QUEEN_VALUE = 9;

// Function to evaluate the board position
int evaluateBoard(const Board &board)
{
    int whiteScore = 0;
    int blackScore = 0;

    // Count material
    for (int i = 0; i < 64; i++)
    {
        Bitboard mask = 1ULL << i;
        if (board.pices[white][pawn] & mask)
            whiteScore += PAWN_VALUE;
        if (board.pices[white][knight] & mask)
            whiteScore += KNIGHT_VALUE;
        if (board.pices[white][bishop] & mask)
            whiteScore += BISHOP_VALUE;
        if (board.pices[white][rook] & mask)
            whiteScore += ROOK_VALUE;
        if (board.pices[white][queen] & mask)
            whiteScore += QUEEN_VALUE;

        if (board.pices[black][pawn] & mask)
            blackScore += PAWN_VALUE;
        if (board.pices[black][knight] & mask)
            blackScore += KNIGHT_VALUE;
        if (board.pices[black][bishop] & mask)
            blackScore += BISHOP_VALUE;
        if (board.pices[black][rook] & mask)
            blackScore += ROOK_VALUE;
        if (board.pices[black][queen] & mask)
            blackScore += QUEEN_VALUE;
    }

    // You can add more sophisticated evaluation here (e.g., piece positioning, pawn structure)

    return whiteScore - blackScore;
}

// Function to generate all legal moves
std::vector<std::string> generateAllLegalMoves(const Board &board, int color)
{
    std::vector<std::string> allMoves;

    for (char col = 'a'; col <= 'h'; col++)
    {
        for (int row = 1; row <= 8; row++)
        {
            // Check each piece type
            std::vector<std::vector<std::string>> pieceMoves;

            if (board.pices[color][pawn] & (1ULL << ((row * 8) - (col - 97) - 1)))
                pieceMoves = get_pawn_moves(const_cast<Board &>(board), col, row);
            else if (board.pices[color][rook] & (1ULL << ((row * 8) - (col - 97) - 1)))
                pieceMoves = get_rook_moves(const_cast<Board &>(board), col, row);
            else if (board.pices[color][knight] & (1ULL << ((row * 8) - (col - 97) - 1)))
                pieceMoves = get_knight_moves(const_cast<Board &>(board), col, row);
            else if (board.pices[color][bishop] & (1ULL << ((row * 8) - (col - 97) - 1)))
                pieceMoves = get_bishop_moves(const_cast<Board &>(board), col, row);
            else if (board.pices[color][queen] & (1ULL << ((row * 8) - (col - 97) - 1)))
                pieceMoves = get_queen_moves(const_cast<Board &>(board), col, row);
            else if (board.pices[color][king] & (1ULL << ((row * 8) - (col - 97) - 1)))
                pieceMoves = get_king_moves(const_cast<Board &>(board), col, row);

            for (const auto &moveList : pieceMoves)
            {
                for (const auto &move : moveList)
                {
                    allMoves.push_back(std::string(1, col) + std::to_string(row) + move);
                }
            }
        }
    }

    return allMoves;
}

// Structure to store move information
struct MoveInfo
{
    std::string move;
    int movingColor;
    int capturedPiece; // -1 if no capture
    bool wasFirstMove; // For pawns and kings (castling rights)
};

// Function to make a move on the board
MoveInfo makeMove(Board &board, const std::string &move)
{
    char fromCol = move[0], toCol = move[2];
    int fromRow = move[1] - '0', toRow = move[3] - '0';

    Bitboard fromMask = 1ULL << ((fromRow * 8) - (fromCol - 'a') - 1);
    Bitboard toMask = 1ULL << ((toRow * 8) - (toCol - 'a') - 1);

    int movingColor = (board.ouccupancy[white] & fromMask) ? white : black;
    int capturedColor = 1 - movingColor;

    MoveInfo info;
    info.move = move;
    info.movingColor = movingColor;
    info.capturedPiece = -1;
    info.wasFirstMove = false;

    // Check for capture and store captured piece
    for (int pieceType = 0; pieceType < 6; pieceType++)
    {
        if (board.pices[capturedColor][pieceType] & toMask)
        {
            info.capturedPiece = pieceType;
            board.pices[capturedColor][pieceType] &= ~toMask;
            break;
        }
    }

    // Move the piece
    for (int pieceType = 0; pieceType < 6; pieceType++)
    {
        if (board.pices[movingColor][pieceType] & fromMask)
        {
            board.pices[movingColor][pieceType] &= ~fromMask;
            board.pices[movingColor][pieceType] |= toMask;

            // Check if it's a pawn or king's first move
            if ((pieceType == pawn && (fromRow == 2 || fromRow == 7)) ||
                (pieceType == king && fromCol == 'e' && (fromRow == 1 || fromRow == 8)))
            {
                info.wasFirstMove = true;
            }

            break;
        }
    }

    // Update occupancy bitboards
    board.ouccupancy[movingColor] = (board.ouccupancy[movingColor] & ~fromMask) | toMask;
    board.ouccupancy[capturedColor] &= ~toMask;
    board.ouccupancy[both] = board.ouccupancy[white] | board.ouccupancy[black];

    return info;
}

// Function to undo a move on the board
void undoMove(Board &board, const MoveInfo &info)
{
    char fromCol = info.move[0], toCol = info.move[2];
    int fromRow = info.move[1] - '0', toRow = info.move[3] - '0';

    Bitboard fromMask = 1ULL << ((fromRow * 8) - (fromCol - 'a') - 1);
    Bitboard toMask = 1ULL << ((toRow * 8) - (toCol - 'a') - 1);
    int movingColor = info.movingColor;
    int capturedColor = 1 - movingColor;

    // Move the piece back
    for (int pieceType = 0; pieceType < 6; pieceType++)
    {
        if (board.pices[movingColor][pieceType] & toMask)
        {
            board.pices[movingColor][pieceType] &= ~toMask;
            board.pices[movingColor][pieceType] |= fromMask;
            break;
        }
    }

    // Restore captured piece if any
    if (info.capturedPiece != -1)
    {
        board.pices[capturedColor][info.capturedPiece] |= toMask;
    }

    // Update occupancy bitboards
    board.ouccupancy[movingColor] = (board.ouccupancy[movingColor] & ~toMask) | fromMask;
    if (info.capturedPiece != -1)
    {
        board.ouccupancy[capturedColor] |= toMask;
    }
    board.ouccupancy[both] = board.ouccupancy[white] | board.ouccupancy[black];
}

// Updated minimax function
int minimax(Board &board, int depth, int alpha, int beta, bool maximizingPlayer)
{
    if (depth == 0)
    {
        return evaluateBoard(board);
    }

    std::vector<std::string> moves = generateAllLegalMoves(board, maximizingPlayer ? white : black);

    if (maximizingPlayer)
    {
        int maxEval = INT_MIN;
        for (const auto &move : moves)
        {
            MoveInfo info = makeMove(board, move);
            int eval = minimax(board, depth - 1, alpha, beta, false);
            undoMove(board, info);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha)
                break;
        }
        return maxEval;
    }
    else
    {
        int minEval = INT_MAX;
        for (const auto &move : moves)
        {
            MoveInfo info = makeMove(board, move);
            int eval = minimax(board, depth - 1, alpha, beta, true);
            undoMove(board, info);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha)
                break;
        }
        return minEval;
    }
}

// Updated getBestMove function
std::string getBestMove(Board &board, int color, int depth)
{
    std::vector<std::string> moves = generateAllLegalMoves(board, color);
    std::string bestMove;
    int bestValue = (color == white) ? INT_MIN : INT_MAX;

    for (const auto &move : moves)
    {
        cout << move << " ";
        MoveInfo info = makeMove(board, move);
        int value = minimax(board, depth - 1, INT_MIN, INT_MAX, color != white);
        undoMove(board, info);

        if ((color == white && value > bestValue) || (color == black && value < bestValue))
        {
            bestValue = value;
            bestMove = move;
        }
    }
    cout << "endl";

    return bestMove;
}