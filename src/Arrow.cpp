#include "../include/Arrow.h"
//#include<iostream>
#include <raylib.h>
#include "../include/Game.h";

Arrow::Arrow(int start_row, int start_col):start_row(start_row), start_col(start_col), end_row(start_row), end_col(start_col){}

void Arrow::setEndPos(int end_row, int end_col)
{
	this->end_row = end_row;
	this->end_col = end_col;
}

void Arrow::render()
{
	if (start_row == end_row && start_col == end_col) {
		return;
	}
	float x = (start_col * Game::CELL_SIZE) + Game::CELL_SIZE / 2;
	float y = (start_row * Game::CELL_SIZE) + Game::CELL_SIZE / 2;

	float endX = (end_col * Game::CELL_SIZE) + Game::CELL_SIZE / 2;
	float endY = (end_row * Game::CELL_SIZE) + Game::CELL_SIZE / 2;

	//std::cout << start_col << " " << start_row << " " << end_col << " " << end_row << std::endl;
	//std::cout << x << " " << y << " " << endX << " " << endY << std::endl;

	Color arrowColor = { 200, 161, 0, 255 };

	bool isKnight = abs(start_row - end_row) * abs(start_col - end_col) == 2;

	if (isKnight) {

		if (abs(start_row - end_row) == 2) {
			float angle = atan2f(endY - y, x - x) * RAD2DEG;
			float angle2 = atan2f(endY - endY, endX - x) * RAD2DEG;

			int offsetY = -9;
			if (endY > y) {
				offsetY = 9;
			}


			DrawAngledRectangle({ x,y }, { x,endY+offsetY }, 18, arrowColor, angle);
			DrawAngledRectangle({ x,endY }, { endX,endY }, 18, arrowColor, angle2);
			DrawArrowhead({ endX,endY }, angle2, 24, arrowColor);
		}
		else {
			float angle = atan2f(y - y, endX - x) * RAD2DEG;
			float angle2 = atan2f(endY - y, endX - endX) * RAD2DEG;

			int offsetX = -9;
			if (endX > x) {
				offsetX = 9;
			}


			DrawAngledRectangle({ x,y }, { endX+offsetX,y }, 18, arrowColor, angle);
			DrawAngledRectangle({ endX,y }, { endX,endY }, 18, arrowColor, angle2);
			DrawArrowhead({ endX,endY }, angle2, 24, arrowColor);
		}

	}

	else {

		float angle = atan2f(endY - y, endX - x) * RAD2DEG;

		DrawAngledRectangle({ x,y }, { endX,endY }, 18, arrowColor, angle);


		DrawArrowhead({ endX,endY }, angle, 24, arrowColor);
	}


	
}


void Arrow::DrawAngledRectangle(Vector2 p1, Vector2 p2, float width, Color color,float angle) {
	// Compute the center
	Vector2 center = { (p1.x + p2.x) / 2, (p1.y + p2.y) / 2 };

	// Compute the angle in degrees
	//float angle = atan2f(p2.y - p1.y, p2.x - p1.x) * RAD2DEG;

	// Compute the length of the rectangle (distance between p1 and p2)
	float length = sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));

	Rectangle rect = { center.x, center.y, length, width };

	DrawRectanglePro(rect,  { length / 2, width / 2 }, angle, color);
}


void Arrow::DrawArrowhead(Vector2 pos, float angle, float size, Color color) {
	// Compute three points for the arrowhead using trigonometry
	//Vector2 tip = { pos.x + cosf(angle) * size, pos.y + sinf(angle) * size }; // Tip of arrow
	//Vector2 left = { pos.x + cosf(angle + 140 * DEG2RAD) * size * 0.6f,
	//				pos.y + sinf(angle + 140 * DEG2RAD) * size * 0.6f }; // Left wing
	//Vector2 right = { pos.x + cosf(angle - 140 * DEG2RAD) * size * 0.6f,
	//				 pos.y + sinf(angle - 140 * DEG2RAD) * size * 0.6f }; // Right wing

	//// Ensure the correct winding order (counterclockwise)
	
	//DrawTriangle(tip, left, right, color);
	//DrawTriangleLines(tip, left, right, color);
	DrawPoly(pos, 3, size, angle, color);
}



