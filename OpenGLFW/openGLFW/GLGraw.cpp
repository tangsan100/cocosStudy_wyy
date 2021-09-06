#include "GLDraw.h"
#include "Base.h"
#include "Shader.h"
#include "AImage.h"

GLDraw::GLDraw(int width,int height):width(width),height(height) {
	shader = new Shader();
	vMatrix = glm::mat4(1.0f);
	pMatrix = glm::mat4(1.0f);
}

GLDraw::~GLDraw() {
	delete shader;
}

void GLDraw::init() {
	initModel();
	initTexture();
	shader->initShader("vertexShader.glsl", "fragmentShader.glsl");
}

void GLDraw::initTexture() {
	img = AImage::loadImage("res/wall.jpg");

	
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// 尺寸超出的时候，采用平铺的模式， s 水平， T 竖直
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 缩小的时候取临近像素， 放大的时候取线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->getWidth(),img->getHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,img->getData());
}


void GLDraw::initModel() {
	/*float vertices[] = {
		0.5f,0.5f,0.0f, 1.0f,0.0f,0.0f,	1.0f,1.0f,
		0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,	1.0f,0.0f,
		-0.5f,-0.5f,0.0f,   0.0f,0.0f,1.0f,	0.0f,0.0f,
		-0.5f,0.5f,0.0f,   0.0f,1.0f,0.0f,	0.0f,1.0f, 
	};*/

	float vertices[] = {
		-0.5f,-0.5f,-0.5f,		0.0f,0.0f,
		0.5f,-0.5f,-0.5f,		1.0f,0.0f,
		0.5f,0.5f,-0.5f,		1.0f,1.0f,
		0.5f,0.5f,-0.5f,		1.0f,1.0f,
		-0.5f,0.5f,-0.5f,		1.0f,0.0f,
		-0.5f,-0.5f,-0.5f,		0.0f,0.0f,

		-0.5f,-0.5f,0.5f,		0.0f,0.0f,
		0.5f,-0.5f,0.5f,		1.0f,0.0f,
		0.5f,0.5f,0.5f,		1.0f,1.0f,
		0.5f,0.5f,0.5f,		1.0f,1.0f,
		-0.5f,0.5f,0.5f,		0.0f,1.0f,
		-0.5f,-0.5f,0.5f,		0.0f,0.0f,

		-0.5f,0.5f,0.5f,		1.0f,0.0f,
		-0.5f,0.5f,-0.5f,		1.0f,1.0f,
		-0.5f,-0.5f,-0.5f,		0.0f,1.0f,
		-0.5f,-0.5f,-0.5f,		0.0f,1.0f,
		-0.5f,-0.5f,0.5f,		0.0f,0.0f,
		-0.5f,0.5f,0.5f,		1.0f,0.0f,


		0.5f,0.5f,0.5f,		1.0f,0.0f,
		0.5f,0.5f,-0.5f,		1.0f,1.0f,
		0.5f,-0.5f,-0.5f,		0.0f,1.0f,
		0.5f,-0.5f,-0.5f,		0.0f,1.0f,
		0.5f,-0.5f,0.5f,		0.0f,0.0f,
		0.5f,0.5f,0.5f,		1.0f,0.0f,

		-0.5f,-0.5f,-0.5f,		0.0f,1.0f,
		0.5f,-0.5f,-0.5f,		1.0f,1.0f,
		0.5f,-0.5f,0.5f,		1.0f,0.0f,
		0.5f,-0.5f,0.5f,		1.0f,0.0f,
		0.5f,-0.5f,-0.5f,		1.0f,1.0f,
		-0.5f,-0.5f,-0.5f,		0.0f,1.0f,

		-0.5f,0.5f,-0.5f,		0.0f,1.0f,
		0.5f,0.5f,-0.5f,		1.0f,1.0f,
		0.5f,0.5f,0.5f,		1.0f,0.0f,
		0.5f,0.5f,0.5f,		1.0f,0.0f,
		0.5f,0.5f,-0.5f,		1.0f,1.0f,
		-0.5f,0.5f,-0.5f,		0.0f,1.0f,
	};

	// EBO 模式
	/*int indices[] = {
		0,1,3,
		1,2,3
	};*/
	// VAO 在core 模式下是必须的，在非core 模式下可以不用VAO 接管
	// 使用VAO 的目的是，每个VAO 都能管理一个VBO，如果不使用VAO
	// 那么 后面的VBO 数据会覆盖掉前面的VBO的数据
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	//EBO 相关操作
	/*unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
	


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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)*3));
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*6));

	// 5. 激活锚点
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	// 解绑VBO、VA0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}



void GLDraw::rander() {

	
	// 设置要清理画布的颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// 清理画布
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// mvp 矩阵变换信息
	vMatrix = glm::lookAt(glm::vec3(3.0, 3.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	glBindTexture(GL_TEXTURE_2D, textureID);
	// 使用shader 程序
	shader->start();

	// 传入vp 信息
	shader->setMatrix("vMatrix", vMatrix);
	shader->setMatrix("pMatrix", pMatrix);

	//shader->shaderGreenTest();

	// 绑定VAO 主程序
	glBindVertexArray(VAO);
	
	
	// 告诉OpenGL 要画一个三角形， 从第0个顶点开始，有效顶点数3个
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// EBO的绘制方式
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// 解绑shader 程序
	shader->end();
}
