#pragma once

#include "Base.h"
#include "Camara.h"

class Shader;

class DrawTest
{
private:

	int width;  //�������
	int height; //�����߶�

	Camara* cam; // ���
	Shader *shaderColor; // �߹�
	Shader *shaderCube;

	// ������ͼ��������ͼ
	uint textureBox;
	uint textureSpec;

	// �����VAO
	uint VAO_cube = 0;
	
public:
	DrawTest(int w, int h);
	~DrawTest();

	void init();
	uint createTexture(const char* fileName);
	uint createModel();
	void bindTexture();
	void render();

	//-----test--------
	void testHightLight(); // �߹�

	void camMove(CAM_MOVE type) {
		cam->move(type);
	}

	void mouseMove(double x, double y) {
		cam->mouseMove(x, y);
	}
};

