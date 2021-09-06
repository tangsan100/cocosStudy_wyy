#pragma once
#include <string>
#include "Base.h"

class Shader;
class AImage;
class GLDraw
{
private:

	int width;
	int height;

	unsigned int VBO = 0;
	unsigned int VAO = 0;
	unsigned int textureID = 0;
	Shader* shader;
	AImage* img;

	// vp æÿ’Û–≈œ¢
	glm::mat4 vMatrix;
	glm::mat4 pMatrix;
public:
	void init();
	void initTexture();
	void initModel();
	
	void rander();
	GLDraw(int width,int height);
	~GLDraw();
	
};

