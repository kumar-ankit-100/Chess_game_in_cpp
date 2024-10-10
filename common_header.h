#include <bits/stdc++.h>
#include <string>
using namespace std;
typedef uint64_t Bitboard;
struct Board
{

  Bitboard pices[2][6];
  Bitboard ouccupancy[3];
  string name ; 
  vector<string> emptySquares;
};
enum
{
  pawn,
  rook,
  knight,
  bishop,
  king,
  queen
};
enum
{
  white,
  black,
  both
};

int is_this_pawn_first_move(Board &board, char move, int flag);
int is_pawn_move_valid(Board &board, string &moves,int flag);  
int is_rook_move_valid(Board &board, string &moves,int flag);  
int is_bishop_move_valid(Board &board, string &moves,int flag); 
int is_king_move_valid(Board &board, string &moves,int flag);  
int is_knight_move_valid(Board &board, string &moves,int flag);  
int is_queen_move_valid(Board &board, string &moves,int flag);  

int moveGeneration(string moves, struct Board &board);
void initializeBoard(Board &board);
void printBoard(Board &board);
void capturePieces(Bitboard to_mask, Board &board, int flag);

vector<string> tempfun();
vector<vector<string>> get_pawn_moves(Board &board, char col , int row);
vector<vector<string>> get_rook_moves(Board &board, char col, int row);
vector<vector<string>> get_bishop_moves(Board &board, char col, int row) ;
vector<vector<string>> get_queen_moves(Board &board, char col, int row);
vector<vector<string>> get_knight_moves(Board &board, char col, int row);
vector<vector<string>> get_king_moves(Board &board, char col, int row);

bool is_king_in_check(Board &board, int kingColor);
bool is_square_under_attack(Board &board, char col, int row, int opponentColor);
// bool isValidMoveInput(const string &move);


//check condition
bool is_king_in_check(Board &board, int kingColor);
bool is_checkmate(Board &board, int currentPlayer);
bool is_stalemate(Board &board, int currentPlayer);

string positionToString(char col, int row);
int is_piece_present_in_square(Board &board, char col, int row);
