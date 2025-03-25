#pragma once
#include "Piece.h"
#include <set>


class King :public Piece {
public:

	std::vector<std::pair<int, int>> moves = { {0,1},{0,-1},{1,0},{-1,0},{1,1},{-1,-1},{1,-1},{-1,1}};
	bool isChecked = false;
	std::set<Piece*> checkingPieces;


	King(int row, int col, bool pieceColor, std::string imageLoc, PieceType type);
	void storeValidMoves() override;
	void storeCastlingMoves() override;
	void addToCurrentAttackedBlocks(std::set<std::pair<int, int>>& attackedBlocks) override;
};