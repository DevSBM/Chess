#pragma once

#include<raylib.h>

#include "PieceType.h";
#include <iostream>
#include <vector>
#include <set>

class Piece {
public:
	Texture2D texturePiece;
	int currentX, currentY;
	int row, col;
	std::vector<std::pair<int,int>> validMoves; // Could have used Set
	std::vector<std::pair<int, int>> captureMoves;
	std::set<std::pair<int, int>> checkedPath;
public:

	std::string imageLoc;

	PieceType type;
	bool pieceColor;  // true for white
	bool hasMoved = false;
	
	int pawnDir = -1;

	bool isEnPassant = false;
	std::pair<int, int> enPassantPos;
	Piece* enPassantPiece;

	std::vector<std::pair<int, int>> castlingMoves;


	Piece(int row, int col, bool pieceColor, std::string imageLoc, PieceType type);
	~Piece();
	void loadObjects(std::string imageLoc);
	void unloadObjects();
	void render();
	void update(Vector2 mousePos);
	void setX();
	void setY();
	bool isSelectable(bool currentColor, int x, int y);
	virtual bool canMove();
	virtual bool canCapture();
	virtual void storeValidMoves();
	virtual void storeCheckedPath(Piece* king);
	virtual void storeCastlingMoves();
	Piece* isPinned(Piece* king);
	void filterMoves();
	void filterNormalStatePinnedMoves();
	void filterCheckedStateValidMoves();
	virtual void renderValidMoves();
	virtual void addToCurrentAttackedBlocks(std::set<std::pair<int,int>>& attackedBlocks);
	void renderCaptureMoves();
	void resetPos();
	bool promoteIfPawn();
	void moveToCurrentPos();
	void removeCapturedPiece();
	
	void setEnPassant();
	void resetEnPassant();
	void renderEnPassant();

	void renderCastlingMoves();

	void place();
	int getRowForX();
	int getColForY();
	void highlightHoverSquare();
	void move(int x, int y);



};