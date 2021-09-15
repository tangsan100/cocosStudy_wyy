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
	// ��ʼ�����
	glm::vec3 eyes(0.0f, 0.0f, 3.0f);	// �۲�λ��
	glm::vec3 front(0.0f, 0.0f, 1.0f);	// ǰ��λ��
	glm::vec3 popUp(0.0f, 1.0f, 0.0f);  // �������
	cam->lookAt(eyes, front, popUp);
	cam->setSensitivity(0.1f);			// ���������������

	// ����������ͼ
	textureBox = createTexture("res/box.png");
	textureSpec = createTexture("res/specular.png");

	// VAO ��ʼ��
	VAO_cube = createModel();

	// ��ʼ��shader
	shaderColor->initShader("shader/colorV.glsl", "shader/colorF.glsl");
	shaderCube->initShader("shader/cubeV.glsl", "shader/cubeF.glsl");
}

uint DrawTest::createTexture(const char* fileName) {
	AImage* img = AImage::loadImage(fileName);


	uint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// �ߴ糬����ʱ�򣬲���ƽ�̵�ģʽ�� s ˮƽ�� T ��ֱ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ��С��ʱ��ȡ�ٽ����أ� �Ŵ��ʱ��ȡ���Բ�ֵ
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

	
	// VAO ��core ģʽ���Ǳ���ģ��ڷ�core ģʽ�¿��Բ���VAO �ӹ�
	// ʹ��VAO ��Ŀ���ǣ�ÿ��VAO ���ܹ���һ��VBO�������ʹ��VAO
	// ��ô �����VBO ���ݻḲ�ǵ�ǰ���VBO������
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // ����
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3)); //UV
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 5));	// ����

	// 5. ����ê��
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// ���VBO��VA0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

// ��������
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

	// ����Ҫ����������ɫ
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	


	// �߹����
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

	// �߹�Ч��
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
