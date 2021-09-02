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
	zBuffer = new float[wWidth*wHeight];
	
	
}

GT::Canvas::~Canvas() {
	delete[] zBuffer;
}


void GT::Canvas::drawPoint(Point pt) {

	if ((pt._y*wWidth+pt._x) >= wWidth*wHeight)
	{
		return;
	}
	int idx = pt._y*wWidth + pt._x;

	if (pt._z > zBuffer[idx] )
	{
		return;
	}

	zBuffer[idx] = pt._z;

	buffer[idx] = pt._color;
}

void GT::Canvas::clear() {
	memset(buffer, 0, wWidth*wHeight * sizeof(RGBA));
	memset(zBuffer, 255, sizeof(float)*wWidth*wHeight);
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

// ����uv ��ֵ�����ձ�������
floatV2 GT::Canvas::lerpUV(floatV2 uv1, floatV2 uv2, float scale) {
	floatV2 uv;
	uv.x = uv1.x + (uv2.x - uv1.x)*scale;
	uv.y = uv1.y + (uv2.y - uv1.y)*scale;

	return uv;
}

float GT::Canvas::zLerp(float z1, float z2, float scale) {
	return z1 + (z2 - z1)*scale;
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


	// p �ĳ�ʼֵ
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

		float z = this->zLerp(pt1._z, pt2._z, scale);
		this->drawPoint(Point(x, y,z, color));

		
		


		if (p > 0) {
			p = p + 2 * dy - 2 * dx;
			if (xDir)
				y += yStep;
			else
				x += xStep;
			
		}
		/*else if (p == 0) {

		}*/
		else {
			p += 2 * dy;
		}

		// ����������
		if (xDir)
			x += xStep;
		else
			y += yStep;

		
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
				this->drawPoint(Point(x, y,0, RGBA(255, 30, 0)));
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

	float zStart1 = flat1._y < pt._y ? flat1._z : pt._z;
	float zEnd1 = flat1._y < pt._y ? pt._z : flat1._z;
	float zStart2 = flat2._y < pt._y ? flat2._z : pt._z;
	float zEnd2 = flat2._y < pt._y ? pt._z : flat2._z;

	floatV2 uvStart1 = flat1._y < pt._y ? flat1.uv : pt.uv;
	floatV2 uvEnd1 = flat1._y < pt._y ? pt.uv : flat1.uv;
	floatV2 uvStart2 = flat2._y < pt._y ? flat2.uv : pt.uv;
	floatV2 uvEnd2 = flat2._y < pt._y ? pt.uv : flat2.uv;

	// y ���򲽽�
	for (int y = minY; y < maxY; ++y) {
		float x1 = (k2 == 0)? pt._x :((y - b2) / k2);
		float x2 = (k1 == 0)?pt._x:((y - b1) / k1);

		x1 = x1 < 0 ? 0 : x1;
		x1 = x1 > wWidth ? wWidth - 1 : x1;
		x2 = x2 < 0 ? 0 : x2;
		x2 = x2 > wWidth ? wWidth - 1 : x2;

		// ������ɫֵ
		float s = abs(float(y-oMinY) / (oMaxY-oMinY));
		RGBA c1 = this->getScalingColor(colorStart1, colorEnd1, s);
		RGBA c2 = this->getScalingColor(colorStart2, colorEnd2, s);

		floatV2 uv1 = this->lerpUV(uvStart1, uvEnd1, s);
		floatV2 uv2 = this->lerpUV(uvStart2, uvEnd2, s);

		float z1 = this->zLerp(zStart1, zEnd1, s);
		float z2 = this->zLerp(zStart2, zEnd2, s);

		this->brensenhamLine(Point(x1, y,z1,c1,uv1), Point(x2, y,z2,c2,uv2));
	}
}

/*
* ������״������
*/
void GT::Canvas::drawTriangleCommon(Point pt1, Point pt2, Point pt3) {


	// ����y���꣬�Ӵ�С����
	std::vector<GT::Point> vp;
	vp.push_back(pt1);
	vp.push_back(pt2);
	vp.push_back(pt3);

	// �ж���Ļ���������������Ƿ��н���
	do {
		Rect rect(0, wWidth, 0, wHeight);
		// �ж��������Ƿ��ھ�������
		if (this->isInRect(pt1, rect) || this->isInRect(pt2, rect) || this->isInRect(pt3, rect)) break;
		
		// �жϾ�������4�������Ƿ����������ε��ڲ�
		if (this->isInTriangle(Point(0, 0), vp) || 
			this->isInTriangle(Point(0, wWidth), vp) || 
			this->isInTriangle(Point(0, wHeight), vp) || 
			this->isInTriangle(Point(wWidth, wHeight), vp)) break;

		// û���ཻ�㣬ֱ�ӷ���
		return;

	} while (0);
	

	
	// �����εĶ��㰴�� y ֵ�Ӵ�С����
	sort(vp.begin(), vp.end(), [] ( const Point &p1,const Point &p2){
		return p1._y > p2._y;
	});

	Point pMax = vp[0];
	Point pMid = vp[1];
	Point pMin = vp[2];


	// �����ƽ�׵ģ�ֱ�ӵ��û�ƽ�׵�������
	if (pMax._y == pMid._y) {
		this->drawTriangleFlat(pMid, pMax, pMin);
		return;
	}
		

	if (pMid._y == pMin._y){
		this->drawTriangleFlat(pMid, pMin, pMax);
		return;
	}
		

	
	// pMid Ϊ����Ա�  ���ֱ�ߵ�б��ʽ����
	float k = 0.0;
	int x = 0;
	if (pMax._x != pMin._x) {
		k = (float(pMax._y) - pMin._y) / (float(pMax._x) - pMin._x);
		float b = pMin._y - pMin._x*k;

		// ���ƽ�ױߣ� ����һ����
		x = (pMid._y - b) / k;
	}
	else {
		// ��ȵĻ�
		x = pMax._x;
	}

	

	// ������ɫֵ
	float scale = (float(pMid._y) - pMin._y) / (float(pMax._y) - pMin._y);
	RGBA color = this->getScalingColor(pMin._color, pMax._color, scale);
	
	// ����UV ����
	floatV2 uv = this->lerpUV(pMin.uv, pMax.uv, scale);

	float z = this->zLerp(pMin._z, pMax._z, scale);

	this->drawTriangleFlat(pMid, Point(x, pMid._y,z,color, uv), pMax);
	this->drawTriangleFlat(pMid, Point(x, pMid._y,z,color, uv), pMin);
	
}

/*
* �жϵ��Ƿ��ھ���������
* pt: ��Ҫ�жϵĵ�
* rect: ������
*/
bool GT::Canvas::isInRect(Point pt, Rect rect) {
	if (pt._x < rect.left || pt._x > rect.right || pt._y < rect.bottom || pt._y > rect.top) {
		return false;
	}

	return true;
}


/*
* �Ƿ����������ڲ�
* @param pt : Ҫ�жϵĵ�
* pts :�����ε���������
*/
bool GT::Canvas::isInTriangle(Point pt, std::vector<Point> pts) {

	Point pt1 = pts[0];
	Point pt2 = pts[1];
	Point pt3 = pts[2];

	// TODO ����������ߵĺ���ʽ
	float k1 = (float(pt2._y) - pt3._y) / (float(pt2._x) - pt3._x);
	float k2 = (float(pt1._y) - pt3._y) / (float(pt1._x) - pt3._x);
	float k3 = (float(pt2._y) - pt1._y) / (float(pt2._x) - pt1._x);

	float b1 = pt2._y - k1 * pt2._x;
	float b2 = pt3._y - k2 * pt3._x;
	float b3 = pt1._y - k3 * pt1._x;


	// ��������

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
				// alpha ���
				RGBA bgColor = this->getPointColor(x+u, y+v);
				color = getScalingColor(bgColor, color, float(color.m_a) / bgColor.m_a*img->getAlpha());
				//drawPoint(x + u, y + v, color);
			}
			else {
				//drawPoint(x + u, y + v, color);
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
			
			// ȡpt0�����
			float* verTexPoint = (float*)vertexData;
			pt0._x = verTexPoint[0];
			pt0._y = verTexPoint[1];
			pt0._z = verTexPoint[2];
			vertexData += statement.vertexData.stride;

			// ȡpt1�����
			verTexPoint = (float*)vertexData;
			pt1._x = verTexPoint[0];
			pt1._y = verTexPoint[1];
			pt1._z = verTexPoint[2];
			vertexData += statement.vertexData.stride;

			// ȡpt0 ����ɫֵ
			
			RGBA* rgbData = (RGBA*)colorData;
			pt0._color = rgbData[0];
			colorData += statement.colorData.stride;

			// ȡpt1 ����ɫֵ
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
			// ȡpt0�����
			float* verTexPoint = (float*)vertexData;
			pt0._x = verTexPoint[0];
			pt0._y = verTexPoint[1];
			pt0._z = verTexPoint[2];
			vertexData += statement.vertexData.stride;

			// ȡpt1�����
			verTexPoint = (float*)vertexData;
			pt1._x = verTexPoint[0];
			pt1._y = verTexPoint[1];
			pt1._z = verTexPoint[2];
			vertexData += statement.vertexData.stride;


			// ȡpt2�����
			verTexPoint = (float*)vertexData;
			pt2._x = verTexPoint[0];
			pt2._y = verTexPoint[1];
			pt2._z = verTexPoint[2];
			vertexData += statement.vertexData.stride;

			// ȡpt0 ����ɫֵ
			RGBA* rgbData = (RGBA*)colorData;
			pt0._color = rgbData[0];
			colorData += statement.colorData.stride;

			// ȡpt1 ����ɫֵ
			rgbData = (RGBA*)colorData;
			pt1._color = rgbData[0];
			colorData += statement.colorData.stride;

			// ȡpt2 ����ɫֵ
			rgbData = (RGBA*)colorData;
			pt2._color = rgbData[0];
			colorData += statement.colorData.stride;

			// ȡpt0 ��uv ����
			floatV2* uvData = (floatV2*)texCoorData;
			pt0.uv = uvData[0];
			texCoorData += statement.texCoorData.stride;

			// ȡpt1 ��uv ����
			uvData = (floatV2*)texCoorData;
			pt1.uv = uvData[0];
			texCoorData += statement.texCoorData.stride;

			// ȡpt2 ��uv ����
			uvData = (floatV2*)texCoorData;
			pt2.uv = uvData[0];
			texCoorData += statement.texCoorData.stride;

			gtVertexTransform(pt0);
			gtVertexTransform(pt1);
			gtVertexTransform(pt2);

			this->drawTriangleCommon(pt0, pt1, pt2);
		}
	}
		break;
	default:
		break;
	}
}

void GT::Canvas::gtMatrixMode(GT_MATRIX_MODE mode) {


	if (mode != GT_MATRIX_VIEW && mode != GT_MATRIX_PROJECTION && mode != GT_MATRIX_MODEL) {
		return;
	}

	statement.matrixMode = mode;

}

void GT::Canvas::gtLoadMatrix(glm::mat4 matrix) {
	switch (statement.matrixMode)
	{
	case GT_MATRIX_VIEW:
		statement.viewMatrix = matrix;
		break;
	case GT_MATRIX_PROJECTION:
		statement.projectionMatrix = matrix;
		break;
	case GT_MATRIX_MODEL:
		statement.modelMatrix = matrix;
	default:
		break;
	}
}

//	��ɵ�λ��
void GT::Canvas::gtLoadIdentity() {
	switch (statement.matrixMode)
	{
	case GT_MATRIX_VIEW:
		statement.viewMatrix = glm::mat4(1.0f);
		break;
	case GT_MATRIX_PROJECTION:
		statement.projectionMatrix = glm::mat4(1.0f);
		break;
	case GT_MATRIX_MODEL:
		statement.modelMatrix = glm::mat4(1.0f);
		break;
	default:
		break;
	}
}


// ���һ������
void GT::Canvas::gtMultiMatrix(glm::mat4 matrix) {
	switch (statement.matrixMode)
	{
	case GT_MATRIX_VIEW:
		statement.viewMatrix = matrix * statement.viewMatrix;
		break;
	case GT_MATRIX_PROJECTION:
		statement.projectionMatrix = matrix * statement.projectionMatrix;
		break;
	case GT_MATRIX_MODEL:
		statement.modelMatrix = matrix * statement.modelMatrix;
		break;
	default:
		break;
	}
}

/*
	�������� mvp �任
*/
void GT::Canvas::gtVertexTransform(Point &pt) {
	glm::vec4 ptv4(pt._x, pt._y, pt._z, 1);

	ptv4 = statement.projectionMatrix * statement.viewMatrix * statement.modelMatrix * ptv4;

	pt._x = (ptv4.x / ptv4.w + 1)*float(wWidth) / 2;
	pt._y = (ptv4.y / ptv4.w + 1)*float(wHeight) / 2;
	pt._z = ptv4.z/ptv4.w;
}


// ����ѹջ
void GT::Canvas::pushMatrix() {
	statement.matrixVec.push_back(statement.modelMatrix);
}

// ����ջ
void GT::Canvas::popMatrix() {
	if (statement.matrixVec.empty())
	{
		return;
	}

	statement.modelMatrix = statement.matrixVec.back();
	statement.matrixVec.pop_back();
}