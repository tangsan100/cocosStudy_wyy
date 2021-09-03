
#include "Base.h"

#include "GLDraw.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
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

	// 初始化顶点
	GLDraw* draw = new GLDraw;
	draw->init();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// 设置要清理画布的颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// 清理画布
		glClear(GL_COLOR_BUFFER_BIT);

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


