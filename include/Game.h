#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <set>
#include "./piece/Piece.h"
#include "./piece/Pawn.h"
#include "./piece/King.h"
#include "./Arrow.h"
#include "./GameState.h"

class Game {
	private:
		Texture2D textureBoard;
		Piece* activePiece = nullptr;
	public:
		std::vector<Arrow*> arrows;
		Arrow* currentArrow;
		inline static King* BLACK_KING, *WHITE_KING;
		inline static std::vector<Piece*> pieces;
		
		inline static std::vector<Piece*> blackPromotionPieces;
		inline static std::vector<Piece*> whitePromotionPieces;

		
		inline static bool currentColor; // true for white
		inline const static bool WHITE_COLOR = 1;
		inline const static bool BLACK_COLOR = 0;

		inline static std::set<std::pair<int,int>> currentAttackedBlocks;

		inline static GameState currentGameState;
		inline static std::pair<int, int> promotionPos{0,0};


		inline static const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;
		inline static const int CELL_SIZE = 100;
		inline static const int FPS = 60;

		inline static bool checkmatedBy=true;

		inline static Font endgameFont;

		Game();
		~Game();
		void init();
		void addPiece(Piece* p);
		void loadObjects();
		void unloadObjects();
		void run();
		void clearArrows();
		static void calculateAllValidMoves();
		void promoteNow(Piece* promoPiece);
		void handleInputInPromotionState();
		void handleInput();
		void update();
		static void changeTurn();

		void renderPieces();
		void renderIfPromotionState();
		void renderOtherStates();
		void renderIfCheckState();
		void render();

		static int getColForX(int x);
		static int getRowForY(int y);

		static void setCurrentAttackedBlocks();
		
		static int getTotalMovePossible();
		static void checkIfGameOver();
};