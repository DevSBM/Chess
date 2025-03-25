#pragma once
#include "Piece.h"
#include <set>

class Rook :public Piece {
public:

	Rook(int row, int col, bool pieceColor, std::string imageLoc, PieceType type);
	void storeValidMoves() override;
	void addToCurrentAttackedBlocks(std::set<std::pair<int, int>>& attackedBlocks) override;
};