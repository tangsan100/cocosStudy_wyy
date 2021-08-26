#pragma once
#include "Canvas.h"
#include <string>
using namespace GT;
class Image;
class DrawTest
{
private:
	Canvas* canvas;

	Image*	bg;
	Image*	img;
	Image*	img2;
	float   speed; // ������ٶ�
public:
	DrawTest(Canvas* ca);

	// ����һ��ͼƬ
	void loagImage(std::string fileName);

	// ����Ļ�ϻ���ͼƬ
	void drawImage(int x, int y);

	// ��ֱ��
	void drawLine();

	// ��������
	void drawTriangle(Point pt1, Point pt2, Point pt3);

	// ��ƽ��������
	void drawTriangleFlat(Point flat1, Point flat2, Point pt);

	// ��һ����������
	void drawTriangleCommon(Point pt1, Point pt2, Point pt3);

	// ������
	void drawRay();

	// ��ѩ����Ļ
	void drawSnowScreen(int wWidth, int wHeight);

	// ����UV �����UV ������ͼ
	void testUV();

	// ����gtGL �ӿڣ� ģ���װopenGL �ӿڲ���
	void testGTGL();

	
};

