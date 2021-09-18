#pragma once
#include "Base.h"
#include <vector>

std::vector<glm::vec3> gWinPos{
	glm::vec3(1.5f, 0.0f, -0.48f),
	glm::vec3(1.5f,0.0f,0.51f),
	glm::vec3(0.0f,0.0f,0.7f),
	glm::vec3(-0.3f,0.0f,-2.3f),
	glm::vec3(0.5f,0.0f,-0.6f)
};

float gCubeVertices[] = {
	// points				// normal			//textCoods
	// Back face
	-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // Bottom-left
	 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // top-right
	 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // bottom-right         
	 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // top-right
	-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // bottom-left
	-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,  0.0f, 1.0f, // top-left
	// Front face
	-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,	 0.0f, 0.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,	 1.0f, 0.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,	 1.0f, 1.0f, // top-right
	 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,	 1.0f, 1.0f, // top-right
	-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,	 0.0f, 1.0f, // top-left
	-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,	 0.0f, 0.0f, // bottom-left
	// Left face
	-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-right
	-0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // top-left
	-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // bottom-right
	-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-right
	// Right face
	 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-left
	 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // top-right         
	 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-left
	 0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // bottom-left     
	// Bottom face
	-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // top-right
	 0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,  1.0f, 1.0f, // top-left
	 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // bottom-left
	-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,  0.0f, 0.0f, // bottom-right
	-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // top-right
	// Top face
	-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // top-left
	 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,  1.0f, 1.0f, // top-right     
	 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // bottom-right
	-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // top-left
	-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,  0.0f, 0.0f  // bottom-left        
};

float gPlaneVextices[] = {
		 5.0f,	-0.5f,	 5.0f,	0.0f,  1.0f,  0.0f, 2.0f, 0.0f,
		-5.0f,	-0.5f,	 5.0f,	0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-5.0f,	-0.5f,	-5.0f,	0.0f,  1.0f,  0.0f, 0.0f, 2.0f,

		 5.0f,	-0.5f,	 5.0f,	0.0f,  1.0f,  0.0f, 2.0f, 0.0f,
		-5.0f,	-0.5f,	-5.0f,	0.0f,  1.0f,  0.0f, 0.0f, 2.0f,
		 5.0f,	-0.5f,	-5.0f,	0.0f,  1.0f,  0.0f, 2.0f, 2.0f,
};

float gWinVextices[] = {
		0.0f,	 0.5f,	0.0f,	0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
		0.0f,	-0.5f,	0.0f,	0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
		1.0f,	-0.5f,	0.0f,	0.0f,  1.0f,  0.0f,	1.0f, 1.0f,

		0.0f,	 0.5f,	0.0f,	0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
		1.0f,	-0.5f,	0.0f,	0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
		1.0f,	 0.5f,	0.0f,	0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
};

// 天空盒子顶点数据
float gSkyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};