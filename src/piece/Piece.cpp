#include <iostream>
#include <raylib.h>

#include "../../include/piece/Piece.h"
#include "../../include/piece/King.h"
#include "../../include/piece/PieceType.h"
#include "../../include/Game.h";

Piece::Piece(int row,int col,bool pieceColor,std::string imageLoc,PieceType type) {

	loadObjects(imageLoc);
	this->row = row;
	this->col = col;
	this->pieceColor = pieceColor;
	this->type = type;
	this->imageLoc = imageLoc;
	setX();
	setY();
}

Piece::~Piece() {
	unloadObjects();
}

void Piece::loadObjects(std::string imageLoc) {
	texturePiece = LoadTexture(imageLoc.c_str());
}

void Piece::unloadObjects() {
	UnloadTexture(texturePiece);
}

void Piece::render() {
	DrawTexture(texturePiece,currentX,currentY,WHITE);
}

void Piece::update(Vector2 mousePos) {

}

void Piece::setX() {
	currentX = this->col * Game::CELL_SIZE;
}

void Piece::setY() {
	currentY = this->row * Game::CELL_SIZE;
}

bool Piece::isSelectable(bool currentColor,int x,int y) {
	int mouseRow = y / Game::CELL_SIZE;
	int mouseCol = x/ Game::CELL_SIZE;
	if (this->pieceColor == currentColor && this->row==mouseRow && this->col == mouseCol) {
		return true;
	}
	return false;
}

bool Piece::canMove()
{
	int r = getRowForX();
	int c = getColForY();

	if (r < 0 || c < 0 || r>7 || c>7) {
		return false;
	}

	for (auto v : validMoves) {
		if (v.first == r && v.second == c) {
			return true;
		}
	}

	if (this->type == PieceType::KING) {
		for (auto v : castlingMoves) {
			if (v.first == r && v.second == c) {
				// Also move the rook
				Piece* rookToMove=nullptr;

				// Right Rook
				if (c == this->col+2) {
					for (Piece* p : Game::pieces) {
						if (p->pieceColor == this->pieceColor && p->type == PieceType::ROOK && p->col==7) {
							rookToMove = p;
							break;
						}
					}
					if (rookToMove != nullptr) {
						std::cout << "Right Castling..." << std::endl;
						rookToMove->col = this->col + 1;
						rookToMove->setX();
					}
				}
				// Left Rook
				else if(c==this->col-2) {
					for (Piece* p : Game::pieces) {
						if (p->pieceColor == this->pieceColor && p->type == PieceType::ROOK && p->col == 0) {
							rookToMove = p;
							break;
						}
					}
					if (rookToMove != nullptr) {
						std::cout << "Left Castling..." << std::endl;
						rookToMove->col = this->col - 1;
						rookToMove->setX();
					}
				}
				
				return true;
			}
		}
	}
	

	return false;
}

bool Piece::canCapture()
{
	int r = getRowForX();
	int c = getColForY();
	for (auto v : captureMoves) {
		if (v.first == r && v.second == c) {
			return true;
		}
	}

	// Remove enPassant piece
	if (isEnPassant) {
		if (r == enPassantPos.first && c == enPassantPos.second) {
			for (int i = Game::pieces.size() - 1; i >= 0; i--) {
				Piece* p = Game::pieces[i];
				if (p == enPassantPiece) {
					Game::pieces.erase(Game::pieces.begin() + i);
					return true;
				}
			}
		}
	}
	

	return false;
}

void Piece::storeValidMoves()
{
}

void Piece::storeCheckedPath(Piece* king)
{
	if (this->type == PieceType::KNIGHT || this->type == PieceType::PAWN) {
		return;
	}
	checkedPath.clear();

	// Determine direction of attack
	int rowDir = (this->row > king->row) ? -1 : (this->row < king->row) ? 1 : 0;
	int colDir = (this->col > king->col) ? -1 : (this->col < king->col) ? 1 : 0;

	int r = row, c = col;
	while (!(r == king->row && c == king->col)) {
		r += rowDir;
		c += colDir;
		if (!(r == king->row && c == king->col)) {
			checkedPath.insert({ r,c });
		}
	}

}

void Piece::storeCastlingMoves()
{
}

Piece* Piece::isPinned(Piece* king) {
	// Check if there is any other piece between current piece and king
	// If there are >=1 piece between then we return false
	// After that also check if in the same line(opposite row/col dir) between piece and king 
	// with no piece in between there is a
	// opponent attacking piece of type queen/bishop/rook
	// rook if (0,1/1,0/-1,0/0,-1)
	// bishop if (1,1/-1,1/1,-1/-1,-1)
	// queen combination of both
	// if yes the current piece is not pinned otherwise its pinned
	// 
	// Also first check if piece is pinnable like its horizontal/vertical/diagonal
	// to the king If not return false as it not pinnable
	// 
	
	// See if the piece is pinnable
	int rowDiff = abs(this->row - king->row);
	int colDiff = abs(this->col - king->col);
	bool isDiagonal = rowDiff == colDiff;

	// Not pinnable
	if (!(rowDiff == 0 || colDiff == 0 || rowDiff == colDiff)) {
		return nullptr;
	}

	// Determine direction between piece & king
	int rowDir = (this->row > king->row) ? -1 : (this->row < king->row) ? 1 : 0;
	int colDir = (this->col > king->col) ? -1 : (this->col < king->col) ? 1 : 0;

	// Get all pieces of current color coordinate in a set
	std::set<std::pair<int, int>> pieceCoords;

	for (Piece* p : Game::pieces) {
		if (p != this && p!=king) {
			pieceCoords.insert({ p->row,p->col });
		}
	}
	


	// After getting that we can easily check if it falls in the path between
	// pinned piece and king
	int r = this->row, c = this->col;
	while (!(r == king->row && c == king->col)) {
		r += rowDir;
		c += colDir;
		
		if (pieceCoords.find({ r,c }) != pieceCoords.end()) {
			return nullptr;
		}
	}
	
	
	// To check if there is an enemy piece on the same line we move in opposite direction
	// if there is any piece of same color then we stop iteration and return false
	rowDir *= -1; colDir *= -1;
	r = this->row, c = this->col;

	while (r>=0 && c>=0 && r<=7 && c <=7) {
		r += rowDir;
		c += colDir;

		for (Piece* p : Game::pieces) {
			if (p != this && p != king && p->row==r && p->col==c) {
				// Another piece of same color in front
				if (this->pieceColor == p->pieceColor) {
					return nullptr;
				}
				else {
					if (isDiagonal) {
						if (p->type == PieceType::BISHOP || p->type == PieceType::QUEEN) {
							return p;
						}
						else {
							return nullptr;
						}
					}
					else if(!isDiagonal) {
						if (p->type == PieceType::ROOK || p->type == PieceType::QUEEN) {
							return p;
						}
						else {
							return nullptr;
						}
					}
				}
			}

		}
	}


	return nullptr;
}


void Piece::filterMoves() {
	if (Game::currentGameState == GameState::NORMAL) {
		filterNormalStatePinnedMoves();
	}
	else if (Game::currentGameState == GameState::CHECK) {
		filterCheckedStateValidMoves();
	}
}

void Piece::filterNormalStatePinnedMoves() {
	King* king;
	if (Game::currentColor == Game::WHITE_COLOR) {
		king = Game::WHITE_KING;
	}
	else {
		king = Game::BLACK_KING;
	}

	Piece* pinnedBy = isPinned(king);

	// Return if there is no pin by opponent pieces
	if (pinnedBy!=nullptr) {
		// Dont remove valid moves all at once 
		// As we need to get all the moves between king 
		// and the checking piece If any of the valid moves
		// is in that set of moves we need to keep it
		//validMoves.clear();

		std::set<std::pair<int, int>> betweenMoves;

		int rowDir = (pinnedBy->row > king->row) ? -1 : (pinnedBy->row < king->row) ? 1 : 0;
		int colDir = (pinnedBy->col > king->col) ? -1 : (pinnedBy->col < king->col) ? 1 : 0;

		int r = pinnedBy->row, c = pinnedBy->col;

		// Get all the between moves
		while (!(r == king->row && c == king->col)) {
			r += rowDir;
			c += colDir;

			if (!(r == king->row && c == king->col)) {
				betweenMoves.insert({ r,c });
			}
		}

		for (int i = validMoves.size() - 1; i >= 0;i--) {
			auto move = validMoves[i];
			// If move is not between pinnedBy piece and king
			if (betweenMoves.find(move) == betweenMoves.end()) {
				validMoves.erase(validMoves.begin()+i);
			}
		}
	}
	else {
		return;
	}

	// Also needs to clear captured moves except checking piece if it contains in captureMoves
	bool canCaptureCheckingPiece = false;
	for (auto move : captureMoves) {
		if (move.first == pinnedBy->row && move.second == pinnedBy->col) {
			canCaptureCheckingPiece = true;
			break;
		}
	}
	captureMoves.clear();
	if (canCaptureCheckingPiece) {
		captureMoves.push_back({pinnedBy->row,pinnedBy->col});
	}
}

// In check move filter for all pieces
void Piece::filterCheckedStateValidMoves()
{
	King* kingInCheck;
	if (Game::currentColor == Game::WHITE_COLOR) {
		kingInCheck = Game::WHITE_KING;
	}
	else {
		kingInCheck = Game::BLACK_KING;
	}

	// Double or more checks
	if (kingInCheck->checkingPieces.size() > 1) {
		validMoves.clear();
		captureMoves.clear();
		return;
	}

	// As Pawn and Knight checks cant be blocked
	// They can only be captured
	for (Piece* p : kingInCheck->checkingPieces) {
		if (p->type == PieceType::PAWN || p->type == PieceType::KNIGHT) {
			validMoves.clear();
		}
	}

	// Filter Valid and Capure moves based on attacking path of attacking pieces
	// Will filter Valid Moves based on attacking path
	// Will filter capture moves based on attacking piece coordinate
	for(Piece* p:kingInCheck->checkingPieces) {
		for (int i = validMoves.size() - 1; i >= 0;i--) {
			auto move = validMoves[i];
			// Check for the move in checkedPath set of the attacking piece
			// If its not in set then remove the valid move
			if (p->checkedPath.find(move) == p->checkedPath.end()) {
				validMoves.erase(validMoves.begin()+i);
			}
		}
	}

	for (int i = captureMoves.size() - 1; i >= 0; i--) {
		auto move = captureMoves[i];
		bool isAttackPiece = false;
		for (Piece* p : kingInCheck->checkingPieces) {
			if (p->row == move.first && p->col == move.second) {
				isAttackPiece = true;
			}
		}

		if (!isAttackPiece) {
			captureMoves.erase(captureMoves.begin() + i);
		}
	}

	// Now check for pinned pieces; pinned pieces should not be able to move
	// if pinned valid moves should be cleared
	// We could have ignored this and made the rule so that we able capture king
	// and if king is captured game is over but lets stick to rules as it brings
	// other complications like pinned piece checking opponent king
	if (isPinned(kingInCheck)) {
		validMoves.clear();
	}
}

void Piece::renderValidMoves()
{
	for (auto v : validMoves) {
		float cy = (v.first * Game::CELL_SIZE) + (Game::CELL_SIZE/2);
		float cx = (v.second * Game::CELL_SIZE) + (Game::CELL_SIZE / 2);
		DrawCircle(cx, cy, 14, {0,0,0,50});
	}
	return;
}

void Piece::addToCurrentAttackedBlocks(std::set<std::pair<int, int>>& attackedBlocks)
{
}

void Piece::renderCaptureMoves()
{
	for (auto v : captureMoves) {
		float ry = (v.first * Game::CELL_SIZE);
		float rx = (v.second * Game::CELL_SIZE);
		DrawRectangle(rx, ry, Game::CELL_SIZE, Game::CELL_SIZE, {200,120,120,100});
	}
	return;
}

void Piece::resetPos() {
	setX();
	setY();
}

bool Piece::promoteIfPawn() {

	// Remove Current piece(pawn) and Replace it with choosen promotion
	// After choosing promotion call changeTurn method in Game
	if (this->type == PieceType::PAWN) {
		if (this->pieceColor == Game::WHITE_COLOR && this->row == 0) {
			Game::currentGameState = GameState::PROMOTION;
			Game::promotionPos.first = this->row;
			Game::promotionPos.second = this->col;

			return true;
		}
		else if (this->pieceColor == Game::BLACK_COLOR && this->row == 7) {
			Game::currentGameState = GameState::PROMOTION;
			Game::promotionPos.first = this->row;
			Game::promotionPos.second = this->col;

			return true;
		}
	}
	return false;
}

void Piece::moveToCurrentPos() {
	row = getRowForX();
	col = getColForY();
	setX();
	setY();
	if (!hasMoved) {
		hasMoved = true;
	}

	// Check for promotion if its a pawn
	bool isPromotion = promoteIfPawn();
	if (isPromotion) {
		return;
	}

	Game::changeTurn();
}

void Piece::removeCapturedPiece() {
	int r = getRowForX();
	int c = getColForY();
	for (int i = 0; i < Game::pieces.size();i++) {
		Piece* p = Game::pieces[i];
		if (p->pieceColor!=this->pieceColor && p->row == r && p->col == c) {
			Game::pieces.erase(Game::pieces.begin()+i);
			return;
		}
	}
}

void Piece::setEnPassant() {
	int leftCol = this->col - 1,rightCol=this->col+1;

	for (Piece* p:Game::pieces) {
		// Also check if the enPassant pos is not blocked by opp color piece
		// No need to check as in that case it will not be possible to move the pawn
		if (p->pieceColor != this->pieceColor && p->type == PieceType::PAWN && p->row == this->row && (p->col == leftCol || p->col == rightCol)) {
			p->isEnPassant = true;
			p->enPassantPos = { this->row + (this->pawnDir*-1),this->col };
			p->enPassantPiece = this;
		}
	}
}

void Piece::resetEnPassant() {
	for (Piece* p : Game::pieces) {
		p->isEnPassant = false;
	}
}

void Piece::renderEnPassant()
{
	if (this->type == PieceType::PAWN && this->isEnPassant) {
		float ry = (this->enPassantPos.first * Game::CELL_SIZE);
		float rx = (this->enPassantPos.second * Game::CELL_SIZE);
		DrawRectangle(rx, ry, Game::CELL_SIZE, Game::CELL_SIZE, { 200,120,120,100 });
	}
}

void Piece::renderCastlingMoves()
{
	if (this->type == PieceType::KING) {
		for (auto v : castlingMoves) {
			/*float cy = (v.first * Game::CELL_SIZE) + (Game::CELL_SIZE / 2);
			float cx = (v.second * Game::CELL_SIZE) + (Game::CELL_SIZE / 2);
			DrawCircle(cx, cy, 14, { 0,50,0,50 });*/

			float ry = (v.first * Game::CELL_SIZE);
			float rx = (v.second * Game::CELL_SIZE);
			DrawRectangle(rx, ry, Game::CELL_SIZE, Game::CELL_SIZE, { 120,200,120,100 });
		}
	}
}


void Piece::place() {
	if (canMove()) {
		//std::cout << "Moving...";

		int prevRow = this->row;

		moveToCurrentPos();

		resetEnPassant();
		if (this->type == PieceType::PAWN && abs(this->row - prevRow) == 2) {
			setEnPassant();
		}
	}
	else if (canCapture()) {
		//std::cout << "Capturing...";
		removeCapturedPiece();
		moveToCurrentPos();

		resetEnPassant();
	}
	else {
		resetPos();
	}
}

int Piece::getRowForX() {
	return (currentY + (Game::CELL_SIZE / 2)) / Game::CELL_SIZE;
}

int Piece::getColForY() {
	return (currentX + (Game::CELL_SIZE / 2)) / Game::CELL_SIZE;
}

void Piece::highlightHoverSquare() {
	float r = getRowForX();
	float c = getColForY();
	DrawRectangleLinesEx({ c * Game::CELL_SIZE, r * Game::CELL_SIZE, Game::CELL_SIZE, Game::CELL_SIZE },5,{230,230,230,255});
}

void Piece::move(int x, int y)
{
	this->currentX = x - (Game::CELL_SIZE/2);
	this->currentY = y - (Game::CELL_SIZE / 2);
}
