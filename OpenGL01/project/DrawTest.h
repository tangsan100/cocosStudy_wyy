#pragma once
#include "Canvas.h"
using namespace GT;

class DrawTest
{
private:
	Canvas* canvas;
public:
	DrawTest(Canvas* ca):canvas(ca){}

	// ��ֱ��
	void drawLine();

	// ��������
	void drawTriangle(Point pt1, Point pt2, Point pt3);

	// ��ƽ��������
	void drawTriangleFlat(Point flat1, Point flat2, Point pt);

	// ������
	void drawRay();

	// ��ѩ����Ļ
	void drawSnowScreen(int wWidth, int wHeight);
};

