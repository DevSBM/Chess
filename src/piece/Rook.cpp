#include "../../include/piece/Rook.h";
#include "../../include/Game.h"

Rook::Rook(int row, int col, bool pieceColor, std::string imageLoc, PieceType type) :Piece(row, col, pieceColor, imageLoc, type)
{

}

void Rook::storeValidMoves() {
	validMoves.clear();
	captureMoves.clear();

	// Row iteration keeping column constant

	for (int r = row+1; r <= 7; r++) {

		bool isValid = true;
		for (Piece* p : Game::pieces) {
			if (r == p->row && col == p->col) {
				if (p->pieceColor != this->pieceColor) {
					captureMoves.push_back({ r,col});
				}
				isValid = false;
				break;
			}
		}

		if (isValid) {
			validMoves.push_back({ r,col});
		}
		else {
			break;
		}
	}

	for (int r = row - 1; r >=0; r--) {
		bool isValid = true;
		for (Piece* p : Game::pieces) {
			if (r == p->row && col == p->col) {
				if (p->pieceColor != this->pieceColor) {
					captureMoves.push_back({ r,col });
				}
				isValid = false;
				break;
			}
		}

		if (isValid) {
			validMoves.push_back({ r,col });
		}
		else {
			break;
		}
	}

	// Col iteration keeping row constant
	for (int c = col+1; c <= 7; c++) {
		bool isValid = true;
		for (Piece* p : Game::pieces) {
			if (row == p->row && c == p->col) {
				if (p->pieceColor != this->pieceColor) {
					captureMoves.push_back({ row,c });
				}
				isValid = false;
				break;
			}
		}

		if (isValid) {
			validMoves.push_back({ row,c });
		}
		else {
			break;
		}
	}

	for (int c = col - 1; c >= 0; c--) {
		bool isValid = true;
		for (Piece* p : Game::pieces) {
			if (row == p->row && c == p->col) {
				if (p->pieceColor != this->pieceColor) {
					captureMoves.push_back({ row,c });
				}
				isValid = false;
				break;
			}
		}

		if (isValid) {
			validMoves.push_back({ row,c });
		}
		else {
			break;
		}
	}

	// Filter moves if checked and pinned
	filterMoves();
}

void Rook::addToCurrentAttackedBlocks(std::set<std::pair<int, int>>& attackedBlocks)
{
	// Row iteration keeping column constant
	for (int r = row + 1; r <= 7; r++) {

		bool isValid = true;
		for (Piece* p : Game::pieces) {
			if (r == p->row && col == p->col) {
				attackedBlocks.insert({ r,col });
				if (p->type == PieceType::KING && p->pieceColor != this->pieceColor) {
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
			attackedBlocks.insert({ r,col });
		}
		else {
			break;
		}
	}

	for (int r = row - 1; r >= 0; r--) {
		bool isValid = true;
		for (Piece* p : Game::pieces) {
			if (r == p->row && col == p->col) {
				attackedBlocks.insert({ r,col });
				if (p->type == PieceType::KING && p->pieceColor != this->pieceColor) {
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
			attackedBlocks.insert({ r,col });
		}
		else {
			break;
		}
	}

	//// Col iteration keeping row constant
	for (int c = col + 1; c <= 7; c++) {
		bool isValid = true;
		for (Piece* p : Game::pieces) {
			if (row == p->row && c == p->col) {
				attackedBlocks.insert({ row,c });
				if (p->type == PieceType::KING && p->pieceColor != this->pieceColor) {
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
			attackedBlocks.insert({ row,c });
		}
		else {
			break;
		}
	}

	for (int c = col - 1; c >= 0; c--) {
		bool isValid = true;
		for (Piece* p : Game::pieces) {
			if (row == p->row && c == p->col) {
				attackedBlocks.insert({ row,c });
				if (p->type == PieceType::KING && p->pieceColor != this->pieceColor) {
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
			attackedBlocks.insert({ row,c });
		}
		else {
			break;
		}
	}
}
