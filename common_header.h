#include <bits/stdc++.h>
#include <string>
using namespace std;
typedef uint64_t Bitboard;
struct Board
{

  Bitboard pices[2][6];
  Bitboard ouccupancy[3];
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