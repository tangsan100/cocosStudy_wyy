
#include "Base.h"

#include "GLDraw.h"
#include "Camara.h"

GLDraw* draw = new GLDraw(800, 600);


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		draw->camMove(CAM_MOVE::MOVE_LEFT);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		draw->camMove(CAM_MOVE::MOVE_RIGHT);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		draw->camMove(CAM_MOVE::MOVE_FRONT);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		draw->camMove(CAM_MOVE::MOVE_BACK);
	}
}

void mouse_callback(GLFWwindow* window,double xpos,double ypos) {
	draw->mouseMove(xpos, ypos);
}


int main()
{

	// 上下文环境的初始化
	glfwInit();
	// 版本配置， 核心模式需要3.0以上
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建OpenGL 窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Core", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create OpenGL Win" << std::endl;
		// 退出
		glfwTerminate();
		return -1;
	}
	// 绑定当前窗口
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Faild to initialize GLAD" << std::endl;
		return -1;
	}

	// 设置openGL 视口的大小，和窗体一样大
	glViewport(0, 0, 800, 600);

	// 窗口发生变动的时候的回调
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	
	// 初始化顶点
	
	draw->init();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		

		// TODO rander
		draw->rander();

		// 相当于缓冲区，双缓存，提高渲染效率
		glfwSwapBuffers(window);
		// 事件分发
		glfwPollEvents();
	}

	glfwTerminate();
	delete draw;
	return 0;
}


