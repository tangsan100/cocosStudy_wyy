#include "Shader.h"
#include <fstream>
#include <sstream>


Shader::Shader() {
	shaderProgram = 0;
}

void Shader::initShader(const char* vertexPath, const char* fragmentPath, const char* geoPath) {

	// ���ļ�
	std::string vShaderCode("");
	std::string fShaderCoder("");
	std::string gShaderCode("");
	this->readShaderFile(vertexPath, vShaderCode);
	this->readShaderFile(fragmentPath, fShaderCoder);
	this->readShaderFile(geoPath, gShaderCode);
	//const char* _vShaderStr = vShaderCode.c_str(); //_vertexCode.c_str();
	//const char* _fShaderStr = fShaderCoder.c_str(); //_fragCode.c_str();
	//const char* _gShaderStr = gShaderCode.c_str(); //geoCode.c_str();


	//����
	uint _vertexID = this->CompileShader(vShaderCode, GL_VERTEX_SHADER);

	// ����
	uint  _fragID = this->CompileShader(fShaderCoder, GL_FRAGMENT_SHADER);

	// g
	uint  _geoID = this->CompileShader(gShaderCode, GL_GEOMETRY_SHADER);


	// ����
	this->drawLink(_vertexID, _fragID, _geoID);
	glDeleteShader(_vertexID);
	glDeleteShader(_fragID);
}

void Shader::drawLink(unsigned int vertexID, unsigned int fragmentID,unsigned int geoID) {

	char _infoLog[512];
	int _successFlag = 0;

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexID);
	if (geoID != 0)
	{
		glAttachShader(shaderProgram, geoID);
	}

	glAttachShader(shaderProgram, fragmentID);

	

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &_successFlag);
	if (!_successFlag)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, _infoLog);
		std::cout << _infoLog << std::endl;
	}

}

 uint Shader::CompileShader(std::string shaderCode, unsigned int mode) {

	if (shaderCode.empty())
	{
		return 0;
	}
	unsigned shaderID = 0;
	char _infoLog[512];
	int _successFlag = 0;
	const char* code = shaderCode.c_str();

	shaderID = glCreateShader(mode);
	//�������
	glShaderSource(shaderID, 1, &code, NULL);
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

	if (file == nullptr)
	{
		return;
	}

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