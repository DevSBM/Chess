#include "../../include/piece/Pawn.h";
#include "../../include/Game.h"

Pawn::Pawn(int row, int col, bool pieceColor, std::string imageLoc, PieceType type):Piece(row,col,pieceColor,imageLoc,type)
{
	if (!pieceColor) {
		pawnDir = 1;
	}
}

//bool Pawn::canMove() {
//	if (!Piece::canMove()) {
//		return false;
//	}
//	int r = getRowForX();
//	int c = getColForY();
//	for (auto v : validMoves) {
//		if (v.first == r && v.second == c) {
//			return true;
//		}
//	}
//	return false;
//}
//
//bool Pawn::canCapture() {
//	int r = getRowForX();
//	int c = getColForY();
//	for (auto v : captureMoves) {
//		if (v.first == r && v.second == c) {
//			return true;
//		}
//	}
//
//	return false;
//}

void Pawn::storeValidMoves() {
	validMoves.clear();
	captureMoves.clear();

	int maxMove = 1;
	if (!hasMoved) {
		maxMove = 2;
	}

	// Pawn Front Moves
	for (int i = 1; i <= maxMove; i++) {
		bool isValid = true;
		int pawnRow = this->row + (pawnDir * i);
		for (Piece* p : Game::pieces) {
			if (pawnRow == p->row && this->col==p->col) {
				isValid = false;
				break;
			}
		}
		if (isValid) {
			validMoves.push_back({ pawnRow,this->col });
		}
		else {
			// As we cant move one move it will not be possible
			// to move two moves
			break;
		}
	}

	// Pawn Diagonal Captures
	int pawnRow = this->row + pawnDir;
	for (int i = -1; i <= 1; i+=2) {
		int pawnCol = this->col + i;
		if (pawnCol < 0 || pawnCol>7) {
			continue;
		}
		bool isValid = false;
		for (Piece* p : Game::pieces) {
			if (p->pieceColor!=this->pieceColor && pawnRow == p->row && pawnCol == p->col) {
				isValid = true;
				break;
			}
		}
		if (isValid) {
			captureMoves.push_back({ pawnRow,pawnCol });
		}
	}

	/*std::cout << "Front Moves" << std::endl;
	for (auto v : validMoves) {
		std::cout << v.first << " " << v.second << std::endl;
	}
	std::cout << "Capture Moves" << std::endl;
	for (auto v : captureMoves) {
		std::cout << v.first << " " << v.second << std::endl;
	}*/

	/*if (Game::currentGameState == GameState::CHECK) {
		filterCheckedStateValidMoves();
	}*/

	// Filter moves if checked and pinned
	filterMoves();
}

void Pawn::addToCurrentAttackedBlocks(std::set<std::pair<int, int>>& attackedBlocks)
{
	int pawnRow = this->row + pawnDir;
	for (int i = -1; i <= 1; i += 2) {
		int pawnCol = this->col + i;
		if (pawnCol < 0 || pawnCol>7) {
			continue;
		}

		for (Piece* p : Game::pieces) {
			if (pawnRow == p->row && pawnCol == p->col && p->type == PieceType::KING && p->pieceColor != this->pieceColor) {
				King* k = (King*)p;
				k->isChecked = true;
				k->checkingPieces.insert(this);
			}
		}
		attackedBlocks.insert({ pawnRow,pawnCol });
	}
}
