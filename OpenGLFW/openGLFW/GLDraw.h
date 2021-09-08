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


	unsigned int VAO_cube = 0;
	unsigned int VAO_sun = 0;
	unsigned int textureID = 0;
	Camara* cam;
	Shader* cubeShader;
	Shader* sunShader;
	AImage* img;

	// vp 矩阵信息
	glm::mat4 vMatrix;
	glm::mat4 pMatrix;
public:
	void init();
	void initTexture();
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


	// ------------测试案例
	// 绘制多个立方体
	void testDrawCubes();
	
};

