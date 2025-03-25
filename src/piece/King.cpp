#include "../../include/piece/King.h";
#include "../../include/Game.h"

King::King(int row, int col, bool pieceColor, std::string imageLoc, PieceType type) :Piece(row, col, pieceColor, imageLoc, type)
{

}

void King::storeValidMoves() {
	validMoves.clear();
	captureMoves.clear();


	for (auto move : moves) {
		int r = row+move.first, c = col+move.second;
		bool isValid = true;

		if (!(r >= 0 && c >= 0 && r <= 7 && c <= 7)) {
			continue;
		}

		if (Game::currentAttackedBlocks.find({ r,c }) != Game::currentAttackedBlocks.end()) {
			continue;
		}

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
		
	}

	storeCastlingMoves();
}

void King::storeCastlingMoves()
{
	// Can't Castle If: 
	// In check
	// King Has Already Moved
	// Rook of the side we want to castle has moved
	// If one of the opponent piece is attacking 
	// any of two moves adjacent to the king we want to castle
	// If there is still pieces other than king and rook 
	// side to the direction we want to castle

	castlingMoves.clear();
	bool hasLeftRookMoved=true, hasRightRookMoved = true;
	bool isLeftCastlingPossible = true, isRightCastlingPossible = true;
	if (!hasMoved && !isChecked) {
		for (Piece* p : Game::pieces) {
			if (p->type==PieceType::ROOK && p->pieceColor == this->pieceColor) {
				if (p->col == 0) {
					hasLeftRookMoved = p->hasMoved;
				}
				else if (p->col == 7) {
					hasRightRookMoved = p->hasMoved;
				}
			}
		}

		if (hasLeftRookMoved && hasRightRookMoved) {
			return;
		}

		// Get Friend Pieces coords
		std::set<std::pair<int, int>> pieceCoords;

		for (Piece* p : Game::pieces) {
			if (p->pieceColor == this->pieceColor) {
				pieceCoords.insert({ p->row,p->col });
			}
		}

		// Little Bug: In the left side we are checking 
		// one extra block for Game::currentAttackedBlocks
		if (!hasLeftRookMoved) {
			for (int c = this->col - 1; c > 0; c--) {
				if (Game::currentAttackedBlocks.find({ row,c }) != Game::currentAttackedBlocks.end()) {
					isLeftCastlingPossible = false;
				}

				if (pieceCoords.find({ row,c }) != pieceCoords.end()) {
					isLeftCastlingPossible = false;
				}
			}
		}
		else {
			isLeftCastlingPossible = false;
		}

		if (!hasRightRookMoved) {
			for (int c = this->col + 1; c < 7; c++) {
				if (Game::currentAttackedBlocks.find({ row,c }) != Game::currentAttackedBlocks.end()) {
					isRightCastlingPossible = false;
				}

				if (pieceCoords.find({ row,c }) != pieceCoords.end()) {
					isRightCastlingPossible = false;
				}
			}
		}
		else {
			isRightCastlingPossible = false;
		}

		if (isLeftCastlingPossible) {
			castlingMoves.push_back({row,col-2});
		}
		if (isRightCastlingPossible) {
			castlingMoves.push_back({ row,col + 2 });
		}
	}
}

void King::addToCurrentAttackedBlocks(std::set<std::pair<int, int>>& attackedBlocks)
{
	for (auto move : moves) {
		int r = row + move.first, c = col + move.second;
		attackedBlocks.insert({ r,c });
	}
}
