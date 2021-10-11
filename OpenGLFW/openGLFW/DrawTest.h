#pragma once

#include "Base.h"
#include "Camara.h"
#include "IO.h"

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
	Shader *skyShader; // 天空盒子
	Shader *shaderRed; // 测试ubo
	Shader *shaderGreen; //测试ubo
	Shader *shaderGeo; // 测试geo
	Shader *shaderBox;
	Shader *planetShader; // 行星
	Shader *rockShader;
	Shader *blinnPhone;
	Shader *shaderShadow;
	Shader *dirShader;//
	Shader *shaderPsDepth; // Ps :pointShadow
	Shader *shaderPointShadow; // 点光源的shadow
	Shader *pointShader; // 点光源

	// 材质贴图，光照贴图
	uint textureBox;
	uint textureSpec;
	uint textureWin;
	uint texturePlane;
	uint textureSkybox;

	// 箱体的VAO
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
	uint depthMap; // 深度贴图
	uint depthCubemap; // cubeMap 的深度缓存

	// 模型
	FF::ffModel* planet; //行星
	FF::ffModel* rock;	// 陨石
	FF::ffModel* bag; // 背包模型

	glm::mat4* matrixArr;
public:
	DrawTest(int w, int h);
	~DrawTest();

	void init();

	// instanceID 实例绘制类型
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
	uint createShadowFbo(); // shadowMap 的fbo
	uint createPointShadowFbo();
	void bindTexture();
	void bindTexture(uint textureID);
	void bindShaderData();  // 绑定ubo 
	void render();

	//-----test--------
	void testCube();
	void testHightLight(); // 高光
	void testBlending();	// 半透明绘制
	void testCullFace();	// cullFace 剪裁
	void testFrameBuffer();  // 测试帧缓存
	void testSkybox();		 // 测试天空盒子
	void testUboData();		// 测试UBO
	void testGeo();
	void testInstance(); //测试模型，批量绘制
	void testBlinPhong();
	void testshadowMap(); // shadowMap
	void testPointShadowMap();

	//
	void renderScene(Shader *shader);
	void renderPointShadowScene(Shader* shader);
	void renderBox();

	// 测试平行光
	void testPointLight();

	void camMove(CAM_MOVE type) {
		cam->move(type);
	}

	void mouseMove(double x, double y) {
		cam->mouseMove(x, y);
	}
};

