#include "Player.h"

Player::Player() //c'tor
{
}

Player::~Player()  //d'tor
{
	//TODO: Call the d'tor of the class that communicate with the server
	delete[] this->_board;
}

void Player::setPipe(Pipe p)
{
	this->_p = p;
}

void Player::makeMove(std::string& msgFromGraphics)
{

	error_level_code move_code = isValidCMD(msgFromGraphics);

	if (move_code == valid || move_code == valid_check || move_code == check_mate)
	{

	}
	char msgToGraphics[1024];
	msgToGraphics[0] = (char)(move_code + '0');
	msgToGraphics[1] = 0;

	this->_p.sendMessageToGraphics(msgToGraphics);

}
error_level_code Player::isValidCMD(std::string& msgFromGraphics) const
{
	int srcRow, srcCol, dstRow, dstCol;
	std::tie(srcRow, srcCol, dstRow, dstCol) = PipeInputOperations::moveToPos(msgFromGraphics);
	error_level_code response_code = valid; 


	return valid;
	//check error_level_code 2 - in src position there is no piece of curr player
	if (this->_board[srcRow][srcCol]->getColor() != this->_color)
	{
		response_code = invalid_src_not_occupied;
	}
	else if (this->_board[dstRow][dstCol]->getColor() == this->_color) //check error_level_code 3 - in dst position there is piece of curr player
	{
		response_code = invalid_dst_occupied;
	}
	else if (!this->_board[srcRow][srcCol]->validMove(this->_board, msgFromGraphics)) //check error_level_code 6 - invalid move of piece
	{
		response_code = invalid_behevior; 
	}
	else if (srcRow == dstRow && srcCol == dstCol)  //check error_level_vode 7 - dst posiiton and src position are same
	{
		response_code = invlaid_same_src_dst;
	}
	else
	{
		return valid;
	}

	return response_code;
}

