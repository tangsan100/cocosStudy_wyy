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
	Shader *skyShader; // ��պ���

	// ������ͼ��������ͼ
	uint textureBox;
	uint textureSpec;
	uint textureWin;
	uint texturePlane;
	uint textureSkybox;

	// �����VAO
	uint VAO_cube = 0;
	uint VAO_plane = 0;
	uint VAO_win = 0;
	uint VAO_skybox = 0;

	// famebuffer
	uint frameBuffer = 0;
	uint textureBuffer = 0;
	uint VAO_texture = 0;

	
public:
	DrawTest(int w, int h);
	~DrawTest();

	void init();
	uint createTexture(const char* fileName);
	uint createSkyTexture();
	uint createModel();
	uint createPlane();
	uint createWindow();
	uint createFramBuffer();
	uint createScreenPlane();
	uint createSkyBox();
	void bindTexture();
	void bindTexture(uint textureID);
	void render();

	//-----test--------
	void testHightLight(); // �߹�
	void testBlending();	// ��͸������
	void testCullFace();	// cullFace ����
	void testFrameBuffer();  // ����֡����
	void testSkybox();		 // ������պ���

	void camMove(CAM_MOVE type) {
		cam->move(type);
	}

	void mouseMove(double x, double y) {
		cam->mouseMove(x, y);
	}
};

