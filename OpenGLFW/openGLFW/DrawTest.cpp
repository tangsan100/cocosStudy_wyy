#include "DrawTest.h"
#include "Shader.h"
#include "AImage.h"
#include "gData.h"
#include <map>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

const static int AMOUNT = 10000;
// �����ͼ�ķֱ���
const uint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

DrawTest::DrawTest(int w,int h):width(w),height(h){
	cam					= new Camara();
	shaderColor			= new Shader();
	shaderCube			= new Shader();
	shaderBlending		= new Shader();
	screenShader		= new Shader();
	skyShader			= new Shader();
	shaderRed			= new Shader();
	shaderGreen			= new Shader();
	shaderGeo			= new Shader();
	shaderBox			= new Shader();
	planetShader		= new Shader();
	rockShader			= new Shader();
	blinnPhone			= new Shader();
	shaderShadow		= new Shader();
	dirShader			= new Shader();
	shaderPsDepth		= new Shader();
	shaderPointShadow	= new Shader();

	matrixArr = new glm::mat4[AMOUNT];
}


DrawTest::~DrawTest() {
	
	delete cam;
	delete shaderCube;
	delete shaderColor;
	delete shaderBlending;
	delete screenShader;
	delete skyShader;
	delete shaderRed;
	delete shaderGreen;
	delete shaderBox;
	delete planetShader;
	delete rockShader;
	delete[]matrixArr;
	delete shaderShadow;
	delete dirShader;
	delete shaderPsDepth;
	delete shaderPointShadow;
}

void DrawTest::init() {
	// ��ʼ�����

	// �����ʼ��
	glm::vec3 eyes(0.0f, 0.0f, 3.0f);
	glm::vec3 front(0.0f, 0.0f, 1.0f);
	glm::vec3 popUp(0.0f, 1.0f, 0.0f);
	

	//glm::vec3 eyes(0.0f, 0.0f, 155.0f);	// �۲�λ��
	//glm::vec3 front(0.0f, 0.0f, 1.0f);	// ǰ��λ��
	//glm::vec3 popUp(0.0f, 1.0f, 0.0f);  // �������
	cam->lookAt(eyes, front, popUp);
	cam->setSensitivity(0.05f);			// ���������������


	// ����������ͼ
	textureBox		= createTexture("res/box.png");
	textureSpec		= createTexture("res/specular.png");
	textureWin		= createTexture("res/window.png");
	texturePlane	= createTexture("res/plane.jpg");
	textureSkybox	= createSkyTexture();

	// VAO ��ʼ��
	VAO_cube		= createModel();
	VAO_plane		= createPlane();
	VAO_win			= createWindow();
	frameBuffer		= createFramBuffer();
	VAO_texture		= createScreenPlane();
	VAO_skybox		= createSkyBox();
	VAO_house		= createHouse();
	VAO_box			= createModel();
	depthFbo		= createShadowFbo();
	depthCubeMapFbo = createPointShadowFbo();

	// UBO
	//UBO_red = createUBO();

	/*planet = new FF::ffModel("res/planet/planet.obj");
	rock = new FF::ffModel("res/rock/rock.obj");*/
	
	// ��ʼ��shader
	//shaderCube->initShader("shader/cubeV.glsl", "shader/cubeF.glsl");
	shaderCube->initShader("shader/cc/cubeV.glsl", "shader/cc/cubeF.glsl");
	shaderBox->initShader("shader/boxV.glsl", "shader/boxF.glsl");
	shaderColor->initShader("shader/colorV.glsl", "shader/colorF.glsl");
	shaderBlending->initShader("shader/BlendingV.glsl", "shader/BlendingF.glsl");
	screenShader->initShader("shader/frameBufferV.glsl", "shader/frameBufferF.glsl");
	skyShader->initShader("shader/skyboxV.glsl", "shader/skyboxF.glsl");
	shaderRed->initShader("shader/c4/uboShaderV.glsl", "shader/c4/uboShaderF.glsl");
	shaderGreen->initShader("shader/c4/uboShaderV.glsl", "shader/c4/uboShaderF.glsl");
	shaderGeo->initShader("shader/c4/geoShaderV.glsl", "shader/c4/geoShaderF.glsl", "shader/c4/geoShaderG.glsl");
	planetShader->initShader("shader/c5/planetV.glsl", "shader/c5/planetF.glsl");
	rockShader->initShader("shader/c5/rockV.glsl", "shader/c5/rockF.glsl");
	blinnPhone->initShader("shader/c6/Blinn-PhongV.glsl", "shader/c6/Blinn-PhongF.glsl");
	shaderShadow->initShader("shader/c6/shadowMapV.glsl","shader/c6/shadowMapF.glsl");
	dirShader->initShader("shader/c6/dirLightV.glsl","shader/c6/dirLightF.glsl");
	shaderPsDepth->initShader("shader/c6/shadowDepthV.glsl", "shader/c6/shadowDepthF.glsl", "shader/c6/shadowDepthG.glsl");
	shaderPointShadow->initShader("shader/c6/pointShadowV.glsl", "shader/c6/pointShadowF.glsl");

	/*bindShaderData();
	initInstanceArray();*/

}

void DrawTest::initInstanceArray() {
	
	srand(glfwGetTime());

	float radius = 100.0f;
	float offset = 15.0f;
	

	for (uint i = 0; i < AMOUNT; i++)
	{
		glm::mat4 model(1.0f);
		float angle = float(i) / float(AMOUNT) * 360.0f;

		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle)*radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle)*radius + displacement;

		model = glm::translate(model, glm::vec3(x, y, z));

		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		float rotAngle = (rand()%360);
		model = glm::rotate(model, glm::radians(rotAngle), glm::vec3(0.0f, 1.0f, 0.0f));

		matrixArr[i] = model;
	}

	uint VBO_ins = 0;
	glGenBuffers(1, &VBO_ins);
	glBindBuffer(GL_ARRAY_BUFFER,VBO_ins);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * AMOUNT, &matrixArr[0], GL_STATIC_DRAW);

	for (uint i = 0; i < rock->meshVec.size(); ++ i)
	{
		glBindVertexArray(rock->meshVec[i].VAO);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2*sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3*sizeof(glm::vec4)));


		
		// ����1: ��location ��� index ָ��Ϊinstance ����
		// ����2�� ÿ��1��instance�� vertex shader ��ע��buffer һ������ֵ
		glVertexAttribDivisor(3, 1); // 3������Ϊinstance��ÿ��1��ʵ��
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
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

	delete img;
	return textureID;
}

// ��պ��ӵ�����
uint DrawTest::createSkyTexture() {
	uint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


	std::vector<std::string> faces
	{
		"res/skybox/right.jpg",
		"res/skybox/left.jpg",
		"res/skybox/top.jpg",
		"res/skybox/bottom.jpg",
		"res/skybox/front.jpg",
		"res/skybox/back.jpg"
	};


	for (uint i = 0 ;i < faces.size(); ++i)
	{
		AImage* pImg = AImage::loadImage(faces[i].c_str(),false);
		//unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGBA, pImg->getWidth(), pImg->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pImg->getData()
		);
		
		delete pImg;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

uint DrawTest::createModel() {


	uint VAO;
	uint VBO;

	
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(gCubeVertices), gCubeVertices, GL_STATIC_DRAW);

	// 4. ����shader���ݽ�����ʽ
	// ����1�� ���ĸ�λ�ÿ�ʼ����ê�㣬layout ��λ����Ϣ
	// ����2��ÿ�ζ�ȡ���ٸ�
	// ����3����������
	// ����4���Ƿ��1��
	// ����5������
	// ����6����ʼ��ַ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3)); //����
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));	// UV

	// 5. ����ê��
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// ���VBO��VA0
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


	glBufferData(GL_ARRAY_BUFFER, sizeof(gPlaneVextices1), gPlaneVextices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // ����
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // ����
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // ����

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
	// ����ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,nullptr);
	// ��ֵ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ��texture ��Ϊcolor �������󶨵�framebuffer ���棬  �õ��ǵ�0��colorbuffer ����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);
	

	// D/S buffer
	uint RBO = 0; // render buffer obj
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	// 24 λ�������Ȼ��棬8λ�����stencil
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// ���
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // ����
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // ����

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	return VAO;


}

uint DrawTest::createSkyBox() {
	uint VAO;
	uint VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(gSkyboxVertices), gSkyboxVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	
	/*glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/
	return VAO;

}

uint DrawTest::createUBO() {
	uint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER,ubo);

	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return ubo;
}

uint DrawTest::createHouse() {

	float points[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};

	uint VAO, VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 2)); //��ɫ

	// 5. ����ê��
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// ���VBO��VA0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

uint DrawTest::createShadowFbo() {
	
	uint depthMapFBO = 0;
	// ����framebuffers
	glGenFramebuffers(1, &depthMapFBO);

	// texture2D ����Ȼ���
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	// ע����� gl_depth_component ������
	//��Ϊ����ֻ�������ֵ������Ҫ�������ʽָ��ΪGL_DEPTH_COMPONENT
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// ������������� GL_CLAMP_TO_BORDER �����˼��÷�Χ�����Ϊ1�� ������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/

	// ��֡����
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	// ֻ��Ϊ��Ȼ���
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	// ��������
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return depthMapFBO;
}

uint DrawTest::createPointShadowFbo() {


	uint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);


	// ����cubeMap ֡���棬��Ȼ���
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);


	for (uint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// ��cubeMap �󶨵�fbo
	
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return depthMapFBO;

}


// ��������
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

void DrawTest::bindShaderData() {
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO_red);

	uint red = glGetUniformBlockIndex(shaderRed->getProgram(), "MAT");
	glUniformBlockBinding(shaderRed->getProgram(), red, 0);

	uint green = glGetUniformBlockIndex(shaderGreen->getProgram(), "MAT");
	glUniformBlockBinding(shaderGreen->getProgram(), green, 0);
	
}


void DrawTest::render() {
	cam->update();

	
	//glCullFace(GL_FRONT);
	//glEnable(GL_STENCIL_TEST);

	// ����Ҫ����������ɫ
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// �߹����
	//testHightLight();
	// ͸������
	//testBlending();
	// cullface
	//testCullFace();
	// ֡����
	//testFrameBuffer();
	// ��պ�
	//testSkybox();
	//UBO
	//testUboData();
	//GEO
	//testGeo();
	// instance
	//testInstance();
	// ����blinn-phong ģ��
	//testBlinPhong();

	// shadowMap
	//testshadowMap();
	// pointShadow
	testPointShadowMap();

	//testCube();
	
}

void DrawTest::testCube() {
	// mvp ����任��Ϣ
	glm::mat4 vMatrix = cam->getVMatrix(); //glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	//glBindTexture(GL_TEXTURE_2D, textureBox);
	this->bindTexture();

	static int i = 0;

		glm::mat4 mMatrix(1.0f);
		mMatrix = glm::mat4(1.0f);
		mMatrix = glm::rotate(mMatrix, glm::radians(float(glfwGetTime())*(i + 1) * 5), glm::vec3(0.0f, 1.0f, 0.0f));

		// ʹ��shader ����
		shaderCube->start();

		// ����vp ��Ϣ
		shaderCube->setMatrix("mMatrix", mMatrix);
		shaderCube->setMatrix("vMatrix", vMatrix);
		shaderCube->setMatrix("pMatrix", pMatrix);

		// ��VAO ������
		glBindVertexArray(VAO_cube);


		// ����OpenGL Ҫ��һ�������Σ� �ӵ�0�����㿪ʼ����Ч������3��
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// ���shader ����
		shaderCube->end();

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


void DrawTest::testBlending() {
	
	// ������ɫ���
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

	// �����򣬸�����������ľ���������
	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < gWinPos.size(); i++)
	{
		float distance = glm::length(cam->getPosition() - gWinPos[i]);
		sorted[distance] = gWinPos[i];
	}

	// �Ƚ�Զ���Ȼ���
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
	glCullFace(GL_FRONT); // ��������
	glFrontFace(GL_CW); // ˳ʱ�뷽��Ϊ��

	
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

//֡�������
void DrawTest::testFrameBuffer() {

	// ����framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ������ɫ���
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

	// �����򣬸�����������ľ���������
	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < gWinPos.size(); i++)
	{
		float distance = glm::length(cam->getPosition() - gWinPos[i]);
		sorted[distance] = gWinPos[i];
	}

	// �Ƚ�Զ���Ȼ���
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

void DrawTest::testSkybox() {
	
	// mvp
	glm::mat4 mMatrix(1.0f);
	mMatrix = glm::translate(mMatrix, glm::vec3(0.0f, -0.4f, 0.0f));

	glm::mat4 vMatrix = cam->getVMatrix();
	glm::mat4 pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	

	//////////////////С������////////////////
	//bindTexture(textureBox);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkybox);
	shaderCube->start();
	

	shaderCube->setMatrix("mMatrix", mMatrix);
	shaderCube->setMatrix("vMatrix", vMatrix);
	shaderCube->setMatrix("pMatrix", pMatrix);
	shaderCube->setVec3("viewPos",cam->getPosition());

	glBindVertexArray(VAO_cube);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shaderCube->end();

	// ��պ�
	glDepthFunc(GL_LEQUAL);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkybox);
	skyShader->start();
	glm::mat4 view = glm::mat4(glm::mat3(vMatrix));
	skyShader->setMatrix("vMatrix", view);
	skyShader->setMatrix("pMatrix", pMatrix);
	glBindVertexArray(VAO_skybox);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	skyShader->end();

	glDepthFunc(GL_LESS);
}


void DrawTest::testUboData() {

	// mvp
	glm::mat4 mMatrix(1.0f);
	mMatrix = glm::translate(mMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 vMatrix = cam->getVMatrix();
	glm::mat4 pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	glBindBuffer(GL_UNIFORM_BUFFER, UBO_red);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(pMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(vMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	shaderRed->start();
	shaderRed->setVec3("color", glm::vec3(1.0f, 0.0, 0.0));
	shaderRed->setMatrix("mMatrix", mMatrix);
	glBindVertexArray(VAO_cube);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shaderRed->end();

	shaderGreen->start();
	shaderRed->setVec3("color", glm::vec3(0.0f, 1.0, 0.0));
	mMatrix = glm::mat4(1.0f);
	mMatrix = glm::translate(mMatrix, glm::vec3(1.0, 0.0f, -3.0f));
	shaderRed->setMatrix("mMatrix", mMatrix);
	glBindVertexArray(VAO_cube);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shaderRed->end();
}


void DrawTest::testGeo() {

	bindTexture();
	
	// mvp
	glm::mat4 mMatrix(1.0f);
	mMatrix = glm::translate(mMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 vMatrix = cam->getVMatrix();
	glm::mat4 pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 1.0f, 100.0f);

	

	shaderBox->start();

	shaderBox->setMatrix("mMatrix", mMatrix);
	shaderBox->setMatrix("vMatrix", vMatrix);
	shaderBox->setMatrix("pMatrix", pMatrix);

	glBindVertexArray(VAO_box);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shaderBox->end();


	shaderGeo->start();
	shaderGeo->setMatrix("mMatrix", mMatrix);
	shaderGeo->setMatrix("vMatrix", vMatrix);
	shaderGeo->setMatrix("pMatrix", pMatrix);
	glBindVertexArray(VAO_box);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shaderGeo->end();


	/*shaderGeo->start();
	glBindVertexArray(VAO_house);
	glDrawArrays(GL_POINTS, 0, 4);
	shaderGeo->end();*/
}

void DrawTest::testInstance() {
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
	glm::mat4 view = cam->getVMatrix();
	

	planetShader->start();
	planetShader->setMatrix("projection", projection);
	planetShader->setMatrix("view", view);

	// draw planet
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	planetShader->setMatrix("model", model);
	planet->draw(planetShader);

	// draw rock
	rockShader->start();
	rockShader->setMatrix("projection", projection);
	rockShader->setMatrix("view", view);
	rockShader->setInt("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rock->meshVec[0].texVec[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
	for (unsigned int i = 0; i < rock->meshVec.size(); i++)
	{
		glBindVertexArray(rock->meshVec[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, rock->meshVec[i].indexVec.size(), GL_UNSIGNED_INT, 0, AMOUNT);
		glBindVertexArray(0);
	}
}

void DrawTest::testBlinPhong() {

	// ̫�����ǿ��
	glm::vec3 sunLight(1.0f, 1.0f, 1.0f);

	glm::mat4 mMatrix(1.0f);
	mMatrix = glm::translate(mMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 vMatrix = cam->getVMatrix();
	glm::mat4 pMatrix = glm::perspective(glm::radians(60.0f), float(width) / height, 0.1f, 100.0f);

	blinnPhone->start();
	blinnPhone->setMatrix("mMatrix", mMatrix);
	blinnPhone->setMatrix("vMatrix", vMatrix);
	blinnPhone->setMatrix("pMatrix", pMatrix);

	// ��������
	blinnPhone->setInt("material.diffuse", 0);
	blinnPhone->setInt("material.specular", 0);
	blinnPhone->setInt("material.shiness", 32);

	// blin flag
	blinnPhone->setInt("blinn", true);
	
	// �����������
	blinnPhone->setVec3("light.position", glm::vec3(0.0, 0.0, 0.0));
	blinnPhone->setVec3("light.dir", cam->getPosition());
	blinnPhone->setFloat("light.cutOff", cos(glm::radians(12.5f)));
	blinnPhone->setFloat("light.outCutOff", cos(glm::radians(20.5f)));
	blinnPhone->setVec3("light.ambient", sunLight);
	blinnPhone->setVec3("light.diffuse", sunLight*glm::vec3(0.7f));
	blinnPhone->setVec3("light.specular", sunLight*glm::vec3(0.5f));
	blinnPhone->setFloat("light.c", 1.0f);
	blinnPhone->setFloat("light.l", 0.07f);
	blinnPhone->setFloat("light.q", 0.017f);
	blinnPhone->setVec3("viewPos", cam->getPosition());

	this->bindTexture(texturePlane);
	glBindVertexArray(VAO_plane);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	blinnPhone->end();
}

void DrawTest::testshadowMap() {
	
	//glEnable(GL_CULL_FACE);
	// pass 1
	// create light MVP
	float near_plane = 1.0, far_plane = 7.5f;
	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
	glm::mat4 lightViewMat = glm::lookAt(lightPos,glm::vec3(0.0f),glm::vec3(0.0f,1.0f,0.0f));
	// ����ͶӰ�������ҡ��¡���, �Լ�����Զƽ����ɵİ�Χ��
	glm::mat4 orthoMat = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	glBindFramebuffer(GL_FRAMEBUFFER, depthFbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	shaderShadow->start();
	shaderShadow->setMatrix("vMatrix", lightViewMat);
	shaderShadow->setMatrix("pMatrix", orthoMat);
	
	//glCullFace(GL_FRONT);
	renderScene(shaderShadow);
	shaderShadow->end();
	//glCullFace(GL_BACK);

	// pass2  ��Ⱦ������
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 pMatrix = glm::perspective(glm::radians(45.0f), float(width) / height, 0.1f, 100.0f);
	dirShader->start();
	dirShader->setMatrix("pMatrix", pMatrix);
	dirShader->setMatrix("vMatrix", cam->getVMatrix());
	dirShader->setFloat("shiness",64.0f);
	dirShader->setVec3("lightPos", lightPos);
	dirShader->setVec3("viewPos", cam->getPosition());

	// ��������
	dirShader->setVec3("myLight.ambient", glm::vec3(0.2f));
	dirShader->setVec3("myLight.diffuse", glm::vec3(0.8f));
	dirShader->setVec3("myLight.specular", glm::vec3(0.9f));

	// �Թ�Դ��Ϊ�������ͶӰ���������任������������Ӱ
	dirShader->setMatrix("lightSpaceMat", orthoMat*lightViewMat);
	dirShader->setInt("shadowMap", 1);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	renderScene(dirShader);
	dirShader->end();
	
}

void DrawTest::renderScene(Shader *shader)
{
	// floor
	this->bindTexture(texturePlane);
	glm::mat4 model = glm::mat4(1.0f);
	shader->setMatrix("mMatrix", model);
	glBindVertexArray(VAO_plane);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	glm::vec3 translates[3] = {
		glm::vec3(0.0f,1.5f,0.0),
		glm::vec3(2.0f, 0.0f, 1.0),
		glm::vec3(-1.0f, 0.0f, 2.0)
	};
	glm::vec3 scales[3] = {
		glm::vec3(0.5f),
		glm::vec3(0.5f),
		glm::vec3(0.25f),
	};

	this->bindTexture(textureBox);


	for (uint i = 0; i < 3; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, translates[i]);
		if (i == 2)
		{
			model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		}
		
		model = glm::scale(model, scales[i]);
		shader->setMatrix("mMatrix", model);

		// ����������
		// render Cube
		this->renderBox();
	}
}

void DrawTest::testPointShadowMap() {

	glEnable(GL_CULL_FACE);

	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
	//����6����׵��
	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	GLfloat near = 1.0f;
	GLfloat far = 25.0f;
	glm::mat4 shadowProj = glm::perspective(90.0f, aspect, near, far);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));


	// 1 ����cubemap ��� 
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	{
		// ����Ļ���
		shaderPsDepth->start();
		for (GLuint i = 0; i < 6; ++i) {
			shaderPsDepth->setMatrix(("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);
		}
		shaderPsDepth->setFloat("far_plane", far);
		shaderPsDepth->setVec3("lightPos", lightPos);
		
		renderPointShadowScene(shaderPsDepth);
		shaderPsDepth->end();

	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	// 2. �����ĳ�������
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

	shaderPointShadow->start();
	glm::mat4 projection = glm::perspective(glm::radians(20.0f), (float)width / (float)height, 0.1f, 100.0f);
	glm::mat4 view = cam->getVMatrix();
	shaderPointShadow->setMatrix("pMatrix", projection);
	shaderPointShadow->setMatrix("vMatrix", view);

	// Set light uniforms
	shaderPointShadow->setVec3("lightPos", lightPos);
	shaderPointShadow->setVec3("viewPos", cam->getPosition());

	// Enable/Disable shadows by pressing 'SPACE'
	shaderPointShadow->setInt("shadows", true);
	shaderPointShadow->setFloat("far_plane", far);
	shaderPointShadow->setInt("depthMap", 1);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBox);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	renderPointShadowScene(shaderPointShadow);

	shaderPointShadow->end();
	
}

void DrawTest::renderPointShadowScene(Shader* shader) {
	// Room cube
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(30.0));
	shader->setMatrix("mMatrix", model);
	glDisable(GL_CULL_FACE);
	shader->setInt("reverse_normals", 1); // ���߷�ת������Ǵ����Ҫ��ת��������������Ⱦ
	this->renderBox();
	shader->setInt("reverse_normals", 0); // 
	glEnable(GL_CULL_FACE);


	// Cubes 1
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
	shader->setMatrix("mMatrix", model);
	this->renderBox();

	// Cubes 2
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
	model = glm::scale(model, glm::vec3(1.5));
	shader->setMatrix("mMatrix", model);
	this->renderBox();

	// Cubes 3
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
	shader->setMatrix("mMatrix", model);
	this->renderBox();

	// cubes 4
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
	shader->setMatrix("mMatrix", model);
	this->renderBox();

	// cubes 5
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
	model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(1.5));
	shader->setMatrix("mMatrix", model);
	this->renderBox();
}

void DrawTest::renderBox() {
	glBindVertexArray(VAO_box);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
