#include "DrawTest.h"
#include "project.h"
#include <math.h>
#include <stdlib.h>
#include "Image.h"

DrawTest::DrawTest(Canvas* ca) :canvas(ca) {
	Image* img = Image::loadImage("res/bg.jpg");

	float zoomX = canvas->getWidth() / float(img->getWidth());
	float zoomY = canvas->getHeight() / float(img->getHeight());
	//this->bg = Image::zoomImageSimple(img, zoomX, zoomY);
	this->bg = Image::zoomImage(img, zoomX, zoomY);

	speed = 0.08;

	delete img;
}


void DrawTest::loagImage(std::string fileName) {
	//this->img = Image::zoomImageSimple(Image::loadImage(fileName),3.0,3.0);
	this->img = Image::zoomImageSimple(Image::loadImage(fileName),1.0,1.0);
	this->img->setAlphaBlend(true);

	/*this->img2 = Image::zoomImage(Image::loadImage(fileName), 2.0, 2.0);
	this->img2->setAlphaBlend(true);*/
	//this->img->setAlpha(0.5);
}

void DrawTest::drawImage(int x, int y) {
	this->canvas->drawImage(0, 0, bg);
	this->canvas->drawImage(x, y, img);
	//this->canvas->drawImage(x-100, y, this->img2);
}


void DrawTest::drawLine() {
	canvas->brensenhamLine(Point(300, 300,RGBA(255,0,0)), Point(150, 20, RGBA(255, 0, 0)));
}

void DrawTest::drawTriangle(Point pt1, Point pt2, Point pt3) {
	canvas->drawTriangle(pt1, pt2, pt3);
}

void DrawTest::drawTriangleFlat(Point flat1, Point flat2, Point pt) {
	canvas->drawTriangleFlat(flat1, flat2, pt);
}

void DrawTest::drawTriangleCommon(Point pt1, Point pt2, Point pt3) {
	canvas->drawTriangleCommon(pt1, pt2, pt3);
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

void DrawTest::testUV() {
	/*
		TODO :
		1. 绑定纹理
		2. 构建三角形的三个顶点坐标和UV
		3. 画图
	*/

	//static Image* img = Image::loadImage("res/ha.jpg");
	this->canvas->bindTexture(this->img);

	Point pts1[3] = {
		Point(0,0,RGBA(255,0,0),floatV2(0,0)),
		Point(650,0,RGBA(255,0,0),floatV2(1,0)),
		Point(650,420,RGBA(255,0,0),floatV2(1,1)),
	};

	Point pts2[3] = {
		Point(0,0,RGBA(255,0,0),floatV2(0,0)),
		Point(0,420,RGBA(255,0,0),floatV2(0,1)),
		Point(650,420,RGBA(255,0,0),floatV2(1,1)),
	};

	for (int i=0;i<3;i++)
	{
		pts1[i].uv.x += speed;
		pts2[i].uv.x += speed;
	}

	speed += 0.08;
	this->canvas->drawTriangleCommon(pts1[0], pts1[1], pts1[2]);
	this->canvas->drawTriangleCommon(pts2[0], pts2[1], pts2[2]);
}

void DrawTest::testGTGL() {
	Point pts1[6] = {
		Point(0,0,RGBA(255,0,0),floatV2(0,0)),
		Point(650,0,RGBA(255,0,0),floatV2(1,0)),
		Point(650,420,RGBA(255,0,0),floatV2(1,1)),
		Point(0,0,RGBA(255,0,0),floatV2(0,0)),
		Point(0,420,RGBA(255,0,0),floatV2(0,1)),
		Point(650,420,RGBA(255,0,0),floatV2(1,1))
	};

	this->canvas->bindTexture(this->img);

	canvas->gtVertexPointer(2, GT_FLOAT, sizeof(Point), (byte*)pts1);
	canvas->gtColorPointer(1, GT_FLOAT, sizeof(Point), (byte*)&pts1[0]._color);
	canvas->gtTexCoorPointer(1, GT_FLOAT, sizeof(Point), (byte*)&pts1[0].uv);
	
	canvas->gtDrawArray(GT_TRIANGLE, 0, 6);
}