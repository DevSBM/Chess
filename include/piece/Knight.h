#pragma once
#include "Piece.h"
#include <set>

class Knight :public Piece {
public:
	std::vector<std::pair<int, int>> moves = {
		{2,1},{2,-1},{-2,1},{-2,-1},{1,2},{-1,2},{1,-2},{-1,-2}
	};
	Knight(int row, int col, bool pieceColor, std::string imageLoc, PieceType type);
	void storeValidMoves() override;
	void addToCurrentAttackedBlocks(std::set<std::pair<int, int>>& attackedBlocks) override;

};