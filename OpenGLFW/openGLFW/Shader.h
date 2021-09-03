#pragma once
#include "Base.h"
class Shader
{
private:
	unsigned int shaderProgram;

public:
	Shader() {
		shaderProgram = 0;
	}
	~Shader(){}

	void initShader(const char* vertexPath, const char* fragPath);
	void readShaderFile(const char* file, std::string &code);
	unsigned int CompileShader(const char* shaderCode, unsigned int mode);
	void drawLink(unsigned int vertexID, unsigned int fragmentID);

	void shaderGreenTest();

	void start();
	void end();
};

