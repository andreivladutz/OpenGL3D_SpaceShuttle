#pragma once

#include "ProgramLoader.h"
#include "WindowHandler.h"

class GLContext {
public:
	int framebufWidth = 0, framebufHeight = 0;

	WindowHandler winHandle;
	// VAOHandler vaoHandler;
	ProgramLoader* progrLoader;

	GLuint programId = 0;

	static GLContext& createContext(const GLchar* windowName, std::map<std::string, GLenum> shadersDictionary) noexcept(false);
	static GLContext& getContext() noexcept(false);

	static void handleFramebufferResize(GLFWwindow* window, int width, int height);
	void setFramebufferSize(int width = 0, int height = 0);

	// void initVao(const std::vector<glm::vec4>& positionArray, const std::vector<glm::vec4>& colorArray, const std::vector<glm::vec4>& normalsArray);
	void loop(void (*callback)(double));
private:
	static GLContext* instance;

	GLContext(const GLchar* windowName, std::map<std::string, GLenum> shadersDictionary) noexcept(false);
};