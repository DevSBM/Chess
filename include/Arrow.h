#pragma once
#include <raylib.h>

class Arrow {
private:
	int start_row, start_col, end_row, end_col;
public:
	Arrow(int start_row, int start_col);
	void setEndPos(int end_row, int end_col);
	void render();
	void DrawAngledRectangle(Vector2 p1, Vector2 p2, float width, Color color, float angle);
	static void DrawArrowhead(Vector2 pos, float angle, float size, Color color);
};