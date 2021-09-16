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
	Shader *shaderBlending; // 半透明
	Shader *screenShader; // frameBuffer

	// 材质贴图，光照贴图
	uint textureBox;
	uint textureSpec;
	uint textureWin;
	uint texturePlane;

	// 箱体的VAO
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
	void testHightLight(); // 高光
	void testBlending();	// 半透明绘制
	void testCullFace();	// cullFace 剪裁
	void testFrameBuffer();  // 测试帧缓存

	void camMove(CAM_MOVE type) {
		cam->move(type);
	}

	void mouseMove(double x, double y) {
		cam->mouseMove(x, y);
	}
};

