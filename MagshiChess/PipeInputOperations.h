#pragma once
#include <tuple>
#include "Piece.h"
#include <string>
#include "Rook.h"
#include "Pawn.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Knight.h"
#include <locale>
 
class PipeInputOperations {
public:
	static std::tuple<int, int, int, int> moveToPos(std::string move);
	static Piece*** generateBoard(Color& playerColor, std::string& startingBoard, Color onlinePlayerColor);
};