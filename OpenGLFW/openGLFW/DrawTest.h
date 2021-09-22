#pragma once

#include "Base.h"
#include "Camara.h"

class Shader;

class DrawTest
{
private:

	int width;  //»­²¼¿í¶È
	int height; //»­²¼¸ß¶È

	Camara* cam; // Ïà»ú
	Shader *shaderColor; // ¸ß¹â
	Shader *shaderCube;
	Shader *shaderBlending; // °ëÍ¸Ã÷
	Shader *screenShader; // frameBuffer
	Shader *skyShader; // Ìì¿ÕºÐ×Ó
	Shader *shaderRed; // ²âÊÔubo
	Shader *shaderGreen; //²âÊÔubo
	Shader *shaderGeo; // ²âÊÔgeo
	Shader *shaderBox;

	// ²ÄÖÊÌùÍ¼£¬¹âÕÕÌùÍ¼
	uint textureBox;
	uint textureSpec;
	uint textureWin;
	uint texturePlane;
	uint textureSkybox;

	// ÏäÌåµÄVAO
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
	void bindShaderData();  // °ó¶¨ubo 
	void render();

	//-----test--------
	void testHightLight(); // ¸ß¹â
	void testBlending();	// °ëÍ¸Ã÷»æÖÆ
	void testCullFace();	// cullFace ¼ô²Ã
	void testFrameBuffer();  // ²âÊÔÖ¡»º´æ
	void testSkybox();		 // ²âÊÔÌì¿ÕºÐ×Ó
	void testUboData();		// ²âÊÔUBO
	void testGeo();

	void camMove(CAM_MOVE type) {
		cam->move(type);
	}

	void mouseMove(double x, double y) {
		cam->mouseMove(x, y);
	}
};

