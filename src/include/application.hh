#pragma once
#include <GLFW/glfw3.h>


class Application {
public:
	void run();
private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();

private:
	GLFWwindow* m_window;
};