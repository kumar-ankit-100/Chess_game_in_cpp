#include <bits/stdc++.h>
#include "common_header.h"
using namespace std;

vector<char> pos_name = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

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

        int bit = (board.ouccupancy[both] >> (i * 8 - j)) & 1;
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
void moveGeneration(string moves, struct Board &board)
{
  // pawn moves
  // moves[1]-48 convert ascii to decimal number
  Bitboard bit = 1;
  Bitboard from_mask = bit << (((moves[1] - 48) * 8) - (moves[0] - 97) - 1);
  Bitboard to_mask = bit << (((moves[3] - 48) * 8) - (moves[2] - 97) - 1);
  cout << "from: " << (((moves[1] - 48) * 8) - (moves[0] - 97) - 1) << " to: " << (((moves[3] - 48) * 8) - (moves[2] - 97) - 1) << endl;
  int is_present = (board.ouccupancy[both] >> (((moves[1] - 48) * 8) - (moves[0] - 97) - 1)) & (from_mask >> (((moves[1] - 48) * 8) - (moves[0] - 97) - 1));
  if (is_present)
  {
    cout << "starting piece is present" << endl;
    if (board.ouccupancy[white] & from_mask)
    {
      cout << "Piece are white" << endl;
      if (board.pices[white][pawn] & from_mask)
      {
        cout << "piece is pawn" << endl;
        if (!is_move_valid(board, moves))
        {
          cout << "Please! Enter valid pawn move. " << endl;
          return;
        }
        if (is_this_pawn_first_move(board, moves[0], 1))
        {
          if (!(abs(moves[1] - moves[3]) == 2 || abs(moves[1] - moves[3]) == 1))
          {
            cout << "Please! Enter valid pawn move. " << endl;
            return ;
          }
        }
        else{
           if (!(abs(moves[1] - moves[3]) == 1))
          {
            cout << "Please! Enter valid pawn move. " << endl;
            return ;
          }
        }
        cout << "piece satus of present is : " << is_this_pawn_first_move(board, moves[0], 1) << endl;
        board.pices[white][pawn] = board.pices[white][pawn] & (~from_mask);
        board.pices[white][pawn] = board.pices[white][pawn] | (to_mask);
      }
      else if (board.pices[white][rook] & from_mask)
      {
        cout << "piece is rook" << endl;
        board.pices[white][rook] = board.pices[white][rook] & (~from_mask);
        board.pices[white][rook] = board.pices[white][rook] | (to_mask);
      }
      else if (board.pices[white][knight] & from_mask)
      {
        cout << "piece is knight" << endl;
        board.pices[white][knight] = board.pices[white][knight] & (~from_mask);
        board.pices[white][knight] = board.pices[white][knight] | (to_mask);
      }
      else if (board.pices[white][bishop] & from_mask)
      {
        cout << "piece is bishop" << endl;
        board.pices[white][bishop] = board.pices[white][bishop] & (~from_mask);
        board.pices[white][bishop] = board.pices[white][bishop] | (to_mask);
      }
      else if (board.pices[white][king] & from_mask)
      {
        cout << "piece is king" << endl;
        board.pices[white][king] = board.pices[white][king] & (~from_mask);
        board.pices[white][king] = board.pices[white][king] | (to_mask);
      }
      else if (board.pices[white][queen] & from_mask)
      {
        cout << "piece is queen" << endl;
        board.pices[white][queen] = board.pices[white][queen] & (~from_mask);
        board.pices[white][queen] = board.pices[white][queen] | (to_mask);
      }
      board.ouccupancy[white] = board.ouccupancy[white] & (~from_mask);
      board.ouccupancy[white] = board.ouccupancy[white] | (to_mask);
      board.ouccupancy[both] = board.ouccupancy[both] & (~from_mask);
      board.ouccupancy[both] = board.ouccupancy[both] | (to_mask);
    }
    else
    {
      cout << "Piece are black" << endl;
      if (board.pices[black][pawn] & from_mask)
      {
        cout << "piece is pawn" << endl;
        board.pices[black][pawn] = board.pices[black][pawn] & (~from_mask);
        board.pices[black][pawn] = board.pices[black][pawn] | (to_mask);
      }
      else if (board.pices[black][rook] & from_mask)
      {
        cout << "piece is rook" << endl;
        board.pices[black][rook] = board.pices[black][rook] & (~from_mask);
        board.pices[black][rook] = board.pices[black][rook] | (to_mask);
      }
      else if (board.pices[black][knight] & from_mask)
      {
        cout << "piece is knight" << endl;
        board.pices[black][knight] = board.pices[black][knight] & (~from_mask);
        board.pices[black][knight] = board.pices[black][knight] | (to_mask);
      }
      else if (board.pices[black][bishop] & from_mask)
      {
        cout << "piece is bishop" << endl;
        board.pices[black][bishop] = board.pices[black][bishop] & (~from_mask);
        board.pices[black][bishop] = board.pices[black][bishop] | (to_mask);
      }
      else if (board.pices[black][king] & from_mask)
      {
        cout << "piece is king" << endl;
        board.pices[black][king] = board.pices[black][king] & (~from_mask);
        board.pices[black][king] = board.pices[black][king] | (to_mask);
      }
      else if (board.pices[black][queen] & from_mask)
      {
        cout << "piece is queen" << endl;
        board.pices[black][queen] = board.pices[black][queen] & (~from_mask);
        board.pices[black][queen] = board.pices[black][queen] | (to_mask);
      }
      board.ouccupancy[black] = board.ouccupancy[black] & (~from_mask);
      board.ouccupancy[black] = board.ouccupancy[black] | (to_mask);
      board.ouccupancy[both] = board.ouccupancy[both] & (~from_mask);
      board.ouccupancy[both] = board.ouccupancy[both] | (to_mask);
    }
  }
  else
  {
    cout << "starting piece is not present" << endl;
  }
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
    moveGeneration(moves, board);
  }
}