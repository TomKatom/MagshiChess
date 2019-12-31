#include "ChessProtocolFunctions.hpp"
#pragma warning(disable:4996)
#include <cstring>

/*
This function checks if the data that recived from the other player is movment cmd or not
*/
bool ChessProtocol::isMoveCmd(char* data)
{
	return strncmp(data, "move", 4);
}

/*
This function convert from one player positions on board to another - in the move cmd (data*)
*/
void ChessProtocol::convertMoveCmd(char* data)
{

}

