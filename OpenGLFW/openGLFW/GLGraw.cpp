#include "GLDraw.h"
#include "Base.h"
#include "Shader.h"
#include "AImage.h"
#include "Camara.h"

GLDraw::GLDraw(int width,int height):width(width),height(height) {
	cam = new Camara();
	cubeShader = new Shader();
	sunShader = new Shader();
	vMatrix = glm::mat4(1.0f);
	pMatrix = glm::mat4(1.0f);
}

GLDraw::~GLDraw() {
	delete cubeShader;
	delete sunShader;
}

void GLDraw::init() {
	// �����ʼ��
	glm::vec3 eyes(0.0f, 0.0f, 3.0f);
	glm::vec3 center(0.0f, 0.0f, 1.0f);
	cam->lookAt(eyes, center-eyes , glm::vec3(0.0f, 1.0f, 0.0f));
	cam->setSensitivity(0.1f);


	VAO_cube = createModel();
	VAO_sun = createModel();
	initTexture();

	cubeShader->initShader("vertexShader.glsl", "fragmentShader.glsl");
	sunShader->initShader("vSunShader.glsl", "fSunShader.glsl");
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


uint GLDraw::createModel() {
	/*float vertices[] = {
		0.5f,0.5f,0.0f, 1.0f,0.0f,0.0f,	1.0f,1.0f,
		0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,	1.0f,0.0f,
		-0.5f,-0.5f,0.0f,   0.0f,0.0f,1.0f,	0.0f,0.0f,
		-0.5f,0.5f,0.0f,   0.0f,1.0f,0.0f,	0.0f,1.0f, 
	};*/

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*5));
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*6));

	// 5. ����ê��
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// ���VBO��VA0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	return VAO;
}



void GLDraw::rander() {

	
	cam->update();

	// ����Ҫ����������ɫ
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// ̫�����λ��
	glm::vec3 lightPos(3.0f,0.0f,-1.0f);
	// ̫�����ǿ��
	glm::vec3 sunLight(1.0f, 1.0f, 1.0f);
	// �������ǿ��
	float ambientStrength = 0.2f;
	


	// mvp ����任��Ϣ
	vMatrix = cam->getVMatrix(); //glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	glBindTexture(GL_TEXTURE_2D, textureID);


		glm::mat4 mMatrix(1.0f);
		/*mMatrix = glm::translate(mMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		mMatrix = glm::rotate(mMatrix, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/

		// ʹ��shader ����
		cubeShader->start();

		// ����vp ��Ϣ
		cubeShader->setMatrix("mMatrix", mMatrix);
		cubeShader->setMatrix("vMatrix", vMatrix);
		cubeShader->setMatrix("pMatrix", pMatrix);

		// ���������Ϣ
		cubeShader->setVec3("lightPos", lightPos);
		cubeShader->setVec3("viewPos", cam->getPosition());
		cubeShader->setVec3("sunLight", sunLight);
		cubeShader->setFloat("ambientStrength", ambientStrength);

		//shader->shaderGreenTest();

		// ��VAO ������
		glBindVertexArray(VAO_cube);


		// ����OpenGL Ҫ��һ�������Σ� �ӵ�0�����㿪ʼ����Ч������3��
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// EBO�Ļ��Ʒ�ʽ
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// ���shader ����
		cubeShader->end();

		// ̫�����λ��
		mMatrix = glm::mat4(1.0f);
		mMatrix = glm::translate(mMatrix, glm::vec3(3.0f, 1.0f, -1.0f));
		
		sunShader->start();
		sunShader->setMatrix("mMatrix", mMatrix);
		sunShader->setMatrix("vMatrix", vMatrix);
		sunShader->setMatrix("pMatrix", pMatrix);
		glBindVertexArray(VAO_sun);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		sunShader->end();

	
}

void GLDraw::testDrawCubes() {
	glm::vec3 modelVecs[] = {
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(2.0f,5.0f,-15.0f),
		glm::vec3(-1.5f,-2.2f,-2.5f),
		glm::vec3(-3.8f,-2.0f,-12.3f),
		glm::vec3(2.4f,-0.4f,-3.5f),
		glm::vec3(-1.7f,3.0f,-7.5f),
		glm::vec3(1.3f,-2.0f,-2.5f),
		glm::vec3(1.5f,2.0f,-1.5f),
		glm::vec3(1.5f,0.2f,-1.5f),
		glm::vec3(-1.3f,1.0f,-1.5f),
	};

	// mvp ����任��Ϣ
	vMatrix = cam->getVMatrix(); //glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	glBindTexture(GL_TEXTURE_2D, textureID);

	for (int i = 0; i < 10; ++i)
	{
		glm::mat4 mMatrix(1.0f);
		mMatrix = glm::translate(mMatrix, modelVecs[i]);
		mMatrix = glm::rotate(mMatrix, glm::radians(float(glfwGetTime())*(i + 1) * 5), glm::vec3(0.0f, 1.0f, 0.0f));

		// ʹ��shader ����
		cubeShader->start();

		// ����vp ��Ϣ
		cubeShader->setMatrix("mMatrix", mMatrix);
		cubeShader->setMatrix("vMatrix", vMatrix);
		cubeShader->setMatrix("pMatrix", pMatrix);

		//shader->shaderGreenTest();

		// ��VAO ������
		glBindVertexArray(VAO_cube);


		// ����OpenGL Ҫ��һ�������Σ� �ӵ�0�����㿪ʼ����Ч������3��
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// EBO�Ļ��Ʒ�ʽ
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// ���shader ����
		cubeShader->end();

	}
}
