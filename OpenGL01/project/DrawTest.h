#pragma once
#include "Canvas.h"
using namespace GT;

class DrawTest
{
private:
	Canvas* canvas;
public:
	DrawTest(Canvas* ca):canvas(ca){}

	// 画直线
	void drawLine();

	// 画三角形
	void drawTriangle(Point pt1, Point pt2, Point pt3);

	// 画平底三角形
	void drawTriangleFlat(Point flat1, Point flat2, Point pt);

	// 画射线
	void drawRay();

	// 画雪花屏幕
	void drawSnowScreen(int wWidth, int wHeight);
};

