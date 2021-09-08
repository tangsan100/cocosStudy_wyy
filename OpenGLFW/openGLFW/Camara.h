#pragma once
#include "Base.h"

enum CAM_MOVE
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_FRONT,
	MOVE_BACK
};

class Camara
{
private:
	
	glm::vec3 position; // �����λ��
	glm::vec3 front;	// ���������ķ�������
	glm::vec3 up;		// ���������������


	float speed;  // ��������ƶ��ٶ�

	glm::mat4 vMatrix; // v ͶӰ����

	float rPitch;
	float rYaw;
	float sensitivity;
	double xPos;
	double yPos;

public:
	Camara();
	~Camara();

	glm::mat4 getVMatrix() { return vMatrix; }
	glm::vec3 getPosition() { return position; }
	void lookAt(glm::vec3 posEyes, glm::vec3 posFront, glm::vec3 posUp);
	void update();
	void move(CAM_MOVE type);
	void mouseMove(double x, double y);

	void pitch(float yOffset);
	void yaw(float xOffset);
	void setSensitivity(float sensitivity);
};

