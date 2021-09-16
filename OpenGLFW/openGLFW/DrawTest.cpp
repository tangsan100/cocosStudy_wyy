#include "DrawTest.h"
#include "Shader.h"
#include "AImage.h"
#include "gData.h"
#include <map>

DrawTest::DrawTest(int w,int h):width(w),height(h){
	cam				= new Camara();
	shaderColor		= new Shader();
	shaderCube		= new Shader();
	shaderBlending	= new Shader();
	screenShader	= new Shader();
}


DrawTest::~DrawTest() {
	
	delete cam;
	delete shaderCube;
	delete shaderColor;
	delete shaderBlending;
	delete screenShader;
}

void DrawTest::init() {
	// 初始化相机
	glm::vec3 eyes(0.0f, 0.0f, 3.0f);	// 观察位置
	glm::vec3 front(0.0f, 0.0f, 1.0f);	// 前方位置
	glm::vec3 popUp(0.0f, 1.0f, 0.0f);  // 相机仰角
	cam->lookAt(eyes, front, popUp);
	cam->setSensitivity(0.1f);			// 设置相机的灵敏度

	// 问题问题贴图
	textureBox = createTexture("res/box.png");
	textureSpec = createTexture("res/specular.png");
	textureWin = createTexture("res/window.png");
	texturePlane = createTexture("res/stone.png");

	// VAO 初始化
	VAO_cube = createModel();
	VAO_plane = createPlane();
	VAO_win = createWindow();
	frameBuffer = createFramBuffer();
	VAO_texture = createScreenPlane();

	// 初始化shader
	shaderColor->initShader("shader/colorV.glsl", "shader/colorF.glsl");
	shaderCube->initShader("shader/cubeV.glsl", "shader/cubeF.glsl");
	shaderBlending->initShader("shader/BlendingV.glsl", "shader/BlendingF.glsl");
	screenShader->initShader("shader/frameBufferV.glsl", "shader/frameBufferF.glsl");
}

uint DrawTest::createTexture(const char* fileName) {
	AImage* img = AImage::loadImage(fileName);


	uint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// 尺寸超出的时候，采用平铺的模式， s 水平， T 竖直
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 缩小的时候取临近像素， 放大的时候取线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->getWidth(), img->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img->getData());

	delete img;
	return textureID;
}

uint DrawTest::createModel() {


	uint VAO;
	uint VBO;

	
	// VAO 在core 模式下是必须的，在非core 模式下可以不用VAO 接管
	// 使用VAO 的目的是，每个VAO 都能管理一个VBO，如果不使用VAO
	// 那么 后面的VBO 数据会覆盖掉前面的VBO的数据
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	// VBO 程序相关的操作
	// 1, 获取vbo 的index
	glGenBuffers(1, &VBO);

	// 2. 绑定vbo 的index
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// 3. 给vbo分配显存空间 传输数据, STATIC_DRAW 告诉GPU 顶点数据不会变动
	glBufferData(GL_ARRAY_BUFFER, sizeof(gCubeVertices), gCubeVertices, GL_STATIC_DRAW);

	// 4. 告诉shader数据解析方式
	// 参数1： 从哪个位置开始读，锚点，layout 的位置信息
	// 参数2：每次读取多少个
	// 参数3：数据类型
	// 参数4：是否归1化
	// 参数5：步长
	// 参数6：起始地址
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 顶点
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3)); //法线
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));	// UV

	// 5. 激活锚点
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// 解绑VBO、VA0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

uint DrawTest::createPlane() {
	uint VAO = 0;
	uint VBO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	glBufferData(GL_ARRAY_BUFFER, sizeof(gPlaneVextices), gPlaneVextices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 顶点
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Normal
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // UV

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	return VAO;
}

uint DrawTest::createWindow() {
	uint VAO = 0;
	uint VBO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	

	glBufferData(GL_ARRAY_BUFFER, sizeof(gWinVextices), gWinVextices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 顶点
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // 顶点
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // 顶点

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	return VAO;
}

uint DrawTest::createFramBuffer() {
	uint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//color buffer
	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
	// 分配空间
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,nullptr);
	// 插值运算
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 绑定textureBuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);
	

	// D/S buffer
	uint RBO = 0;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	// 24 位分配给深度缓存，8位分配给stencil
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// 解绑
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return FBO;

}

uint DrawTest::createScreenPlane() {
	uint VAO = 0;
	uint VBO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	float quadVertices[] = {
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // 顶点
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // 顶点

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	return VAO;


}
// 启用纹理
void DrawTest::bindTexture() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBox);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureSpec);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureWin);
}

void DrawTest::bindTexture(uint textureId) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void DrawTest::render() {
	cam->update();

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	//glEnable(GL_STENCIL_TEST);

	// 设置要清理画布的颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// 清理画布
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	


	// 高光绘制
	//testHightLight();
	// 透明绘制
	//testBlending();
	// cullface
	//testCullFace();
	// 帧缓存
	testFrameBuffer();
	
}

void DrawTest::testHightLight() {
	bindTexture();

	glStencilFunc(GL_ALWAYS, 1.0f, 0xff);
	glStencilMask(0xff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	shaderCube->start();
	// mvp
	glm::mat4 mMatrix(1.0f);
	mMatrix = glm::translate(mMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 vMatrix = cam->getVMatrix();
	glm::mat4 pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	shaderCube->setMatrix("mMatrix", mMatrix);
	shaderCube->setMatrix("vMatrix", vMatrix);
	shaderCube->setMatrix("pMatrix", pMatrix);

	glBindVertexArray(VAO_cube);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shaderCube->end();

	// 高光效果
	glStencilFunc(GL_NOTEQUAL, 1, 0xff);
	glStencilMask(0x00);
	shaderColor->start();
	mMatrix = glm::scale(mMatrix, glm::vec3(1.05f, 1.05f, 1.05f));
	shaderColor->setMatrix("mMatrix", mMatrix);
	shaderColor->setMatrix("vMatrix", vMatrix);
	shaderColor->setMatrix("pMatrix", pMatrix);
	glBindVertexArray(VAO_cube);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shaderColor->end();

	glStencilMask(0xff);
	glStencilFunc(GL_ALWAYS, 1.0f, 0xff);
}


void DrawTest::testBlending() {
	
	// 开启颜色混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//this->bindTexture();
	// mvp
	glm::mat4 mMatrix(1.0f);
	mMatrix = glm::translate(mMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 vMatrix = cam->getVMatrix();
	glm::mat4 pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	shaderBlending->start();
	shaderBlending->setMatrix("mMatrix", mMatrix);
	shaderBlending->setMatrix("vMatrix", vMatrix);
	shaderBlending->setMatrix("pMatrix", pMatrix);

	this->bindTexture(texturePlane);
	glBindVertexArray(VAO_plane);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	this->bindTexture(textureBox);
	glBindVertexArray(VAO_cube);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	this->bindTexture(textureWin);

	// 先排序，根据与摄像机的距离来排序
	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < gWinPos.size(); i++)
	{
		float distance = glm::length(cam->getPosition() - gWinPos[i]);
		sorted[distance] = gWinPos[i];
	}

	// 比较远的先绘制
	for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
		mMatrix = glm::mat4(1.0f);
		mMatrix = glm::translate(mMatrix, it->second);
		shaderBlending->setMatrix("mMatrix", mMatrix);
		glBindVertexArray(VAO_win);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	

	shaderBlending->end();
}

void DrawTest::testCullFace() {

	bindTexture();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // 剪裁正面
	glFrontFace(GL_CW); // 顺时针方向为正

	
	shaderCube->start();
	// mvp
	glm::mat4 mMatrix(1.0f);
	mMatrix = glm::translate(mMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 vMatrix = cam->getVMatrix();
	glm::mat4 pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	shaderCube->setMatrix("mMatrix", mMatrix);
	shaderCube->setMatrix("vMatrix", vMatrix);
	shaderCube->setMatrix("pMatrix", pMatrix);

	glBindVertexArray(VAO_cube);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shaderCube->end();

}

//帧缓存测试
void DrawTest::testFrameBuffer() {

	// 开启framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 开启颜色混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/**************************1 pass ********************/

	//this->bindTexture();
	// mvp
	glm::mat4 mMatrix(1.0f);
	mMatrix = glm::translate(mMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 vMatrix = cam->getVMatrix();
	glm::mat4 pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	shaderBlending->start();
	shaderBlending->setMatrix("mMatrix", mMatrix);
	shaderBlending->setMatrix("vMatrix", vMatrix);
	shaderBlending->setMatrix("pMatrix", pMatrix);

	this->bindTexture(texturePlane);
	glBindVertexArray(VAO_plane);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	this->bindTexture(textureBox);
	glBindVertexArray(VAO_cube);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	this->bindTexture(textureWin);

	// 先排序，根据与摄像机的距离来排序
	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < gWinPos.size(); i++)
	{
		float distance = glm::length(cam->getPosition() - gWinPos[i]);
		sorted[distance] = gWinPos[i];
	}

	// 比较远的先绘制
	for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
		mMatrix = glm::mat4(1.0f);
		mMatrix = glm::translate(mMatrix, it->second);
		shaderBlending->setMatrix("mMatrix", mMatrix);
		glBindVertexArray(VAO_win);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}


	shaderBlending->end();

	/**************************2 pass ********************/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	screenShader->start();
	glBindVertexArray(VAO_texture);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	screenShader->end();
}