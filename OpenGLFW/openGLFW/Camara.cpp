#include "Camara.h"
#include <math.h>


Camara::Camara() {
	position	= glm::vec3(0, 0, 0);
	front		= glm::vec3(0, 0, 0);
	up			= glm::vec3(0, 0, 0);
	speed		= 0.08f;

	rPitch		= 0;
	rYaw		= -90.0f;
	xPos		= 0.0f;
	yPos		= 0.0f;
}


Camara::~Camara() {

}

void Camara::lookAt(glm::vec3 posEyes, glm::vec3 posFront, glm::vec3 posUp) {
	position = posEyes;
	front = posFront;// glm::normalize(posFront);
	up = posUp;

	vMatrix = glm::lookAt(position, position + front, up);
}


void Camara::update() {
	vMatrix = glm::lookAt(position, position + front, up);
}


void Camara::move(CAM_MOVE type) {
	switch (type)
	{
	case MOVE_LEFT:
		position -= glm::normalize(glm::cross(up, front))*speed;
		break;
	case MOVE_RIGHT:
		position += glm::normalize(glm::cross(up, front))*speed;
		break;
	case MOVE_FRONT:
		position += speed*glm::normalize(front);
		break;
	case MOVE_BACK:
		position -= speed * glm::normalize(front);
		break;
	default:
		break;
	}

	this->update();
}

void Camara::mouseMove(double x, double y) {
	if (xPos <= 0.000001 && yPos <= 0.000001)
	{
		xPos = x;
		yPos = y;

		return;
	}

	float xOffset = -(x - xPos);
	float yOffset = -(y - yPos);

	xPos = x;
	yPos = y;

	pitch(yOffset);
	yaw(xOffset);
	//this->update();
}

void Camara::pitch(float yOffset) {
	rPitch += yOffset * sensitivity;

	if (rPitch >= 89.0f)
	{
		rPitch = 89.0f;
	}

	if (rPitch <= -89.0f)
	{
		rPitch = -89.0f;
	}

	front.y = sin(glm::radians(rPitch));
	front.x = cos(glm::radians(rPitch))*cos(glm::radians(rYaw));
	front.z = cos(glm::radians(rPitch))*sin(glm::radians(rYaw));

	front = glm::normalize(front);

	update();
}


void Camara::yaw(float xOffset) {
	rYaw += xOffset * sensitivity;
	front.y = sin(glm::radians(rPitch));
	front.x = cos(glm::radians(rPitch))*cos(glm::radians(rYaw));
	front.z = cos(glm::radians(rPitch))*sin(glm::radians(rYaw));
	front = glm::normalize(front);
	update();
}


void Camara::setSensitivity(float sensitivity) {
	this->sensitivity = sensitivity;
}