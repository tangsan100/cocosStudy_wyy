#include "Canvas.h"
#include "project.h"
#include<string.h>
#include<stdlib.h>
#include<iostream>

using namespace std;
using namespace GT;

GT::Canvas::Canvas(int width, int height, void* buffer){

	wWidth = width;
	wHeight = height;
	this->buffer = (RGBA*)buffer;
}


void GT::Canvas::drawPoint( int x, int y, RGBA rgba) {
	buffer[y*wWidth + x] = rgba;
}

void GT::Canvas::clear() {
	memset(buffer, 0, wWidth*wHeight * sizeof(RGBA));
}

RGBA GT::Canvas::getPointColor(RGBA color0, RGBA color1, float scale) {
	int r = color0.m_r + (color1.m_r - color0.m_r)*scale;
	int g = color0.m_g + (color1.m_g - color0.m_g)*scale;
	int b = color0.m_b + (color1.m_b - color0.m_b)*scale;
	int a = color0.m_a + (color1.m_a - color0.m_a)*scale;

	return RGBA(r, g, b, a);
}


/*
* brensenham �����㷨
* p1 ��ʼ��
* p2 �յ�
* color ��ɫ
* ��ʼ p = 2��y-��x
* �� pi >= 0, �� d1 > d2 , yi_1 = yi + 1,   pi_1 = pi + 2��y-2��x
* �� pi< 0 �� d1 < d2 , yi_1 = yi, pi_1 = pi+2��y
*/
void GT::Canvas::brensenhamLine(Point pt1, Point pt2) {

	// ����ƫ����
	int dx = abs(pt1._x - pt2._x);
	int dy = abs(pt1._y - pt2._y);


	// ��������
	int xDir = true;
	if (dy > dx) {
		// �޸Ĳ�������Ϊy �ķ��򲽽�
		xDir = false;
		swap<int>(dx, dy);
	}

	int sumStep = dx;

	int x = pt1._x;
	int y = pt1._y;

	int xStep = pt1._x < pt2._x ? 1: -1;
	int yStep = pt1._y < pt2._y ? 1 : -1;


	// p �ĳ�ʼֵ
	int p = 2 * dy - dx;

	for (int i = 0; i <sumStep; ++i) {

		float scale = 0;
		if (xDir) {
			scale = i / float(sumStep);
		}
		else {
			scale = i / float(sumStep);
		}

		this->drawPoint(x, y, this->getPointColor(pt1._color,pt2._color,scale));

		// ����������
		if (xDir)
			x += xStep;
		else
			y += yStep;
		


		if (p > 0) {
			p = p + 2 * dy - 2 * dx;
			if (xDir)
				y += yStep;
			else
				x += xStep;
			
		}
		else {
			p += 2 * dy;
		}

		
	}
	

}

void GT::Canvas::drawTriangle(Point pt1, Point pt2, Point pt3) {
	// �����εľ��ΰ�Χ��
	int left = MIN(MIN(pt1._x, pt2._x), pt3._x);
	int bottom = MIN(MIN(pt1._y, pt2._y), pt3._y);
	int right = MAX(MAX(pt1._x, pt2._x), pt3._x);
	int top = MAX(MAX(pt1._y, pt2._y), pt3._y);


	// TODO ����������ߵĺ���ʽ
	float k1 = (float(pt2._y) - pt3._y) / (float(pt2._x) - pt3._x);
	float k2 = (float(pt1._y) - pt3._y) / (float(pt1._x) - pt3._x);
	float k3 = (float(pt2._y) - pt1._y) / (float(pt2._x) - pt1._x);

	float b1 = pt2._y - k1 * pt2._x;
	float b2 = pt3._y - k2 * pt3._x;
	float b3 = pt1._y - k3 * pt1._x;


	// ��������
	for (int x = left; x < right; ++x) {
		for (int y = bottom; y < top; y++)
		{
			int judge1 = (y - k1 * x - b1)*(pt1._y - k1 * pt1._x - b1);
			int judge2 = (y - k2 * x - b2)*(pt2._y - k2 * pt2._x - b2);
			int judge3 = (y - k3 * x - b3)*(pt3._y - k3 * pt3._x - b3);

			if (judge1 >= 0 && judge2 >= 0 && judge3 >= 0) {
				this->drawPoint(x, y, RGBA(255, 30, 0));
			}
		}
	}
}

void GT::Canvas::drawTriangleFlat(Point flat1, Point flat2, Point pt) {
	// ����б�ߵ�б��ʽ

	float k1 = 0.0;
	float k2 = 0.0;

	if (flat2._x != pt._x) {
		k1 = (float(pt._y) - flat2._y) / (float(pt._x) - flat2._x);
	}

	if (flat1._x != pt._x) {
		k2 = (float(pt._y) - flat1._y) / (float(pt._x) - flat1._x);
	}

	float b1 = pt._y - k1 * pt._x;
	float b2 = pt._y - k2 * pt._x;

	int minY = MIN(MIN(flat1._y, flat2._y), pt._y);
	int maxY = MAX(MAX(flat1._y, flat2._y), pt._y);

	for (int y = minY; y < maxY; ++y) {
		int x1 = (y - b2) / k2;
		int x2 = (y - b1) / k1;
		this->brensenhamLine(Point(x1, y, RGBA(255, 0, 0)), Point(x2, y, RGBA(255, 0, 0)));
	}
}