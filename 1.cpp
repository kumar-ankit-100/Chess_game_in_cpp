#include <bits/stdc++.h>
using namespace std;

typedef uint64_t Bitboard;
struct Board
{

  Bitboard pices[2][6];
  Bitboard ouccupancy[3];
};
vector<char> pos_name = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
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
void initializeBoard(Board &board)
{

  // bitboard for each pices of white colour
  board.pices[white][pawn] = 0x000000000000ff00;
  board.pices[white][rook] = 0x0000000000000081;
  board.pices[white][knight] = 0x0000000000000042;
  board.pices[white][bishop] = 0x0000000000000024;
  board.pices[white][king] = 0x0000000000000008;
  board.pices[white][queen] = 0x0000000000000010;

  // bitboard for each pices of black colour
  board.pices[black][pawn] = 0x00ff000000000000;
  board.pices[black][rook] = 0x8100000000000000;
  board.pices[black][knight] = 0x4200000000000000;
  board.pices[black][bishop] = 0x2400000000000000;
  board.pices[black][king] = 0x0800000000000000;
  board.pices[black][queen] = 0x1000000000000000;

  // bitboard for each colour and both colour
  board.ouccupancy[white] = 0x000000000000ffff;
  board.ouccupancy[black] = 0xffff000000000000;
  board.ouccupancy[both] = 0xffff00000000ffff;
}
void printBoard(Board &board)
{
  int flag = 1; // this flag is used to check occupancy
  for (int i = 8; i >= 0; i--)
  {

    for (int j = 0; j <= 8; j++)
    {
      // condition for writing position name like 1-8 & a-z
      if (i == 0)
      {
        if (j == 0)
          cout << "   ";
        else
          cout << pos_name[j - 1] << " ";
        continue;
      }
      if (j == 0)
      {
        cout << i << "  ";
        continue;
      }

      // printing board using bitboard
      // white pices
      if (flag)
      {

        if (board.pices[white][king] >> (i * 8 - j) & 1)
          cout << "K ";
        else if (board.pices[white][queen] >> (i * 8 - j) & 1)
          cout << "Q ";
        else if (board.pices[white][bishop] >> (i * 8 - j) & 1)
          cout << "B ";
        else if (board.pices[white][knight] >> (i * 8 - j) & 1)
          cout << "N ";
        else if (board.pices[white][rook] >> (i * 8 - j) & 1)
          cout << "R ";
        else if (board.pices[white][pawn] >> (i * 8 - j) & 1)
          cout << "P ";

        // black pieces

        else if (board.pices[black][king] >> (i * 8 - j) & 1)
          cout << "k ";
        else if (board.pices[black][queen] >> (i * 8 - j) & 1)
          cout << "q ";
        else if (board.pices[black][bishop] >> (i * 8 - j) & 1)
          cout << "b ";
        else if (board.pices[black][knight] >> (i * 8 - j) & 1)
          cout << "n ";
        else if (board.pices[black][rook] >> (i * 8 - j) & 1)
          cout << "r ";
        else if (board.pices[black][pawn] >> (i * 8 - j) & 1)
          cout << "p ";

        else
          cout << "0 ";
      }

      if (!flag)
      {

        int bit = board.ouccupancy[both] >> (i * 8 - j) & 1;
        if (bit)
        {
          cout << "1 ";
        }
        else
          cout << "0 ";
      }
    }
    cout << endl;
  }
}
void moveGeneration(string moves)
{
  // pawn moves
  // moves[1]-48 convert ascii to decimal number
  int from_mask = 1ULL << (((moves[1] - 48) * 8) - (moves[0] - 97) - 1);
  int to_mask = 1ULL << (((moves[3] - 48) * 8) - (moves[2] - 97) - 1);
  cout << (((moves[3] - 48) * 8) - (moves[2] - 97) - 1) << endl;
}

int main()
{

  // cout<<king;
  Board board;
  string moves;
  initializeBoard(board);
  int flag = 1;
  while (true)
  {
    printBoard(board);
    if (flag)
    {
      cout << "\nWhite Moves , Enter Moves name (eg. e2e4): ";
      cin >> moves;
      flag = 0;
    }
    else
    {
      cout << "\nBlack Moves , Enter Moves name (eg. e2e4): ";
      cin >> moves;
      flag = 1;
    }
    // cout<<moves[2]<<" "<<moves[3]<<"\n";
    moveGeneration(moves);
  }
}