#include "DrawTest.h"
#include "Shader.h"
#include "AImage.h"

DrawTest::DrawTest(int w,int h):width(w),height(h){
	cam			= new Camara();
	shaderColor = new Shader();
	shaderCube  = new Shader();
}


DrawTest::~DrawTest() {
	delete shaderColor;
	delete cam;
	delete shaderCube;
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

	// VAO 初始化
	VAO_cube = createModel();

	// 初始化shader
	shaderColor->initShader("shader/colorV.glsl", "shader/colorF.glsl");
	shaderCube->initShader("shader/cubeV.glsl", "shader/cubeF.glsl");
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

	//delete img;
	return textureID;
}

uint DrawTest::createModel() {


	uint VAO;
	uint VBO;

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,	0.0f,0.0f,   0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,0.0f,	 0.0f,  0.0f, -1.0f,
		 0.5f,	0.5f, -0.5f,	1.0f,1.0f,	 0.0f,  0.0f, -1.0f,
		 0.5f,	0.5f, -0.5f,	1.0f,1.0f,	 0.0f,  0.0f, -1.0f,
		-0.5f,	0.5f, -0.5f,	0.0f,1.0f,	 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,0.0f,	 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,	0.5f,	0.0f,0.0f,	 0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,	0.5f,	1.0f,0.0f,	 0.0f,  0.0f,  1.0f,
		 0.5f,	0.5f,	0.5f,	1.0f,1.0f,	 0.0f,  0.0f,  1.0f,
		 0.5f,	0.5f,	0.5f,	1.0f,1.0f,	 0.0f,  0.0f,  1.0f,
		-0.5f,	0.5f,	0.5f,	0.0f,1.0f,	 0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,	0.5f,	0.0f,0.0f,	 0.0f,  0.0f,  1.0f,

		-0.5f,	0.5f,	0.5f,	1.0f,0.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f,	0.5f,  -0.5f,	1.0f,1.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,1.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,1.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,	0.5f,	0.0f,0.0f,	-1.0f,  0.0f,  0.0f,
		-0.5f,	0.5f,	0.5f,	1.0f,0.0f,	-1.0f,  0.0f,  0.0f,


		0.5f,	0.5f,	0.5f,	1.0f,0.0f,	 1.0f,  0.0f, 0.0f,
		0.5f,	0.5f,  -0.5f,	1.0f,1.0f,	 1.0f,  0.0f, 0.0f,
		0.5f,  -0.5f,  -0.5f,	0.0f,1.0f,	 1.0f,  0.0f, 0.0f,
		0.5f,  -0.5f,  -0.5f,	0.0f,1.0f,	 1.0f,  0.0f, 0.0f,
		0.5f,  -0.5f,	0.5f,	0.0f,0.0f,	 1.0f,  0.0f, 0.0f,
		0.5f,	0.5f,	0.5f,	1.0f,0.0f,	 1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  -0.5f,	0.0f,1.0f,	 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  -0.5f,	1.0f,1.0f,	 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,	0.5f,	1.0f,0.0f,	 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,	0.5f,	1.0f,0.0f,	 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,   0.5f,	0.0f,0.0f,	 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,1.0f,	 0.0f, -1.0f, 0.0f,

		-0.5f,	0.5f,  -0.5f,	0.0f,1.0f,	 0.0f, 1.0f, 0.0f,
		 0.5f,	0.5f,  -0.5f,	1.0f,1.0f,	 0.0f, 1.0f, 0.0f,
		 0.5f,	0.5f,	0.5f,	1.0f,0.0f,	 0.0f, 1.0f, 0.0f,
		 0.5f,	0.5f,	0.5f,	1.0f,0.0f,	 0.0f, 1.0f, 0.0f,
		-0.5f,	0.5f,   0.5f,	1.0f,0.0f,	 0.0f, 1.0f, 0.0f,
		-0.5f,	0.5f,  -0.5f,	0.0f,1.0f,	 0.0f, 1.0f, 0.0f,
	};

	
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 4. 告诉shader数据解析方式
	// 参数1： 从哪个位置开始读，锚点，layout 的位置信息
	// 参数2：每次读取多少个
	// 参数3：数据类型
	// 参数4：是否归1化
	// 参数5：步长
	// 参数6：起始地址
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 顶点
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3)); //UV
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 5));	// 法线

	// 5. 激活锚点
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// 解绑VBO、VA0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

// 启用纹理
void DrawTest::bindTexture() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBox);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureSpec);
}

void DrawTest::render() {
	cam->update();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	// 设置要清理画布的颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// 清理画布
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	


	// 高光绘制
	testHightLight();
	
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
