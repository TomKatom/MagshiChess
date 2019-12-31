#include "ChessProtocolFunctions.hpp"
#pragma warning(disable:4996)
#include <cstring>
#include <string>

/*
This function checks if the data that recived from the other player is movment cmd or not
*/
bool ChessProtocol::isMoveCmd(char* data)
{
	return strncmp(data, "move", 4) || strncmp(data, "crown", 5);
}

/*
This function convert from one player positions on board to another - in the move cmd (data*)
*/
void ChessProtocol::convertMoveCmd(char* data)
{
	auto [opcode, srcRow, srcCol, dstRow, dstCol] = phraseDataToPos(data);
	data = ChessProtocol::convertPosToData(std::make_tuple(opcode, 7 - srcRow, 7 - srcCol, 7 - dstRow, 7 - dstCol));
}

std::tuple<char*, int, int, int, int> ChessProtocol::phraseDataToPos(char* data)
{
	if (strncmp(data, "move", 4) != 0)
	{
		char opcode[6];
		strcpy(opcode, "crown");
		opcode[5] = '/0';
		return std::make_tuple(opcode, data[6], data[7], data[8], data[9]);
	}
	else
	{
		char opcode[5];
		strcpy(opcode, "move");
		opcode[4] = '/0';
		return std::make_tuple(opcode, data[5], data[6], data[7], data[8]);
	}
}

char* ChessProtocol::convertPosToData(std::tuple<char*, int, int, int, int> data)
{
	auto [opcode, srcRow, srcCol, dstRow, dstCol] = data;
	std::string new_data;
	char* cstr;
	if (strncmp(opcode, "move", 4) != 0)
		new_data = "crown ";

	else
		new_data = "move ";
	
	new_data += static_cast<char>(srcRow) + static_cast<char>(srcCol) + static_cast<char>(dstRow) + static_cast<char>(dstCol);
	cstr = new char(new_data.length() + 1);

	strcpy(cstr, new_data.c_str());
	return cstr; 
	
}