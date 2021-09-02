#include "DrawTest.h"
#include "project.h"
#include <math.h>
#include <stdlib.h>
#include "Image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
	canvas->brensenhamLine(Point(300, 300,0,RGBA(255,0,0)), Point(150, 20,0, RGBA(255, 0, 0)));
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
	// ����һ���̻���
	GT::Point pt1(150, 150,0,GT::RGBA(255,0,0));
	GT::RGBA color(rand() % 255, rand() % 255, rand() % 255);

	// Բ���뾶100
	float dis = 100;

	for (float i = 0; i <= 360; i += 15) {
		// ����һ�����ȣ�  ����= ��*��/180��
		double d = DEG2RAD(i);

		// ͨ�������Һ��������x,y ��ƫ����
		int dx = cos(d)*dis;
		int dy = sin(d)*dis;

		int x = dx + pt1._x;
		int y = dy + pt1._y;


		canvas->brensenhamLine(pt1, GT::Point(x, y,0,GT::RGBA(0,255,0,255)));
	}
}


void DrawTest::drawSnowScreen(int wWidth, int wHeight) {
	
	for (int x = 0; x < wWidth; ++x) {
		for (int y = 0; y < wHeight; ++y) {
			GT::RGBA color(rand() % 255, rand() % 255, rand() % 255);
			canvas->drawPoint(Point(x, y, 0, color));
		}
	}
}

void DrawTest::testUV() {
	/*
		TODO :
		1. ������
		2. ���������ε��������������UV
		3. ��ͼ
	*/

	//static Image* img = Image::loadImage("res/ha.jpg");
	this->canvas->bindTexture(this->img);

	Point pts1[3] = {
		Point(0,0,0,RGBA(255,0,0),floatV2(0,0)),
		Point(650,0,0,RGBA(255,0,0),floatV2(1,0)),
		Point(650,420,0,RGBA(255,0,0),floatV2(1,1)),
	};

	Point pts2[3] = {
		Point(0,0,0,RGBA(255,0,0),floatV2(0,0)),
		Point(0,420,0,RGBA(255,0,0),floatV2(0,1)),
		Point(650,420,0,RGBA(255,0,0),floatV2(1,1)),
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

// �����Զ���GL �Ľӿ�
void DrawTest::testGTGL() {
	Point pts1[6] = {
		Point(0,0,0,RGBA(255,0,0),floatV2(0,0)),
		Point(650,0,0,RGBA(255,0,0),floatV2(1,0)),
		Point(650,420,0,RGBA(255,0,0),floatV2(1,1)),
		Point(0,0,0,RGBA(255,0,0),floatV2(0,0)),
		Point(0,420,0,RGBA(255,0,0),floatV2(0,1)),
		Point(650,420,0,RGBA(255,0,0),floatV2(1,1))
	};

	this->canvas->bindTexture(this->img);

	canvas->gtVertexPointer(2, GT_FLOAT, sizeof(Point), (byte*)pts1);
	canvas->gtColorPointer(1, GT_FLOAT, sizeof(Point), (byte*)&pts1[0]._color);
	canvas->gtTexCoorPointer(1, GT_FLOAT, sizeof(Point), (byte*)&pts1[0].uv);
	
	canvas->gtDrawArray(GT_TRIANGLE, 0, 6);
}

void DrawTest::testRotate() {
	Point pts1[6] = {
		Point(0.0f,0.0f,0.0f,RGBA(255,0,0),floatV2(0,0)),
		Point(400.0f,0.0f,0.0f,RGBA(255,0,0),floatV2(1,0)),
		Point(400.0f,250.0f,0.0f,RGBA(255,0,0),floatV2(1,1)),
		Point(0.0f,0.0f,0.0f,RGBA(255,0,0),floatV2(0,0)),
		Point(0.0f,420.0f,0.0f,RGBA(255,0,0),floatV2(0,1)),
		Point(650.0f,420.0f,0.0f,RGBA(255,0,0),floatV2(1,1))
	};

	this->canvas->bindTexture(this->img);
	static float angle = 0.0f;

	for (int i = 0; i < 6; i++)
	{
		// ��ά���꣬��z=0�� w=1 
		glm::vec4 ptv4(pts1[i]._x, pts1[i]._y, pts1[i]._z, 1);
		// ����һ����λ����
		glm::mat4 rMat(1.0f);
		// ��ת�Ƕ�
		rMat = glm::rotate(rMat, glm::radians(angle), glm::vec3(1, 0, 0));

		// ƽ��
		glm::mat4 tMat(1.0f);
		tMat = glm::translate(tMat, glm::vec3(100, 0, 0));

		glm::mat4 vMat(1.0f);
		vMat = glm::lookAt(glm::vec3(200, 0, 200), glm::vec3(200, 0, 0), glm::vec3(0, 1, 0));

		// ���������ת��ľ���
		ptv4 = tMat*rMat * ptv4;
		ptv4 = vMat * ptv4;
		pts1[i]._x = ptv4.x;
		pts1[i]._y = ptv4.y;
		pts1[i]._z = ptv4.z;

	}

	//angle += 2;

	draw(GT_TRIANGLE, pts1, 3);
}

void DrawTest::test3D() {
	Point pts1[6] = {
		Point(0.0f,0.0f,0.0f,RGBA(255,0,0),floatV2(0,0)),
		Point(300.0f,0.0f,0.0f,RGBA(255,0,0),floatV2(1,0)),
		Point(300.0f,300.0f,0.0f,RGBA(255,0,0),floatV2(1,1)),

		Point(300.0f,0.0f,0.0f,RGBA(255,0,0),floatV2(0,0)),
		Point(300.0f,300.0f,0.0f,RGBA(0,255,0),floatV2(0,0)),
		Point(300.0f,0.0f,-500.0f,RGBA(0,0,255),floatV2(0,0)),

	};

	this->canvas->bindTexture(this->img);
	static float angle = 0.0f;

	float width = canvas->getWidth();
	float height = canvas->getHeight();


		//��ת
		glm::mat4 rMat(1.0f);
		rMat = glm::rotate(rMat, glm::radians(angle), glm::vec3(0, 1, 0));
		rMat = glm::rotate(rMat, glm::radians(50.0f), glm::vec3(1, 0, 0));

		// ƽ��
		glm::mat4 tMat(1.0f);
		tMat = glm::translate(tMat, glm::vec3(-300, 0, 0));


		canvas->gtMatrixMode(GT_MATRIX_MODEL);
		canvas->gtLoadIdentity();
		canvas->gtMultiMatrix(tMat);
		canvas->gtMultiMatrix(rMat);

		// v�任
		glm::mat4 vMat(1.0f);
		vMat = glm::lookAt(glm::vec3(0,0,1000), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		canvas->gtMatrixMode(GT_MATRIX_VIEW);
		canvas->gtLoadIdentity();
		canvas->gtLoadMatrix(vMat);

		// p ͸��ͶӰ�任
		glm::mat4 pMat(1.0f);
		// ����1�� ���Ž�
		// ����2����߱�
		// ����3����ƽ�����
		// ����4��Զƽ�����
		pMat = glm::perspective(glm::radians(60.0f), width / height, 1.0f, 1000.0f);
		

		canvas->gtMatrixMode(GT_MATRIX_PROJECTION);
		canvas->gtLoadIdentity();
		canvas->gtLoadMatrix(pMat);


	angle += 2;


	draw(GT_TRIANGLE, pts1, 6);

}

void DrawTest::test3D1() {
	Point pts0[3] = {
		Point(0.0f,0.0f,0.0f,RGBA(255,0,0),floatV2(0,0)),
		Point(300.0f,0.0f,0.0f,RGBA(255,0,0),floatV2(1,0)),
		Point(300.0f,300.0f,0.0f,RGBA(255,0,0),floatV2(1,1)),

	};

	Point pts1[3] = {
		Point(300.0f,0.0f,0.0f,RGBA(255,0,0),floatV2(0,0)),
		Point(300.0f,300.0f,0.0f,RGBA(0,255,0),floatV2(0,0)),
		Point(600.0f,0.0f,0.0f,RGBA(0,0,255),floatV2(0,0)),
	};

	this->canvas->bindTexture(this->img);
	static float angle = 0.0f;
	static float xStep = 0.0f;

	float width = canvas->getWidth();
	float height = canvas->getHeight();

	// ģ�ͱ任

	canvas->gtMatrixMode(GT_MATRIX_MODEL);
	canvas->gtLoadIdentity();

	glm::mat4 sumMat(1.0f);
	sumMat = glm::translate(sumMat, glm::vec3(xStep,0,0));
	canvas->gtMultiMatrix(sumMat);
	xStep += 2;


	// ��������
	canvas->pushMatrix();
	canvas->gtLoadIdentity();

	// ������2��ƽ��
	glm::mat4 tMat(1.0f);
	tMat = glm::translate(tMat, glm::vec3(-300, 0, 0));
	canvas->gtMultiMatrix(tMat);

	//  ������2����ת
	glm::mat4 rMat(1.0f);
	rMat = glm::rotate(rMat, glm::radians(angle), glm::vec3(0, 1, 0));
	canvas->gtMultiMatrix(rMat);
	angle += 2;

	// v �任
	canvas->gtMatrixMode(GT_MATRIX_VIEW);
	canvas->gtLoadIdentity();
	glm::mat4 vMat(1.0f);
	vMat = glm::lookAt(glm::vec3(0, 0, 1000), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	canvas->gtLoadMatrix(vMat);

	// p �任
	canvas->gtMatrixMode(GT_MATRIX_PROJECTION);
	canvas->gtLoadIdentity();
	glm::mat4 pMat(1.0f);
	pMat = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 1000.0f);
	canvas->gtLoadMatrix(pMat);

	// ����������2
	draw(GT_TRIANGLE, pts0, 3);

	// ����������1
	canvas->popMatrix();
	draw(GT_TRIANGLE, pts1, 3);

}

void DrawTest::draw(DRAW_MODE drawMode, Point arr[], int count) {


	canvas->gtVertexPointer(2, GT_FLOAT, sizeof(Point), (byte*)arr);
	canvas->gtColorPointer(1, GT_FLOAT, sizeof(Point), (byte*)&arr[0]._color);
	canvas->gtTexCoorPointer(1, GT_FLOAT, sizeof(Point), (byte*)&arr[0].uv);

	canvas->gtDrawArray(drawMode, 0, count);
}