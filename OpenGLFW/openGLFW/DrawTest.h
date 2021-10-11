#pragma once

#include "Base.h"
#include "Camara.h"
#include "IO.h"

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
	Shader *planetShader; // ����
	Shader *rockShader;
	Shader *blinnPhone;
	Shader *shaderShadow;
	Shader *dirShader;//
	Shader *shaderPsDepth; // Ps :pointShadow
	Shader *shaderPointShadow; // ���Դ��shadow
	Shader *pointShader; // ���Դ

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
	uint depthFbo;
	uint depthCubeMapFbo;

	// UBO 
	uint UBO_red;
	uint depthMap; // �����ͼ
	uint depthCubemap; // cubeMap ����Ȼ���

	// ģ��
	FF::ffModel* planet; //����
	FF::ffModel* rock;	// ��ʯ
	FF::ffModel* bag; // ����ģ��

	glm::mat4* matrixArr;
public:
	DrawTest(int w, int h);
	~DrawTest();

	void init();

	// instanceID ʵ����������
	void initInstanceArray();

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
	uint createShadowFbo(); // shadowMap ��fbo
	uint createPointShadowFbo();
	void bindTexture();
	void bindTexture(uint textureID);
	void bindShaderData();  // ��ubo 
	void render();

	//-----test--------
	void testCube();
	void testHightLight(); // �߹�
	void testBlending();	// ��͸������
	void testCullFace();	// cullFace ����
	void testFrameBuffer();  // ����֡����
	void testSkybox();		 // ������պ���
	void testUboData();		// ����UBO
	void testGeo();
	void testInstance(); //����ģ�ͣ���������
	void testBlinPhong();
	void testshadowMap(); // shadowMap
	void testPointShadowMap();

	//
	void renderScene(Shader *shader);
	void renderPointShadowScene(Shader* shader);
	void renderBox();

	// ����ƽ�й�
	void testPointLight();

	void camMove(CAM_MOVE type) {
		cam->move(type);
	}

	void mouseMove(double x, double y) {
		cam->mouseMove(x, y);
	}
};

