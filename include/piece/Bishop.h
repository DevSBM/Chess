#pragma once
#include "Piece.h"
#include <set>

class Bishop :public Piece {
public:

	std::vector<std::pair<int, int>> moves = { {1,1},{-1,1},{1,-1},{-1,-1} };

	Bishop(int row, int col, bool pieceColor, std::string imageLoc, PieceType type);
	void storeValidMoves() override;
	void addToCurrentAttackedBlocks(std::set<std::pair<int, int>>& attackedBlocks) override;

};