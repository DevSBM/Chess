#include "../../include/piece/Queen.h";
#include "../../include/Game.h"

Queen::Queen(int row, int col, bool pieceColor, std::string imageLoc, PieceType type) :Piece(row, col, pieceColor, imageLoc, type)
{

}

void Queen::storeValidMoves() {
	validMoves.clear();
	captureMoves.clear();


	for (auto move : diagonalMoves) {
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

	// Row iteration keeping column constant

	for (int r = row + 1; r <= 7; r++) {

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

	for (int r = row - 1; r >= 0; r--) {
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
	for (int c = col + 1; c <= 7; c++) {
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

void Queen::addToCurrentAttackedBlocks(std::set<std::pair<int, int>>& attackedBlocks)
{
	for (auto move : diagonalMoves) {
		int r = row, c = col;
		while (r >= 0 && c >= 0 && r <= 7 && c <= 7) {
			bool isValid = true;
			r += move.first;
			c += move.second;
			for (Piece* p : Game::pieces) {
				if (r == p->row && c == p->col) {
					attackedBlocks.insert({ r,c });
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
				attackedBlocks.insert({ r,c });
			}
			else {
				break;
			}
		}
	}

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

	// Col iteration keeping row constant
	for (int c = col + 1; c <= 7; c++) {
		bool isValid = true;
		for (Piece* p : Game::pieces) {
			if (row == p->row && c == p->col) {
				attackedBlocks.insert({ row,c});
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
