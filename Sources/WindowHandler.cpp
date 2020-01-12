#include "../Headers/WindowHandler.h"
#include <exception>

WindowHandler::WindowHandler() {

}

WindowHandler::~WindowHandler() {
	glfwTerminate();
}

void WindowHandler::initialize(const char *windowName) noexcept(false) {
	// try initialising. if it succeeds, it returns GLFW_TRUE
	if (!glfwInit()) {
		throw std::exception("Glfw init failed.");
	}

	GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	// stop iconifying window when clicking away from the monitor
	// glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);

	// create the glfw window
	window = glfwCreateWindow(mode->width, mode->height, windowName, primaryMonitor, nullptr);
	glfwSetWindowPos(window, 0, 0);

	// window creation failed
	if (!window) {
		throw std::exception("Window init failed.");
	}

	// make the window context as current
	glfwMakeContextCurrent(window);

	// initialize glew
	if (glewInit() != GLEW_OK) {
		throw std::exception("Glew init failed.");
	}

	// set the viewport size
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	glViewport(0, 0, bufferWidth, bufferHeight);

	// set how many screen updates it waits for to swap intervals
	glfwSwapInterval(1);
}

WindowHandler& WindowHandler::setLoopCallback(void (*cb)(double)) {
	loopCallback = cb;

	return *this;
}

void WindowHandler::loop() {
	// exits while loop when an action to close window is caught 
	while (!glfwWindowShouldClose(window))
	{
		double timeNow = glfwGetTime();
		// pass in the deltaTime to the loop callback
		loopCallback(timeNow - lastTimeLooped);
		lastTimeLooped = timeNow;

		glfwSwapBuffers(window);

		// check for events that appeared in the meanwhile
		glfwPollEvents();
	}
}