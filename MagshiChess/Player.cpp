#include "Player.h"

Player::Player(Color c, std::string startingBoard) //c'tor
{
	this->_color = c;
	this->_board = PipeInputOperations::generateBoard(this->_color, startingBoard);
	if (c == Color::black) this->_king = dynamic_cast<King*>(this->_board[7][3]);
	else this->_king = dynamic_cast<King*>(this->_board[0][4]);
}

Player::~Player()  //d'tor
{
	//TODO: Call the d'tor of the class that communicate with the server
	delete[] this->_board;
}

King* Player::getKing()
{
	return this->_king;
}

Piece*** Player::getBoard()
{
	return this->_board;
}
void Player::makeMove(std::tuple<int,int,int,int>positions)
{
	int srcRow, srcCol, dstRow, dstCol;
	std::tie(srcRow, srcCol, dstRow, dstCol) = positions;
	Piece* temp = this->_board[srcRow][srcCol];
	this->_board[dstRow][dstCol] = temp;
	this->_board[srcRow][srcCol] = nullptr;

}
error_level_code Player::isValidCMD(std::tuple<int, int, int, int> positions) 
{
	int srcRow, srcCol, dstRow, dstCol;
	std::tie(srcRow, srcCol, dstRow, dstCol) = positions;
	error_level_code response_code = valid; 

	//check error_level_code 2 - in src position there is no piece of curr player
	if (this->_board[srcRow][srcCol] == nullptr || this->_board[srcRow][srcCol]->getColor() != this->_color)
	{
		response_code = invalid_src_not_occupied;
	}
	//check error_level_code 3 - in dst position there is piece of curr player
	else if (this->_board[dstRow][dstCol] != nullptr && this->_board[dstRow][dstCol]->getColor() == this->_color)
	{
		response_code = invalid_dst_occupied;
	} 
	else if (srcRow == dstRow && srcCol == dstCol)  //check error_level_vode 7 - dst posiiton and src position are same
	{
		response_code = invlaid_same_src_dst;
	}
	//check error_level_code 6 - invalid move of piece
	else if (this->_board[srcRow][srcCol] != nullptr && !this->_board[srcRow][srcCol]->validMove(this->_board, positions))
	{
		response_code = invalid_behevior; 
	}
	else   //TODO: add check to check and mate
	{
		auto temp = this->_board;
		this->makeMove(positions);
		if (!this->_king->isChecked(this->_board))
			return valid;
		
		else
		{
			this->_board = temp;
			return invalid_make_check;
		}
	}

	return response_code;
}

