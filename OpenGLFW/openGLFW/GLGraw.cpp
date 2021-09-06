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
	// �ߴ糬����ʱ�򣬲���ƽ�̵�ģʽ�� s ˮƽ�� T ��ֱ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ��С��ʱ��ȡ�ٽ����أ� �Ŵ��ʱ��ȡ���Բ�ֵ
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

	// EBO ģʽ
	/*int indices[] = {
		0,1,3,
		1,2,3
	};*/
	// VAO ��core ģʽ���Ǳ���ģ��ڷ�core ģʽ�¿��Բ���VAO �ӹ�
	// ʹ��VAO ��Ŀ���ǣ�ÿ��VAO ���ܹ���һ��VBO�������ʹ��VAO
	// ��ô �����VBO ���ݻḲ�ǵ�ǰ���VBO������
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	//EBO ��ز���
	/*unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
	


	// VBO ������صĲ���
	// 1, ��ȡvbo ��index
	glGenBuffers(1, &VBO);

	// 2. ��vbo ��index
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// 3. ��vbo�����Դ�ռ� ��������, STATIC_DRAW ����GPU �������ݲ���䶯
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 4. ����shader���ݽ�����ʽ
	// ����1�� ���ĸ�λ�ÿ�ʼ����ê�㣬layout ��λ����Ϣ
	// ����2��ÿ�ζ�ȡ���ٸ�
	// ����3����������
	// ����4���Ƿ��1��
	// ����5������
	// ����6����ʼ��ַ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float)*3));
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*6));

	// 5. ����ê��
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	// ���VBO��VA0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}



void GLDraw::rander() {

	
	// ����Ҫ����������ɫ
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// mvp ����任��Ϣ
	vMatrix = glm::lookAt(glm::vec3(3.0, 3.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	glBindTexture(GL_TEXTURE_2D, textureID);
	// ʹ��shader ����
	shader->start();

	// ����vp ��Ϣ
	shader->setMatrix("vMatrix", vMatrix);
	shader->setMatrix("pMatrix", pMatrix);

	//shader->shaderGreenTest();

	// ��VAO ������
	glBindVertexArray(VAO);
	
	
	// ����OpenGL Ҫ��һ�������Σ� �ӵ�0�����㿪ʼ����Ч������3��
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// EBO�Ļ��Ʒ�ʽ
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// ���shader ����
	shader->end();
}
