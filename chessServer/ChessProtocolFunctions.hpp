#pragma once
#include <tuple>

namespace ChessProtocol
{
	bool isMoveCmd(char* data);  //This function checks if the data that recived from the other player is movment cmd or not
	void convertMoveCmd(char* data); //This function convert from one player positions on board to another - in the move cmd (data*)
}
