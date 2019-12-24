#include "PipeInputOperations.h"
#include <tuple>
std::tuple<int, int, int, int> PipeInputOperations::moveToPos(std::string move) {
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	srcRow = move[1] - '0';
	dstRow = move[3] - '0';
	srcCol = move[0] - 'a';
	dstCol = move[2] - 'a';
	return std::make_tuple(srcRow, srcCol, dstRow, dstCol);
}