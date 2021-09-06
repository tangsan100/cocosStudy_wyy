#include "Shader.h"
#include <fstream>
#include <sstream>

void Shader::initShader(const char* vertexPath, const char* fragmentPath) {

	// ���ļ�
	std::string vShaderCode("");
	std::string fShaderCoder("");
	this->readShaderFile(vertexPath, vShaderCode);
	this->readShaderFile(fragmentPath, fShaderCoder);
	const char* _vShaderStr = vShaderCode.c_str(); //_vertexCode.c_str();
	const char* _fShaderStr = fShaderCoder.c_str(); //_fragCode.c_str();


	//����
	unsigned int _vertexID = this->CompileShader(_vShaderStr, GL_VERTEX_SHADER);

	// ����
	unsigned int  _fragID = this->CompileShader(_fShaderStr, GL_FRAGMENT_SHADER);


	// ����
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
	//�������
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	// ����
	glCompileShader(shaderID);
	//��ȡ����״̬
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
		// ���ļ�
		shaderFile.open(file);


		// �����ļ�������
		std::stringstream _vShaderStream, _fShaderStream;
		_vShaderStream << shaderFile.rdbuf();

		// ��ֵ��shaderCode 
		code = _vShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "read shader Failer" << std::endl;

	}

}

void Shader::shaderGreenTest() {
	// ��shader ��������
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
	// ��shader ��ֵ���Ѿ�����
	glUniformMatrix4fv(glGetUniformLocation(this->shaderProgram, name.c_str()),
		1, GL_FALSE, glm::value_ptr(matrix));
}
