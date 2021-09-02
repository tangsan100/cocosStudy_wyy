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
	float   speed; // 跑马灯速度
public:
	DrawTest(Canvas* ca);

	// 加载一张图片
	void loagImage(std::string fileName);

	// 在屏幕上画出图片
	void drawImage(int x, int y);

	// 画直线
	void drawLine();

	// 画三角形
	void drawTriangle(Point pt1, Point pt2, Point pt3);

	// 画平底三角形
	void drawTriangleFlat(Point flat1, Point flat2, Point pt);

	// 画一个任意三角
	void drawTriangleCommon(Point pt1, Point pt2, Point pt3);

	// 画射线
	void drawRay();

	// 画雪花屏幕
	void drawSnowScreen(int wWidth, int wHeight);

	// 测试UV 坐标和UV 纹理贴图
	void testUV();

	// 测试gtGL 接口， 模拟封装openGL 接口测试
	void testGTGL();


	// test GL Rotate
	void testRotate();

	// 3D变换
	void test3D();

	// 3d 变换优化版
	void test3D1();

	void draw(DRAW_MODE drawMode, Point arr[], int count);
	
};

