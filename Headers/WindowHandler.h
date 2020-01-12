#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class WindowHandler {
public:
	WindowHandler();
	// destroys context and closes the opened window
	~WindowHandler();

	// call this function to initialize glfw, glew and open a window, making it the current context
	void initialize(const char *windowName) noexcept(false) ;
	WindowHandler& setLoopCallback(void (*)(double));

	GLFWwindow* getWindow() {
		return window;
	}

	void loop();
private:
	GLFWwindow* window = nullptr;
	void (*loopCallback)(double) = nullptr;

	double lastTimeLooped = 0;
};