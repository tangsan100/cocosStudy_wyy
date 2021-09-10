#pragma once
#include "Base.h"
class Shader
{
private:
	unsigned int shaderProgram;

public:
	Shader();
	~Shader(){}

	void initShader(const char* vertexPath, const char* fragPath);
	void readShaderFile(const char* file, std::string &code);
	unsigned int CompileShader(const char* shaderCode, unsigned int mode);
	void drawLink(unsigned int vertexID, unsigned int fragmentID);

	void shaderGreenTest();

	void start();
	void end();

	void setMatrix(const std::string name, glm::mat4 matrix);
	void setVec3(const std::string name, glm::vec3 v3);
	void setFloat(const std::string name, float f);
	void setInt(const std::string name, int i);
};

