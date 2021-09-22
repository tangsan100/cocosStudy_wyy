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
	Shader *shaderRed; // ����ubo
	Shader *shaderGreen; //����ubo
	Shader *shaderGeo; // ����geo
	Shader *shaderBox;

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
	uint VAO_house = 0;
	uint VAO_box = 0;

	// famebuffer
	uint frameBuffer = 0;
	uint textureBuffer = 0;
	uint VAO_texture = 0;

	// UBO 
	uint UBO_red;

	
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
	uint createUBO();  // unifrom block
	uint createHouse();
	void bindTexture();
	void bindTexture(uint textureID);
	void bindShaderData();  // ��ubo 
	void render();

	//-----test--------
	void testHightLight(); // �߹�
	void testBlending();	// ��͸������
	void testCullFace();	// cullFace ����
	void testFrameBuffer();  // ����֡����
	void testSkybox();		 // ������պ���
	void testUboData();		// ����UBO
	void testGeo();

	void camMove(CAM_MOVE type) {
		cam->move(type);
	}

	void mouseMove(double x, double y) {
		cam->mouseMove(x, y);
	}
};

