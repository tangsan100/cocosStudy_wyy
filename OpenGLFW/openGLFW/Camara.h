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
	
	glm::vec3 position; // 摄像机位置
	glm::vec3 front;	// 摄像机看向的方向向量
	glm::vec3 up;		// 摄像机的仰角向量


	float speed;  // 摄像机的移动速度

	glm::mat4 vMatrix; // v 投影矩阵

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
	glm::vec3 getDirection() { return front; }
	void lookAt(glm::vec3 posEyes, glm::vec3 posFront, glm::vec3 posUp);
	void update();
	void move(CAM_MOVE type);
	void mouseMove(double x, double y);

	void pitch(float yOffset);
	void yaw(float xOffset);
	void setSensitivity(float sensitivity);
};

