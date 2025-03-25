#include "../../include/piece/Bishop.h";
#include "../../include/Game.h"

Bishop::Bishop(int row, int col, bool pieceColor, std::string imageLoc, PieceType type) :Piece(row, col, pieceColor, imageLoc, type)
{

}

void Bishop::storeValidMoves() {
	validMoves.clear();
	captureMoves.clear();

	

	for (auto move : moves) {
		int r = row, c = col;
		while (r >= 0 && c >= 0 && r <= 7 && c <= 7) {
			bool isValid = true;
			r += move.first;
			c += move.second;
			for (Piece* p : Game::pieces) {
				if (r == p->row && c == p->col) {
					if (p->pieceColor != this->pieceColor) {
						captureMoves.push_back({ r,c });
					}
					isValid = false;
					break;
				}
			}

			if (isValid) {
				validMoves.push_back({ r,c });
			}
			else {
				break;
			}
		}
	}

	// Filter moves if checked and pinned
	filterMoves();
}

void Bishop::addToCurrentAttackedBlocks(std::set<std::pair<int, int>>& attackedBlocks)
{
	bool isChecking = false;
	for (auto move : moves) {
		int r = row, c = col;
		while (r >= 0 && c >= 0 && r <= 7 && c <= 7) {
			bool isValid = true;
			r += move.first;
			c += move.second;
			for (Piece* p : Game::pieces) {
				if (r == p->row && c == p->col) {
					attackedBlocks.insert({ r,c });
					
					if (p->type == PieceType::KING && p->pieceColor!=this->pieceColor) {
						King* k = (King*)p;
						k->isChecked = true;
						k->checkingPieces.insert(this);
					}
					else {
						isValid = false;
						break;
					}
				}
			}

			if (isValid) {
				attackedBlocks.insert({ r,c });
			}
			else {
				break;
			}
		}
	}
}
