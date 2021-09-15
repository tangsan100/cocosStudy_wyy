#include "Shader.h"
#include <fstream>
#include <sstream>


Shader::Shader() {
	shaderProgram = 0;
}

void Shader::initShader(const char* vertexPath, const char* fragmentPath) {

	// 打开文件
	std::string vShaderCode("");
	std::string fShaderCoder("");
	this->readShaderFile(vertexPath, vShaderCode);
	this->readShaderFile(fragmentPath, fShaderCoder);
	const char* _vShaderStr = vShaderCode.c_str(); //_vertexCode.c_str();
	const char* _fShaderStr = fShaderCoder.c_str(); //_fragCode.c_str();


	//编译
	unsigned int _vertexID = this->CompileShader(_vShaderStr, GL_VERTEX_SHADER);

	// 编译
	unsigned int  _fragID = this->CompileShader(_fShaderStr, GL_FRAGMENT_SHADER);


	// 链接
	this->drawLink(_vertexID, _fragID);
	glDeleteShader(_vertexID);
	glDeleteShader(_fragID);
}

void Shader::drawLink(unsigned int vertexID, unsigned int fragmentID) {

	char _infoLog[512];
	int _successFlag = 0;

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexID);
	glAttachShader(shaderProgram, fragmentID);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &_successFlag);
	if (!_successFlag)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, _infoLog);
		std::cout << _infoLog << std::endl;
	}

}

unsigned int Shader::CompileShader(const char* shaderCode, unsigned int mode) {
	unsigned shaderID = 0;
	char _infoLog[512];
	int _successFlag = 0;

	shaderID = glCreateShader(mode);
	//传入代码
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	// 编译
	glCompileShader(shaderID);
	//获取编译状态
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &_successFlag);
	if (!_successFlag)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, _infoLog);
		std::cout << _infoLog << std::endl;
		return -1;
	}

	return shaderID;
}

void Shader::readShaderFile(const char* file, std::string &code) {


	std::ifstream shaderFile;


	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


	try
	{
		// 打开文件
		shaderFile.open(file);


		// 读入文件的内容
		std::stringstream _vShaderStream, _fShaderStream;
		_vShaderStream << shaderFile.rdbuf();

		// 赋值给shaderCode 
		code = _vShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "read shader Failer" << std::endl;

	}

}

void Shader::shaderGreenTest() {
	// 往shader 传入数据
	float tm = glfwGetTime();
	float g = sin(tm)*0.5f + 0.5f;
	int location = glGetUniformLocation(shaderProgram, "ourColor");
	glUniform4f(location, 0.0f, g, 0.0f, 1.0f);
}

void Shader::start() {
	glUseProgram(shaderProgram);
}

void Shader::end() {
	glUseProgram(0);
}

void Shader::setMatrix(const std::string name, glm::mat4 matrix) {
	// 往shader 传值，把矩阵传入
	glUniformMatrix4fv(glGetUniformLocation(this->shaderProgram, name.c_str()),
		1, GL_FALSE, glm::value_ptr(matrix));
}


void Shader::setVec3(const std::string name, glm::vec3 v3) {
	glUniform3fv(glGetUniformLocation(this->shaderProgram, name.c_str()), 1, glm::value_ptr(v3));
}
void Shader::setFloat(const std::string name, float f) {
	glUniform1f(glGetUniformLocation(this->shaderProgram, name.c_str()), f);
}

void Shader::setInt(const std::string name, int i) {
	glUniform1i(glGetUniformLocation(this->shaderProgram, name.c_str()), i);
}

void Shader::getVal(const std::string name) {
	//glm::vec3 v = glGetUniformLocation(this->shaderProgram, name.c_str());
}