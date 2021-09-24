#pragma once
#include <string>
#include "Base.h"
#include "Camara.h"
#include "IO.h"

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
	Shader* dirShader;   // 平行光
	Shader* pointShader; // 点光源
	Shader* spotShader;  // 点光源
	Shader* sceneShader; // 多光源
	AImage* img;


	FF::ffModel* model;

	// vp 矩阵信息
	glm::mat4 vMatrix;
	glm::mat4 pMatrix;

	// 材质贴图，光照贴图
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
	
	void render();
	GLDraw(int width,int height);
	~GLDraw();


	void bindTexture();

	// ------------测试案例
	// 绘制多个立方体
	void testDrawCubes();

	// 测试材质，光照贴图等
	void testMaterial();

	// 测试平行光
	void testDirLight();

	// 测试聚光灯
	void testSpotLight();

	// 测试多光源
	void testManyLights();
	
};

