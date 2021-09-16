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
	Shader *shaderBlending; // ��͸��
	Shader *screenShader; // frameBuffer

	// ������ͼ��������ͼ
	uint textureBox;
	uint textureSpec;
	uint textureWin;
	uint texturePlane;

	// �����VAO
	uint VAO_cube = 0;
	uint VAO_plane = 0;
	uint VAO_win = 0;

	// famebuffer
	uint frameBuffer = 0;
	uint textureBuffer = 0;
	uint VAO_texture = 0;
	
public:
	DrawTest(int w, int h);
	~DrawTest();

	void init();
	uint createTexture(const char* fileName);
	uint createModel();
	uint createPlane();
	uint createWindow();
	uint createFramBuffer();
	uint createScreenPlane();
	void bindTexture();
	void bindTexture(uint textureID);
	void render();

	//-----test--------
	void testHightLight(); // �߹�
	void testBlending();	// ��͸������
	void testCullFace();	// cullFace ����
	void testFrameBuffer();  // ����֡����

	void camMove(CAM_MOVE type) {
		cam->move(type);
	}

	void mouseMove(double x, double y) {
		cam->mouseMove(x, y);
	}
};

