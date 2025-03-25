#include "../../include/piece/Knight.h";
#include "../../include/Game.h"

Knight::Knight(int row, int col, bool pieceColor, std::string imageLoc, PieceType type) :Piece(row, col, pieceColor, imageLoc, type)
{
	
}

void Knight::storeValidMoves() {
	validMoves.clear();
	captureMoves.clear();

	for (auto move : moves) {
		int kRow = this->row + move.first;
		int kCol = this->col + move.second;

		if (kRow < 0 || kCol < 0 || kCol>7 || kRow>7) {
			continue;
		}
		bool isValid = true;
		for (Piece* p : Game::pieces) {
			if (kRow == p->row && kCol == p->col) {
				if (p->pieceColor != this->pieceColor) {
					captureMoves.push_back({ kRow,kCol });
				}
				isValid = false;
				break;
			}
		}

		if (isValid) {
			validMoves.push_back({ kRow,kCol });
		}
	}

	// Filter moves if checked and pinned
	filterMoves();
}

void Knight::addToCurrentAttackedBlocks(std::set<std::pair<int, int>>& attackedBlocks)
{
	for (auto move : moves) {
		int kRow = this->row + move.first;
		int kCol = this->col + move.second;

		if (kRow < 0 || kCol < 0 || kCol>7 || kRow>7) {
			continue;
		}

		for (Piece* p : Game::pieces) {
			if (kRow == p->row && kCol == p->col && p->type == PieceType::KING && p->pieceColor!=this->pieceColor) {
				King* k = (King*)p;
				k->isChecked = true;
				k->checkingPieces.insert(this);
			}
		}

		attackedBlocks.insert({ kRow,kCol });
	}
}
