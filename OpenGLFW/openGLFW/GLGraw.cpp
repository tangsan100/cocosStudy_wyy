#include "GLDraw.h"
#include "Base.h"
#include "Shader.h"

GLDraw::GLDraw() {
	shader = new Shader();
}

void GLDraw::init() {
	initModel();
	shader->initShader("vertexShader.glsl", "fragmentShader.glsl");
}


void GLDraw::initModel() {
	float vertices[] = {
		-0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,
		0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,
		0.0f,0.5f,0.0f,   0.0f,0.0f,1.0f
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float)*3));

	// 5. 激活锚点
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// 解绑VBO、VA0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



void GLDraw::rander() {
	// 使用shader 程序
	shader->start();


	shader->shaderGreenTest();

	// 绑定VAO 主程序
	glBindVertexArray(VAO);
	
	// 告诉OpenGL 要画一个三角形， 从第0个顶点开始，有效顶点数3个
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// 解绑shader 程序
	shader->end();
}
