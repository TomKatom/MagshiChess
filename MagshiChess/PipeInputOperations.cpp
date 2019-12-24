#pragma once
#include "PipeInputOperations.h"

std::tuple<int, int, int, int> PipeInputOperations::moveToPos(std::string move) {
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	srcRow = move[1] - '0'-1;
	dstRow = move[3] - '0'-1;
	srcCol = move[0] - 'a';
	dstCol = move[2] - 'a';
	return std::make_tuple(srcRow, srcCol, dstRow, dstCol);
}

Piece*** PipeInputOperations::generateBoard(Color& playerColor, std::string& startingBoard)
{
	Piece*** temp = new Piece * *[8];
	Color piece_color;
	char ch;

	for (int i = 0; i < 8; i++)
	{
		temp[i] = new Piece * [8];
	}

	for (int i = 7; i  >=0; i--)
	{
		for (int j = 7; j >= 0; j--)
		{
			ch = startingBoard.at(i*8 +j);
			if (isupper(ch))
				piece_color = white;
			else
				piece_color = black;

			switch (ch)
			{
			case 'r':
			case 'R':
				temp[i][j] = new Rook(piece_color);
				break;

			case '#':
				temp[i][j] = nullptr;
				break;
			default:
				break;
			}
		}
	}

	return temp;

}

