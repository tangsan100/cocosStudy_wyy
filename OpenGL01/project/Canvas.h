#pragma once

#include "GTMath.h"

namespace GT {
	struct RGBA
	{
		byte m_b;
		byte m_g;
		byte m_r;
		byte m_a;

		RGBA(byte r = 255, byte g = 255, byte b = 255, byte a = 255) :m_r(r), m_g(g), m_b(b), m_a(a) {

		}

	};

	struct Point {
		int _x;
		int _y;
		RGBA _color;

		Point(int x=0, int y=0, RGBA color=RGBA(255,255,255,255)):_x(x),_y(y),_color(color){

		}
	};

	class Canvas
	{
	private:
		int wWidth;
		int wHeight;
		RGBA* buffer;

	public:

		Canvas(int width, int height, void* buffer);

		void drawPoint(int x,int y, RGBA rgb);

		void clear();

		void brensenhamLine(Point p1, Point p2);

		RGBA getPointColor(RGBA color0, RGBA color1, float scale);

		// 画三角形
		void drawTriangle(Point pt1, Point pt2, Point pt3);

		// 画平底三角形
		void drawTriangleFlat(Point flat1, Point flat2, Point pt);
	};
}



