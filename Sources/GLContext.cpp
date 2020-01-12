#include "../Headers/GLContext.h"
#include "../glm/gtx/transform.hpp"

GLContext* GLContext::instance = nullptr;

GLContext& GLContext::createContext(const GLchar* windowName, std::map<std::string, GLenum> shadersDictionary) noexcept(false) {
	if (!instance) {
		instance = new GLContext(windowName, shadersDictionary);
	}
	else {
		throw std::exception("Trying to reinitialise context already initialised. Call getContext() instead.");
	}

	return *instance;
}

GLContext& GLContext::getContext() noexcept(false) {
	if (!instance) {
		throw std::exception("getContext() called without context being created first. Call createContext(windowName, shaders)");
	}

	return *instance;
}

GLContext::GLContext(const GLchar* windowName, std::map<std::string, GLenum> shadersDictionary) noexcept(false) {
	try {
		winHandle.initialize(windowName);

		progrLoader = new ProgramLoader();
		programId = progrLoader->compileProgram(shadersDictionary).use().getProgramId();
	}
	catch (std::exception & e) {
		throw e;
	}

	glfwSetFramebufferSizeCallback(winHandle.getWindow(), GLContext::handleFramebufferResize);
	setFramebufferSize();
}

// handling resize, updating the size of the screen
void GLContext::setFramebufferSize(int width, int height) {
	if (width == 0 || height == 0) {
		glfwGetFramebufferSize(winHandle.getWindow(), &width, &height);

		// the window has been minimised
		if (width == 0 || height == 0) {
			return;
		}
	}

	framebufWidth = width;
	framebufHeight = height;
}

// handling resize, also updating the resize transformation matrix
void GLContext::handleFramebufferResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	instance->setFramebufferSize(width, height);
}

//void GLContext::initVao(const std::vector<glm::vec4> &positionArray, const std::vector<glm::vec4> &colorArray, const std::vector<glm::vec4>& normalsArray) {
//	vaoHandler.initVao(positionArray, colorArray, normalsArray);
//}

void GLContext::loop(void (*callback)(double)) {
	winHandle.setLoopCallback(callback).loop();
}