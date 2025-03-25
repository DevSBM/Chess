#pragma once
#include "Piece.h"
#include <set>

class Queen :public Piece {
public:

	std::vector<std::pair<int, int>> diagonalMoves = { {1,1},{-1,1},{1,-1},{-1,-1} };

	Queen(int row, int col, bool pieceColor, std::string imageLoc, PieceType type);
	void storeValidMoves() override;
	void addToCurrentAttackedBlocks(std::set<std::pair<int, int>>& attackedBlocks) override;
	//void storeCheckedPath(Piece* king) override;
};