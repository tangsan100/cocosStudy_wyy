#pragma once
#include <string>

class Shader;
class GLDraw
{
private:
	unsigned int VBO = 0;
	unsigned int VAO = 0;
	Shader* shader;
public:
	void init();
	void initModel();
	
	void rander();
	GLDraw();
	~GLDraw() {
		delete shader;
	}
	
};

