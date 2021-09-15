#pragma once

#include "Base.h"
#include "Camara.h"

class Shader;

class DrawTest
{
private:

	int width;  //画布宽度
	int height; //画布高度

	Camara* cam; // 相机
	Shader *shaderColor; // 高光
	Shader *shaderCube;

	// 材质贴图，光照贴图
	uint textureBox;
	uint textureSpec;

	// 箱体的VAO
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
	void testHightLight(); // 高光

	void camMove(CAM_MOVE type) {
		cam->move(type);
	}

	void mouseMove(double x, double y) {
		cam->mouseMove(x, y);
	}
};

