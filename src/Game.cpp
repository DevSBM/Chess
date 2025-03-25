
#include <raylib.h>
#include "../include/Game.h"
#include "../include/piece/Knight.h"
#include "../include/piece/Rook.h"
#include "../include/piece/Bishop.h"
#include "../include/piece/Queen.h"
#include <vector>



Game::Game()
{
	init();
}

Game::~Game() {
	CloseWindow();
	unloadObjects();
}

void Game::init()
{
	Game::currentColor = true;
	
	Game::currentGameState = GameState::NORMAL;

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Chess From Scratch ~ SBM");
	SetTargetFPS(FPS);
	loadObjects();
	setCurrentAttackedBlocks();
	calculateAllValidMoves();
}

void Game::addPiece(Piece* p) {
	pieces.push_back(p);
}

void Game::loadObjects() {
	textureBoard = LoadTexture("./res/images/chess_board_800.png");
	//endgameFont = LoadFontEx("./res/fonts/ViaodaLibre-Regular.ttf", 120, 0, 0);
	endgameFont = LoadFontEx("./res/fonts/trade_winds.ttf", 120, 0, 0);

	for (size_t i = 0; i < 8; i++)
	{
		addPiece(new Pawn(1, i, 0, "./res/images/piece/pb.png", PieceType::PAWN));
	}
	for (size_t i = 0; i < 8; i++)
	{
		addPiece(new Pawn(6, i, 1, "./res/images/piece/pw.png", PieceType::PAWN));
	}

	addPiece(new Rook(0, 0, 0, "./res/images/piece/rb.png", PieceType::ROOK));
	addPiece(new Rook(0, 7, 0, "./res/images/piece/rb.png", PieceType::ROOK));

	addPiece(new Rook(7, 0, 1, "./res/images/piece/rw.png", PieceType::ROOK));
	addPiece(new Rook(7, 7, 1, "./res/images/piece/rw.png", PieceType::ROOK));

	addPiece(new Knight(0, 1, 0, "./res/images/piece/nb.png", PieceType::KNIGHT));
	addPiece(new Knight(0, 6, 0, "./res/images/piece/nb.png", PieceType::KNIGHT));

	addPiece(new Knight(7, 1, 1, "./res/images/piece/nw.png", PieceType::KNIGHT));
	addPiece(new Knight(7, 6, 1, "./res/images/piece/nw.png", PieceType::KNIGHT));

	addPiece(new Bishop(0, 2, 0, "./res/images/piece/bb.png", PieceType::BISHOP));
	addPiece(new Bishop(0, 5, 0, "./res/images/piece/bb.png", PieceType::BISHOP));

	addPiece(new Bishop(7, 2, 1, "./res/images/piece/bw.png", PieceType::BISHOP));
	addPiece(new Bishop(7, 5, 1, "./res/images/piece/bw.png", PieceType::BISHOP));

	addPiece(new Queen(0, 3, 0, "./res/images/piece/qb.png", PieceType::QUEEN));
	addPiece(new Queen(7, 3, 1, "./res/images/piece/qw.png", PieceType::QUEEN));


	whitePromotionPieces.push_back(new Queen(0, 2, 1, "./res/images/piece/qw.png", PieceType::QUEEN));
	whitePromotionPieces.push_back(new Rook(0, 3, 1, "./res/images/piece/rw.png", PieceType::ROOK));
	whitePromotionPieces.push_back(new Bishop(0, 4, 1, "./res/images/piece/bw.png", PieceType::BISHOP));
	whitePromotionPieces.push_back(new Knight(0, 5, 1, "./res/images/piece/nw.png", PieceType::KNIGHT));

	blackPromotionPieces.push_back(new Queen(7, 2, 0, "./res/images/piece/qb.png", PieceType::QUEEN));
	blackPromotionPieces.push_back(new Rook(7, 3, 0, "./res/images/piece/rb.png", PieceType::ROOK));
	blackPromotionPieces.push_back(new Bishop(7, 4, 0, "./res/images/piece/bb.png", PieceType::BISHOP));
	blackPromotionPieces.push_back(new Knight(7, 5, 0, "./res/images/piece/nb.png", PieceType::KNIGHT));




	BLACK_KING = new King(0, 4, 0, "./res/images/piece/kb.png", PieceType::KING);
	addPiece(BLACK_KING);

	WHITE_KING = new King(7, 4,1, "./res/images/piece/kw.png", PieceType::KING);
	addPiece(WHITE_KING);
}

void Game::unloadObjects() {
	UnloadTexture(textureBoard);
	for (auto p : pieces) {
		delete p;
	}
	pieces.clear();
}

void Game::run() {
	// Game Loop
	while (!WindowShouldClose()) {
		update();
		render();
	}
}

void Game::clearArrows() {
	for (auto& arr : arrows) {
		delete arr;
	}
	arrows.clear();

}

void Game::calculateAllValidMoves() {
	for (auto p : pieces) {
		if (p->pieceColor == Game::currentColor) {
			p->storeValidMoves();
		}
	}
}

void Game::promoteNow(Piece* promoPiece) {
	// Remove Current Piece (pawn) & replace it with promoted piece

	PieceType t = promoPiece->type;
	for (int i = pieces.size() - 1; i>=0 ; i--) {
		Piece* p = pieces[i];
		if (p->row == promotionPos.first && p->col == promotionPos.second) {
			pieces.erase(pieces.begin()+i);
		}
	}
	int r = promotionPos.first,c=promotionPos.second;
	std::string promoImage = promoPiece->imageLoc;
	if (t == PieceType::QUEEN) {
		pieces.push_back(new Queen(r,c,currentColor, promoImage,t));
	}
	else if (t == PieceType::ROOK) {
		pieces.push_back(new Rook(r, c, currentColor, promoImage, t));
	}
	else if (t == PieceType::BISHOP) {
		pieces.push_back(new Bishop(r, c, currentColor, promoImage, t));
	}
	else if (t == PieceType::KNIGHT) {
		pieces.push_back(new Knight(r, c, currentColor, promoImage, t));
	}
}

void Game::handleInputInPromotionState() {
	std::vector<Piece*>* currentPromotionPieces=nullptr;

	if (this->currentColor == WHITE_COLOR) {
		currentPromotionPieces = &whitePromotionPieces;
	}
	else {
		currentPromotionPieces = &blackPromotionPieces;
	}

	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && currentPromotionPieces!=nullptr) {
		Vector2 mousePos = GetMousePosition();
		clearArrows();

		for (auto p : *currentPromotionPieces) {
			if (p->isSelectable(currentColor, mousePos.x, mousePos.y)) {
				currentGameState = GameState::NORMAL;
				promoteNow(p);
				changeTurn();
				return;
			}
		}
	}
}

void Game::handleInput() {
	if (currentGameState == GameState::PROMOTION) {
		handleInputInPromotionState();
		return;
	}
	if (!((currentGameState == GameState::NORMAL) || (currentGameState == GameState::CHECK))) {
		return;
	}
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		Vector2 mousePos = GetMousePosition();
		clearArrows();
		if (activePiece == nullptr) {
			for (auto p : pieces) {
				if (p->isSelectable(currentColor, mousePos.x, mousePos.y)) {
					activePiece = p;
					break;
				}
			}
		}
		else {
			activePiece->move(mousePos.x,mousePos.y);
			activePiece->renderValidMoves();
		}
		
	}
	else {
		if (activePiece != nullptr) {
			activePiece->place();
			activePiece = nullptr;
		}
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		if (currentArrow == nullptr) {
			Vector2 mousePos = GetMousePosition();
			currentArrow = new Arrow(getRowForY(mousePos.y),getColForX(mousePos.x));
		}
;	}
	else {
		if (currentArrow != nullptr) {
			Vector2 mousePos = GetMousePosition();
			currentArrow->setEndPos(getRowForY(mousePos.y), getColForX(mousePos.x));
			arrows.push_back(currentArrow);
			currentArrow = nullptr;
		}
	}

}

int Game::getRowForY(int y) {
	return y / Game::CELL_SIZE;
}


int Game::getColForX(int x)
{
	return x / Game::CELL_SIZE;
}

void Game::update() {
	handleInput();
}

void Game::renderPieces() {
	if (activePiece != nullptr) {
		activePiece->highlightHoverSquare();
		activePiece->renderValidMoves();
		activePiece->renderCaptureMoves();
		activePiece->renderEnPassant();
		activePiece->renderCastlingMoves();
	}
	for (auto p : pieces) {
		if (p != activePiece) {
			p->render();
		}
	}

	if (activePiece != nullptr) {
		activePiece->render();
	}

	for (auto arr : arrows) {
		arr->render();
	}
}


void Game::setCurrentAttackedBlocks()
{
	currentAttackedBlocks.clear();
	WHITE_KING->isChecked = false;
	BLACK_KING->isChecked = false;
	WHITE_KING->checkingPieces.clear();
	BLACK_KING->checkingPieces.clear();

	for (Piece* p : pieces) {
		if (p->pieceColor != currentColor) {
			p->addToCurrentAttackedBlocks(currentAttackedBlocks);
		}
	}
}

int Game::getTotalMovePossible() {
	int totalMoves = 0;
	//std::cout << "Getting Total Moves\n";
	for (Piece* p:pieces) {
		if (p->pieceColor == currentColor) {
			totalMoves += (p->validMoves.size() + p->captureMoves.size());
			//std::cout << p->row << " " << p->col << " " << totalMoves << std::endl;
		}
	}

	return totalMoves;
}

void Game::checkIfGameOver() {
	int totalMoves = getTotalMovePossible();
	//std::cout <<"Total Moves: " << totalMoves<<std::endl;
	if (totalMoves == 0) {
		if (currentGameState == GameState::CHECK) {
			std::cout << "Checkmate!";
			checkmatedBy = !currentColor;
			currentGameState = GameState::CHECKMATE;
		}
		else {
			std::cout << "Stalemate!";
			currentGameState = GameState::STALEMATE;
		}
	}
	else if (pieces.size() == 2) {
		bool isDraw = true;
		for (Piece* p : pieces) {
			if (p->type != PieceType::KING) {
				isDraw = false;
			}
		}
		if (isDraw) {
			currentGameState = GameState::DRAW;
		}
	}
}

void Game::changeTurn() {
	Game::currentColor = !Game::currentColor;
	Game::currentGameState = GameState::NORMAL;

	setCurrentAttackedBlocks();

	if (Game::currentColor == WHITE_COLOR) {
		std::cout << "White King Checked: " << WHITE_KING->isChecked << "\n";
		if (WHITE_KING->isChecked) {
			Game::currentGameState = GameState::CHECK;
			std::cout << "Checked By " << WHITE_KING->checkingPieces.size() << "\n";

			// Store Checked Path by the checking pieces
			for (Piece* p : WHITE_KING->checkingPieces) {
				p->storeCheckedPath(WHITE_KING);
			}

		}
	}
	else {
		std::cout << "Black King Checked: " << BLACK_KING->isChecked<<"\n";
		if (BLACK_KING->isChecked) {
			Game::currentGameState = GameState::CHECK;
			std::cout << "Checked By " << BLACK_KING->checkingPieces.size() << "\n";

			// Store Checked Path by the checking pieces
			for (Piece* p : BLACK_KING->checkingPieces) {
				p->storeCheckedPath(BLACK_KING);
			}
		}
	}

	calculateAllValidMoves();
	checkIfGameOver();
}

void Game::renderIfPromotionState() {

	if (this->currentGameState == GameState::PROMOTION) {



		if (this->currentColor == WHITE_COLOR) {
			DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, { 0,0,0,180 });
			for (auto p : whitePromotionPieces) {
				p->render();
			}
		}
		else {
			DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, { 255,255,255,180 });
			for (auto p : blackPromotionPieces) {
				p->render();
			}
		}
		
	}
	
}

void Game::renderOtherStates() {
	if (currentGameState == GameState::CHECKMATE || currentGameState == GameState::STALEMATE || currentGameState == GameState::DRAW) {

		DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, { 0,0,0,200 });
		std::string txt = "";
		if (currentGameState == GameState::CHECKMATE) {
			if (checkmatedBy == WHITE_COLOR) {
				txt = "WHITE WON";
			}
			else {
				txt = "BLACK WON";
			}
		}

		else if (currentGameState == GameState::STALEMATE) {
			txt = "STALEMATE";
		}

		else if (currentGameState == GameState::DRAW) {
			txt = "DRAW";
		}

		const int FONT_SIZE = 120;
		Vector2 textSize = MeasureTextEx(endgameFont, txt.c_str(), FONT_SIZE, 1);
		Vector2 position = { (WINDOW_WIDTH - textSize.x) / 2, 300 };

		DrawTextEx(endgameFont, txt.c_str(), position, FONT_SIZE, 1, WHITE);

	}
	
}

void Game::renderIfCheckState() {
	if (currentGameState == GameState::CHECK) {
		if (currentColor == WHITE_COLOR) {
			if (WHITE_KING->isChecked) {
				float ry = (WHITE_KING->row * Game::CELL_SIZE);
				float rx = (WHITE_KING->col  * Game::CELL_SIZE);
				DrawRectangle(rx, ry, Game::CELL_SIZE, Game::CELL_SIZE, { 240,80,80,150 });
			}
		}
		else {
			if (BLACK_KING->isChecked) {
				float ry = (BLACK_KING->row * Game::CELL_SIZE);
				float rx = (BLACK_KING->col * Game::CELL_SIZE);
				DrawRectangle(rx, ry, Game::CELL_SIZE, Game::CELL_SIZE, { 240,80,80,150 });
			}
		}
	}
}

void Game::render() {
	BeginDrawing();
	ClearBackground(BLACK);
	DrawTexture(textureBoard,0,0,WHITE);
	renderIfCheckState();
	renderPieces();
	renderIfPromotionState();
	renderOtherStates();
	EndDrawing();
}




