#include "Canvas.h"
#include "project.h"
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
#include "Image.h"
using namespace std;
using namespace GT;

GT::Canvas::Canvas(int width, int height, void* buffer){

	wWidth = width;
	wHeight = height;
	this->buffer = (RGBA*)buffer;
	
}


void GT::Canvas::drawPoint( int x, int y, RGBA rgba) {

	if ((y*wWidth+x) >= wWidth*wHeight)
	{
		return;
	}
	buffer[y*wWidth + x] = rgba;
}

void GT::Canvas::clear() {
	memset(buffer, 0, wWidth*wHeight * sizeof(RGBA));
}

RGBA GT::Canvas::getPointColor(int x, int y) {
	if (x >= wWidth || x < 0 || y >= wHeight || y < 0)
	{
		return RGBA(255, 255, 255, 255);
	}

	return buffer[y*wWidth + x];
}

RGBA GT::Canvas::getScalingColor(RGBA color0, RGBA color1, float scale) {
	int r = color0.m_r + (color1.m_r - color0.m_r)*scale;
	int g = color0.m_g + (color1.m_g - color0.m_g)*scale;
	int b = color0.m_b + (color1.m_b - color0.m_b)*scale;
	int a = color0.m_a + (color1.m_a - color0.m_a)*scale;

	return RGBA(r, g, b, a);
}

// 计算uv 的值，按照比例计算
floatV2 GT::Canvas::lerpUV(floatV2 uv1, floatV2 uv2, float scale) {
	floatV2 uv;
	uv.x = uv1.x + (uv2.x - uv1.x)*scale;
	uv.y = uv1.y + (uv2.y - uv1.y)*scale;

	return uv;
}


/*
* brensenham 画线算法
* p1 起始点
* p2 终点
* color 颜色
* 起始 p = 2Δy-Δx
* 当 pi >= 0, 即 d1 > d2 , yi_1 = yi + 1,   pi_1 = pi + 2Δy-2Δx
* 当 pi< 0 即 d1 < d2 , yi_1 = yi, pi_1 = pi+2Δy
*/
void GT::Canvas::brensenhamLine(Point pt1, Point pt2) {

	// 计算偏移量
	int dx = abs(pt1._x - pt2._x);
	int dy = abs(pt1._y - pt2._y);


	// 步进方向
	int xDir = true;
	if (dy > dx) {
		// 修改步进方向为y 的方向步进
		xDir = false;
		//std::swap<int>(dx, dy);
		int  tmp = dx;
		dx = dy;
		dy = tmp;
	}

	int sumStep = dx;

	int x = pt1._x;
	int y = pt1._y;

	int xStep = pt1._x < pt2._x ? 1: -1;
	int yStep = pt1._y < pt2._y ? 1 : -1;


	// p 的初始值
	int p = 2 * dy - dx;
	RGBA color;
	for (int i = 0; i <sumStep; ++i) {

		float scale = 0;
		if (xDir) {
			scale = i / float(sumStep);
		}
		else {
			scale = i / float(sumStep);
		}

		if (statement.isBindTexture)
		{
			if (statement.texture)
			{
				floatV2 uv = this->lerpUV(pt1.uv, pt2.uv, scale);
				color = statement.texture->getColorByUV(uv.x, uv.y);
			}
			else {
				color = this->getScalingColor(pt1._color, pt2._color, scale);
			}
		}
		else {
			color = this->getScalingColor(pt1._color, pt2._color, scale);
		}

		this->drawPoint(x, y, color);

		// 步进主坐标
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
	// 三角形的矩形包围体
	int left = MIN(MIN(pt1._x, pt2._x), pt3._x);
	int bottom = MIN(MIN(pt1._y, pt2._y), pt3._y);
	int right = MAX(MAX(pt1._x, pt2._x), pt3._x);
	int top = MAX(MAX(pt1._y, pt2._y), pt3._y);


	// TODO 计算出三条边的函数式
	float k1 = (float(pt2._y) - pt3._y) / (float(pt2._x) - pt3._x);
	float k2 = (float(pt1._y) - pt3._y) / (float(pt1._x) - pt3._x);
	float k3 = (float(pt2._y) - pt1._y) / (float(pt2._x) - pt1._x);

	float b1 = pt2._y - k1 * pt2._x;
	float b2 = pt3._y - k2 * pt3._x;
	float b3 = pt1._y - k3 * pt1._x;


	// 遍历画点
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
	// 两个斜边的斜截式

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

	int oMaxY = maxY;
	int oMinY = minY;

	if (minY < 0) minY = 0;
	if (maxY < 0) maxY = 0;
	if (maxY > wHeight) maxY = wHeight - 1;
	if (minY > wHeight) minY = wHeight - 1;



	if (maxY == 0 || minY == (wHeight-1)) return;

	RGBA colorStart1 = flat1._y < pt._y ? flat1._color : pt._color;
	RGBA colorEnd1  = flat1._y < pt._y ? pt._color : flat1._color;
	RGBA colorStart2 = flat2._y < pt._y ? flat2._color : pt._color;
	RGBA colorEnd2 = flat2._y < pt._y ? pt._color : flat2._color;

	floatV2 uvStart1 = flat1._y < pt._y ? flat1.uv : pt.uv;
	floatV2 uvEnd1 = flat1._y < pt._y ? pt.uv : flat1.uv;
	floatV2 uvStart2 = flat2._y < pt._y ? flat2.uv : pt.uv;
	floatV2 uvEnd2 = flat2._y < pt._y ? pt.uv : flat2.uv;

	// y 方向步进
	for (int y = minY; y < maxY; ++y) {
		int x1 = (k2 == 0)?k2:((y - b2) / k2);
		int x2 = (k1 == 0)?pt._x:((y - b1) / k1);

		x1 = x1 < 0 ? 0 : x1;
		x1 = x1 > wWidth ? wWidth - 1 : x1;
		x2 = x2 < 0 ? 0 : x2;
		x2 = x2 > wWidth ? wWidth - 1 : x2;

		// 计算颜色值
		float s = abs(float(y-oMinY) / (oMaxY-oMinY));
		RGBA c1 = this->getScalingColor(colorStart1, colorEnd1, s);
		RGBA c2 = this->getScalingColor(colorStart2, colorEnd2, s);

		floatV2 uv1 = this->lerpUV(uvStart1, uvEnd1, s);
		floatV2 uv2 = this->lerpUV(uvStart2, uvEnd2, s);

		this->brensenhamLine(Point(x1, y,c1,uv1), Point(x2, y,c2,uv2));
	}
}

/*
* 任意形状三角形
*/
void GT::Canvas::drawTriangleCommon(Point pt1, Point pt2, Point pt3) {


	// 按照y坐标，从大到小排序
	std::vector<GT::Point> vp;
	vp.push_back(pt1);
	vp.push_back(pt2);
	vp.push_back(pt3);

	// 判断屏幕矩形区和三角形是否有交点
	do {
		Rect rect(0, wWidth, 0, wHeight);
		// 判断三角形是否在矩形区内
		if (this->isInRect(pt1, rect) || this->isInRect(pt2, rect) || this->isInRect(pt3, rect)) break;
		
		// 判断矩形区的4个顶点是否有在三角形的内部
		if (this->isInTriangle(Point(0, 0), vp) || 
			this->isInTriangle(Point(0, wWidth), vp) || 
			this->isInTriangle(Point(0, wHeight), vp) || 
			this->isInTriangle(Point(wWidth, wHeight), vp)) break;

		// 没有相交点，直接返回
		return;

	} while (0);
	

	
	// 三角形的顶点按照 y 值从大到小排序
	sort(vp.begin(), vp.end(), [] ( const Point &p1,const Point &p2){
		return p1._y > p2._y;
	});

	Point pMax = vp[0];
	Point pMid = vp[1];
	Point pMin = vp[2];


	// 如果是平底的，直接调用画平底的三角形
	if (pMax._y == pMid._y) {
		this->drawTriangleFlat(pMid, pMax, pMin);
		return;
	}
		

	if (pMid._y == pMin._y){
		this->drawTriangleFlat(pMid, pMin, pMax);
		return;
	}
		

	
	// pMid 为顶点对边  算出直线的斜截式参数
	float k = 0.0;
	int x = 0;
	if (pMax._x != pMin._x) {
		k = (float(pMax._y) - pMin._y) / (float(pMax._x) - pMin._x);
		float b = pMin._y - pMin._x*k;

		// 算出平底边， 的另一个点
		x = (pMid._y - b) / k;
	}
	else {
		// 相等的话
		x = pMax._x;
	}

	

	// 计算颜色值
	float scale = (float(pMid._y) - pMin._y) / (float(pMax._y) - pMin._y);
	RGBA color = this->getScalingColor(pMin._color, pMax._color, scale);
	
	// 计算UV 坐标
	floatV2 uv = this->lerpUV(pMin.uv, pMax.uv, scale);

	this->drawTriangleFlat(pMid, Point(x, pMid._y,color, uv), pMax);
	this->drawTriangleFlat(pMid, Point(x, pMid._y,color, uv), pMin);
	
}

/*
* 判断点是否在矩形区域内
* pt: 需要判断的点
* rect: 矩形区
*/
bool GT::Canvas::isInRect(Point pt, Rect rect) {
	if (pt._x < rect.left || pt._x > rect.right || pt._y < rect.bottom || pt._y > rect.top) {
		return false;
	}

	return true;
}


/*
* 是否在三角形内部
* @param pt : 要判断的点
* pts :三角形的三个顶点
*/
bool GT::Canvas::isInTriangle(Point pt, std::vector<Point> pts) {

	Point pt1 = pts[0];
	Point pt2 = pts[1];
	Point pt3 = pts[2];

	// TODO 计算出三条边的函数式
	float k1 = (float(pt2._y) - pt3._y) / (float(pt2._x) - pt3._x);
	float k2 = (float(pt1._y) - pt3._y) / (float(pt1._x) - pt3._x);
	float k3 = (float(pt2._y) - pt1._y) / (float(pt2._x) - pt1._x);

	float b1 = pt2._y - k1 * pt2._x;
	float b2 = pt3._y - k2 * pt3._x;
	float b3 = pt1._y - k3 * pt1._x;


	// 遍历画点

	int judge1 = (pt._y - k1 * pt._x - b1)*(pt1._y - k1 * pt1._x - b1);
	int judge2 = (pt._y - k2 * pt._x - b2)*(pt2._y - k2 * pt2._x - b2);
	int judge3 = (pt._y - k3 * pt._x - b3)*(pt3._y - k3 * pt3._x - b3);

	if (judge1 >= 0 && judge2 >= 0 && judge3 >= 0) {
		return true;
	}

	return false;
}

void GT::Canvas::drawImage(int x, int y, Image* img) {
	for (int u =0; u < img->getWidth(); u++)
	{
		for (int v=0; v < img->getHeight(); v++)
		{
			RGBA color = img->getColor(u,v);

			if (img->getAlphaBlend())
			{
				// alpha 混合
				RGBA bgColor = this->getPointColor(x+u, y+v);
				color = getScalingColor(bgColor, color, float(color.m_a) / bgColor.m_a*img->getAlpha());
				drawPoint(x + u, y + v, color);
			}
			else {
				drawPoint(x + u, y + v, color);
			}
			
		}
	}
}

void GT::Canvas::bindTexture(const Image* img) {
	if (statement.texture == img)
	{
		return;
	}
	statement.texture = img;
	statement.isBindTexture = true;
}

void GT::Canvas::gtVertexPointer(int size, Data_Type type, int stride, byte* data) {
	statement.vertexData.size = size;
	statement.vertexData.type = type;
	statement.vertexData.stride = stride;
	statement.vertexData.data = data;
}

void  GT::Canvas::gtColorPointer(int size, Data_Type type, int stride, byte* data) {
	statement.colorData.size = size;
	statement.colorData.type = type;
	statement.colorData.stride = stride;
	statement.colorData.data = data;
}


void  GT::Canvas::gtTexCoorPointer(int size, Data_Type type, int stride, byte* data) {
	statement.texCoorData.size = size;
	statement.texCoorData.type = type;
	statement.texCoorData.stride = stride;
	statement.texCoorData.data = data;
}

void GT::Canvas::gtDrawArray(DRAW_MODE mode, int first, int count) {

	byte* vertexData = statement.vertexData.data + first * sizeof(statement.vertexData.stride);
	byte* colorData = statement.colorData.data + first * sizeof(statement.colorData.stride);
	byte* texCoorData = statement.texCoorData.data + first * sizeof(statement.texCoorData.stride);

	count -= first;
	switch (mode)
	{
	case GT::GT_LINE:
	{
		Point pt0, pt1;
		count /= 2;
		
		for (int i= 0; i < count; ++i)
		{
			
			// 取pt0坐标点
			int* verTexPoint = (int*)vertexData;
			pt0._x = verTexPoint[0];
			pt0._y = verTexPoint[1];
			vertexData += statement.vertexData.stride;

			// 取pt1坐标点
			verTexPoint = (int*)vertexData;
			pt1._x = verTexPoint[0];
			pt1._y = verTexPoint[1];
			vertexData += statement.vertexData.stride;

			// 取pt0 的颜色值
			
			RGBA* rgbData = (RGBA*)colorData;
			pt0._color = rgbData[0];
			colorData += statement.colorData.stride;

			// 取pt1 的颜色值
			rgbData = (RGBA*)colorData;
			pt1._color = rgbData[0];
			colorData += statement.colorData.stride;

			

			this->brensenhamLine(pt0, pt1);
		}
		

	}
		break;
	case GT::GT_TRIANGLE:
	{
		Point pt0, pt1, pt2;
		count /= 3;
		for (int i = 0; i < count; ++i)
		{
			// 取pt0坐标点
			int* verTexPoint = (int*)vertexData;
			pt0._x = verTexPoint[0];
			pt0._y = verTexPoint[1];
			vertexData += statement.vertexData.stride;

			// 取pt1坐标点
			verTexPoint = (int*)vertexData;
			pt1._x = verTexPoint[0];
			pt1._y = verTexPoint[1];
			vertexData += statement.vertexData.stride;


			// 取pt2坐标点
			verTexPoint = (int*)vertexData;
			pt2._x = verTexPoint[0];
			pt2._y = verTexPoint[1];
			vertexData += statement.vertexData.stride;

			// 取pt0 的颜色值
			RGBA* rgbData = (RGBA*)colorData;
			pt0._color = rgbData[0];
			colorData += statement.colorData.stride;

			// 取pt1 的颜色值
			rgbData = (RGBA*)colorData;
			pt1._color = rgbData[0];
			colorData += statement.colorData.stride;

			// 取pt2 的颜色值
			rgbData = (RGBA*)colorData;
			pt2._color = rgbData[0];
			colorData += statement.colorData.stride;

			// 取pt0 的uv 坐标
			floatV2* uvData = (floatV2*)texCoorData;
			pt0.uv = uvData[0];
			texCoorData += statement.texCoorData.stride;

			// 取pt1 的uv 坐标
			uvData = (floatV2*)texCoorData;
			pt1.uv = uvData[0];
			texCoorData += statement.texCoorData.stride;

			// 取pt2 的uv 坐标
			uvData = (floatV2*)texCoorData;
			pt2.uv = uvData[0];
			texCoorData += statement.texCoorData.stride;

			this->drawTriangleCommon(pt0, pt1, pt2);
		}
	}
		break;
	default:
		break;
	}
}