#include "DrawTest.h"
#include "project.h"
#include <math.h>
#include <stdlib.h>

void DrawTest::drawLine() {
	canvas->brensenhamLine(Point(300, 300,RGBA(255,0,0)), Point(150, 20, RGBA(255, 0, 0)));
}

void DrawTest::drawTriangle(Point pt1, Point pt2, Point pt3) {
	canvas->drawTriangle(pt1, pt2, pt3);
}

void DrawTest::drawTriangleFlat(Point flat1, Point flat2, Point pt) {
	canvas->drawTriangleFlat(flat1, flat2, pt);
}

void DrawTest::drawRay() {
	// 画出一个烟花线
	GT::Point pt1(150, 150,GT::RGBA(255,0,0));
	GT::RGBA color(rand() % 255, rand() % 255, rand() % 255);

	// 圆弧半径100
	float dis = 100;

	for (float i = 0; i <= 360; i += 15) {
		// 计算一个弧度，  弧度= π*θ/180°
		double d = DEG2RAD(i);

		// 通过正余弦函数计算出x,y 的偏移量
		int dx = cos(d)*dis;
		int dy = sin(d)*dis;

		int x = dx + pt1._x;
		int y = dy + pt1._y;


		canvas->brensenhamLine(pt1, GT::Point(x, y,GT::RGBA(0,255,0,255)));
	}
}


void DrawTest::drawSnowScreen(int wWidth, int wHeight) {
	
	for (int x = 0; x < wWidth; ++x) {
		for (int y = 0; y < wHeight; ++y) {
			GT::RGBA color(rand() % 255, rand() % 255, rand() % 255);
			canvas->drawPoint(x, y, color);
		}
	}
}