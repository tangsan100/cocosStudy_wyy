#pragma once
#include <string>
#include "Base.h"
#include "Camara.h"

class Shader;
class AImage;


class GLDraw
{
private:

	int width;
	int height;


	uint VAO_cube = 0;
	uint VAO_sun = 0;

	Camara* cam;
	Shader* cubeShader;
	Shader* sunShader;
	Shader* dirShader;   // ƽ�й�
	Shader* pointShader; // ���Դ
	Shader* spotShader;  // ���Դ
	Shader* sceneShader; // ���Դ
	AImage* img;

	// vp ������Ϣ
	glm::mat4 vMatrix;
	glm::mat4 pMatrix;

	// ������ͼ��������ͼ
	uint textureBox;
	uint textureSpec;
public:
	void init();
	uint createTexture(const char* fileName);
	uint createModel();
	void camMove(CAM_MOVE type) {
		cam->move(type);
	}

	void mouseMove(double x, double y) {
		cam->mouseMove(x, y);
	}
	
	void rander();
	GLDraw(int width,int height);
	~GLDraw();


	void bindTexture();

	// ------------���԰���
	// ���ƶ��������
	void testDrawCubes();

	// ���Բ��ʣ�������ͼ��
	void testMaterial();

	// ����ƽ�й�
	void testDirLight();

	// ���Ծ۹��
	void testSpotLight();

	// ���Զ��Դ
	void testManyLights();
	
};

