#pragma once
#include <iostream>
enum Color {white, black};
enum error_level_code { valid = 0, valid_check, invalid_src_not_occupied, invalid_dst_occupied, invalid_make_check, invalid_index_error, invalid_behevior, invlaid_same_src_dst, check_mate };

class Piece {
public:
	Piece(Color color);
	~Piece();
	virtual bool validMove(Piece*** board, std::string move) = 0;
	Color getColor() const;
	virtual char getPieceChar() const = 0;
private:
	Color _color;
	char _pieceChar;
};