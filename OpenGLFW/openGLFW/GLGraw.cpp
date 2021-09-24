#include "GLDraw.h"
#include "Base.h"
#include "Shader.h"
#include "AImage.h"
#include "Camara.h"


glm::vec3 modelVecs[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -1.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f),
};

glm::vec3 pointLightPos[] = {
	glm::vec3( 0.7f,  0.2f ,  2.0f),
	glm::vec3( 2.3f, -3.3f , -4.0f),
	glm::vec3(-4.0f,  2.0f , -12.0f),
	glm::vec3( 0.0f,  0.0f , -3.0f),
};

GLDraw::GLDraw(int width,int height):width(width),height(height) {
	cam			= new Camara();
	cubeShader	= new Shader();
	sunShader	= new Shader();
	dirShader	= new Shader();
	pointShader = new Shader();
	spotShader	= new Shader();
	sceneShader =  new Shader();

	vMatrix		= glm::mat4(1.0f);
	pMatrix		= glm::mat4(1.0f);

	
}

GLDraw::~GLDraw() {
	delete cubeShader;
	delete sunShader;
	delete dirShader;
	delete pointShader;
	delete spotShader;
	delete sceneShader;
}

void GLDraw::init() {
	// �����ʼ��
	glm::vec3 eyes(0.0f, 0.0f, 3.0f);
	glm::vec3 center(0.0f, 0.0f, 1.0f);
	cam->lookAt(eyes, center , glm::vec3(0.0f, 1.0f, 0.0f));
	cam->setSensitivity(0.1f);


	VAO_cube = createModel();
	VAO_sun = createModel();
	textureBox = createTexture("res/box.png");
	textureSpec = createTexture("res/specular.png");

	model = new FF::ffModel("res/backpack/backpack.obj");

	//cubeShader->initShader("shader/vertexShader.glsl", "shader/fragmentShader.glsl");
	sunShader->initShader("shader/vSunShader.glsl", "shader/fSunShader.glsl");
	dirShader->initShader("shader/dirShaderV.glsl", "shader/dirShaderF.glsl");
	pointShader->initShader("shader/pointShaderV.glsl", "shader/pointShaderF.glsl");
	spotShader->initShader("shader/spotShaderV.glsl", "shader/spotShaderF.glsl");
	sceneShader->initShader("shader/sceneShaderV.glsl", "shader/sceneShaderF.glsl");

	
}

uint GLDraw::createTexture(const char* fileName) {
	img = AImage::loadImage(fileName);

	
	uint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// �ߴ糬����ʱ�򣬲���ƽ�̵�ģʽ�� s ˮƽ�� T ��ֱ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ��С��ʱ��ȡ�ٽ����أ� �Ŵ��ʱ��ȡ���Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->getWidth(),img->getHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,img->getData());

	return textureID;
}


uint GLDraw::createModel() {


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



void GLDraw::render() {

	
	cam->update();

	// ����Ҫ����������ɫ
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//testMaterial();
	testDirLight();
	//testSpotLight();
	//testManyLights();
}

void GLDraw::bindTexture() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBox);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureSpec);
}

void GLDraw::testDrawCubes() {
	

	// mvp ����任��Ϣ
	vMatrix = cam->getVMatrix(); //glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	glBindTexture(GL_TEXTURE_2D, textureBox);

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

// ���Բ��ʣ�������ͼ��
void GLDraw::testMaterial() {
	// ̫�����λ��
	glm::vec3 lightPos(3.0f, 1.0f, -1.0f);
	// ̫�����ǿ��
	glm::vec3 sunLight(1.0f, 1.0f, 1.0f);



	// ���������ͼ
	this->bindTexture();

	// mvp ����任��Ϣ
	pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);
	glm::mat4 mMatrix(1.0f);

	// ʹ��shader ����
	cubeShader->start();

	// ����vp ��Ϣ
	cubeShader->setMatrix("mMatrix", mMatrix);
	cubeShader->setMatrix("vMatrix", cam->getVMatrix());
	cubeShader->setMatrix("pMatrix", pMatrix);
	cubeShader->setVec3("viewPos", cam->getPosition());

	// �����������
	cubeShader->setVec3("Light.pos", lightPos);
	cubeShader->setVec3("Light.ambient", sunLight*glm::vec3(0.1f));
	cubeShader->setVec3("Light.diffuse", sunLight*glm::vec3(0.7f));
	cubeShader->setVec3("Light.specular", sunLight*glm::vec3(0.5f));

	// �����������
	cubeShader->setInt("Matrial.diffuse", 0);
	cubeShader->setInt("Matrial.specular", 1);
	cubeShader->setFloat("Matrial.shiness", 32);

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
	sunShader->setMatrix("vMatrix", cam->getVMatrix());
	sunShader->setMatrix("pMatrix", pMatrix);
	glBindVertexArray(VAO_sun);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	sunShader->end();
}

// ����ƽ�й�
void GLDraw::testDirLight() {
	// ̫�����λ��
		glm::vec3 lightPos(3.0f, 1.0f, -1.0f);
	// ̫�����ǿ��
	glm::vec3 sunLight(1.0f, 1.0f, 1.0f);



	// ���������ͼ
	this->bindTexture();

	// mvp ����任��Ϣ
	pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);
	glm::mat4 mMatrix(1.0f);

	// ʹ��shader ����
	pointShader->start();


	// �����������
	pointShader->setVec3("Light.pos", lightPos);
	pointShader->setVec3("Light.ambient", sunLight*glm::vec3(0.1f));
	pointShader->setVec3("Light.diffuse", sunLight*glm::vec3(0.7f));
	pointShader->setVec3("Light.specular", sunLight*glm::vec3(0.5f));
	pointShader->setFloat("Light.c", 1.0f);
	pointShader->setFloat("Light.l", 0.07f);
	pointShader->setFloat("Light.q", 0.017f);


	// �����������
	/*pointShader->setInt("myMatrial.diffuse1", 0);
	pointShader->setInt("myMatrial.specular1", 1);*/
	pointShader->setFloat("myMaterial.shiness", 32);

	//pointShader->setFloat("myMatrial.shiness", 32);

	// ����vp ��Ϣ
	
	pointShader->setMatrix("vMatrix", cam->getVMatrix());
	pointShader->setMatrix("pMatrix", pMatrix);
	pointShader->setVec3("viewPos", cam->getPosition());


	/*for (int i = 0; i < 9; i++)
	{
		mMatrix = glm::mat4(1.0f);
		mMatrix = glm::translate(mMatrix, modelVecs[i]);
		mMatrix = glm::rotate(mMatrix, glm::radians(i*10.0f), glm::vec3(0.0, 1.0, 0.0));
		pointShader->setMatrix("mMatrix", mMatrix);

		glBindVertexArray(VAO_cube);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}*/

	model->draw(pointShader);

	// ���shader ����
	pointShader->end();

	// ̫�����λ��
	mMatrix = glm::mat4(1.0f);
	mMatrix = glm::translate(mMatrix, glm::vec3(3.0f, 1.0f, -1.0f));

	sunShader->start();
	sunShader->setMatrix("mMatrix", mMatrix);
	sunShader->setMatrix("vMatrix", cam->getVMatrix());
	sunShader->setMatrix("pMatrix", pMatrix);
	glBindVertexArray(VAO_sun);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	sunShader->end();
}

void GLDraw::testSpotLight() {
	// ̫�����λ��
	glm::vec3 lightPos(3.0f, 1.0f, -1.0f);
	// ̫�����ǿ��
	glm::vec3 sunLight(1.0f, 1.0f, 1.0f);



	// ���������ͼ
	this->bindTexture();

	// mvp ����任��Ϣ
	pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);
	glm::mat4 mMatrix(1.0f);

	// ʹ��shader ����
	spotShader->start();


	// �����������
	spotShader->setVec3("Light.pos", cam->getPosition());
	spotShader->setVec3("Light.dir", cam->getDirection());
	spotShader->setFloat("Light.cutOff", cos(glm::radians(12.5f)));
	spotShader->setFloat("Light.outCutOff", cos(glm::radians(20.5f)));
	spotShader->setVec3("Light.ambient", sunLight*glm::vec3(0.1f));
	spotShader->setVec3("Light.diffuse", sunLight*glm::vec3(0.7f));
	spotShader->setVec3("Light.specular", sunLight*glm::vec3(0.5f));
	spotShader->setFloat("Light.c", 1.0f);
	spotShader->setFloat("Light.l", 0.07f);
	spotShader->setFloat("Light.q", 0.017f);
	


	// �����������
	spotShader->setInt("Matrial.diffuse", 0);
	spotShader->setInt("Matrial.specular", 1);
	spotShader->setFloat("Matrial.shiness", 32);

	// ����vp ��Ϣ
	spotShader->setMatrix("vMatrix", cam->getVMatrix());
	spotShader->setMatrix("pMatrix", pMatrix);
	spotShader->setVec3("viewPos", cam->getPosition());


	for (int i = 0; i < 9; i++)
	{
		mMatrix = glm::mat4(1.0f);
		mMatrix = glm::translate(mMatrix, modelVecs[i]);
		mMatrix = glm::rotate(mMatrix, glm::radians(i*10.0f), glm::vec3(0.0, 1.0, 0.0));
		spotShader->setMatrix("mMatrix", mMatrix);

		glBindVertexArray(VAO_cube);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// ���shader ����
	spotShader->end();

	// ̫�����λ��
	
	sunShader->start();
	
	sunShader->setMatrix("vMatrix", cam->getVMatrix());
	sunShader->setMatrix("pMatrix", pMatrix);

	for (int i = 0; i < 4; ++i)
	{
		mMatrix = glm::mat4(1.0f);
		mMatrix = glm::translate(mMatrix, pointLightPos[i]);
		mMatrix = glm::scale(mMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
		sunShader->setMatrix("mMatrix", mMatrix);
		glBindVertexArray(VAO_sun);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	
	sunShader->end();
}


void GLDraw::testManyLights() {

	// ���������ͼ
	this->bindTexture();


	// mvp ����任��Ϣ
	pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);
	glm::mat4 mMatrix(1.0f);


	// ʹ��shader ����
	sceneShader->start();

	// �����������
	sceneShader->setInt("Matrial.diffuse", 0);
	sceneShader->setInt("Matrial.specular", 1);
	sceneShader->setFloat("Matrial.shiness", 32);

	// ����vp ��Ϣ
	sceneShader->setMatrix("vMatrix", cam->getVMatrix());
	sceneShader->setMatrix("pMatrix", pMatrix);
	sceneShader->setVec3("viewPos", cam->getPosition());

	// direction light
	sceneShader->setVec3("dLight.dir", glm::vec3(0.2f, -1.0f, -0.3f));
	sceneShader->setVec3("dLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	sceneShader->setVec3("dLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
	sceneShader->setVec3("dLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

	// point light1
	sceneShader->setVec3("pLight[0].pos", pointLightPos[0]);
	sceneShader->setVec3("pLight[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	sceneShader->setVec3("pLight[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	sceneShader->setVec3("pLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	sceneShader->setFloat("pLight[0].c", 1.0f);
	sceneShader->setFloat("pLight[0].l", 0.09f);
	sceneShader->setFloat("pLight[0].q", 0.032f);

	// point light2
	sceneShader->setVec3("pLight[1].pos", pointLightPos[1]);
	sceneShader->setVec3("pLight[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	sceneShader->setVec3("pLight[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	sceneShader->setVec3("pLight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	sceneShader->setFloat("pLight[1].c", 1.0f);
	sceneShader->setFloat("pLight[1].l", 0.09f);
	sceneShader->setFloat("pLight[1].q", 0.032f);

	// point light3
	sceneShader->setVec3("pLight[2].pos", pointLightPos[2]);
	sceneShader->setVec3("pLight[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	sceneShader->setVec3("pLight[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	sceneShader->setVec3("pLight[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	sceneShader->setFloat("pLight[2].c", 1.0f);
	sceneShader->setFloat("pLight[2].l", 0.09f);
	sceneShader->setFloat("pLight[2].q", 0.032f);

	// point light4
	sceneShader->setVec3("pLight[3].pos", pointLightPos[3]);
	sceneShader->setVec3("pLight[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	sceneShader->setVec3("pLight[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	sceneShader->setVec3("pLight[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	sceneShader->setFloat("pLight[3].c", 1.0f);
	sceneShader->setFloat("pLight[3].l", 0.09f);
	sceneShader->setFloat("pLight[3].q", 0.032f);

	// spotLight
	sceneShader->setVec3("sLight.pos", cam->getPosition());
	sceneShader->setVec3("sLight.dir", cam->getDirection());
	sceneShader->setVec3("sLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	sceneShader->setVec3("sLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	sceneShader->setVec3("sLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	sceneShader->setFloat("sLight.c", 1.0f);
	sceneShader->setFloat("sLight.l", 0.09f);
	sceneShader->setFloat("sLight.q", 0.032f);
	sceneShader->setFloat("sLight.cutOff", glm::cos(glm::radians(12.5f)));
	sceneShader->setFloat("sLight.outCutOff", glm::cos(glm::radians(15.0f)));


	for (int i = 0; i < 10; i++)
	{
		mMatrix = glm::mat4(1.0f);
		mMatrix = glm::translate(mMatrix, modelVecs[i]);
		mMatrix = glm::rotate(mMatrix, glm::radians(i*10.0f), glm::vec3(0.0, 1.0, 0.0));
		sceneShader->setMatrix("mMatrix", mMatrix);

		glBindVertexArray(VAO_cube);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// ���shader ����
	sceneShader->end();

	// ̫�����λ��

	sunShader->start();

	sunShader->setMatrix("vMatrix", cam->getVMatrix());
	sunShader->setMatrix("pMatrix", pMatrix);

	for (int i = 0; i < 4; ++i)
	{
		mMatrix = glm::mat4(1.0f);
		mMatrix = glm::translate(mMatrix, pointLightPos[i]);
		mMatrix = glm::scale(mMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
		sunShader->setMatrix("mMatrix", mMatrix);
		glBindVertexArray(VAO_sun);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}


	sunShader->end();
}
