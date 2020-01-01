#include "ChessProtocolFunctions.hpp"
#pragma warning(disable:4996)
#include <cstring>
#include <string>

/*
This function checks if the data that recived from the other player is movment cmd or not
*/
bool ChessProtocol::isMsgCmd(char* data)
{
	return std::string(data).find("chat") != std::string::npos;
}

/*
This function convert from one player positions on board to another - in the move cmd (data*)
*/
void ChessProtocol::convertMoveCmd(char* data)
{
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	srcRow = data[5] - '0';
	srcCol = data[7] - '0';
	dstRow = data[9] - '0';
	dstCol = data[11] - '0';
	data[5] = (char)(7 - srcRow + '0');
	data[7] = (char)(7 - srcCol + '0');
	data[9] = (char)(7 - dstRow + '0');
	data[11] = (char)(7 - dstCol + '0');
	
}

std::tuple<char*, int, int, int, int, char> ChessProtocol::phraseDataToPos(char* data)
{
	if (std::string(data).find("move") != std::string::npos)
	{
		char opcode[6];
		strcpy(opcode, "crown");
		opcode[5] = ' ';
		return std::make_tuple(opcode, data[6], data[8], data[10], data[11], data[12]);
	}
	else
	{
		char opcode[5];
		strcpy(opcode, "move");
		opcode[4] = ' ';
		return std::make_tuple(opcode, data[5], data[7], data[8], data[9], ' ');
	}
}

char* ChessProtocol::convertPosToData(std::tuple<char*, int, int, int, int, char> data)
{
	auto [opcode, srcRow, srcCol, dstRow, dstCol, pieceChar] = data;
	std::string new_data;
	char* cstr;

	new_data = static_cast<char>(srcRow) + static_cast<char>(srcCol) + static_cast<char>(dstRow) + static_cast<char>(dstCol);

	if (strncmp(opcode, "move", 4) != 0)
		new_data = "crown " + new_data + pieceChar ;

	else
		new_data = "move ";
	
	cstr = new char(new_data.length() + 1);

	strcpy(cstr, new_data.c_str());
	return cstr; 
	
}

bool ChessProtocol::isMoveCmd(char* data) {
	return std::string(data).find("move") != std::string::npos;
}