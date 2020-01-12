#pragma once
#include <string>
#include <exception>
#include <vector>
#include <fstream>
#include <GL/glew.h>

class ShaderLoader {
	GLenum shaderType;
	std::string fileLocation;
	std::string sourceCode;
	std::ifstream shaderSource;

	GLuint shaderId;

public:
	ShaderLoader(std::string location, GLenum shaderType) noexcept(false);
	ShaderLoader(ShaderLoader&&) noexcept;
	~ShaderLoader();

	const ShaderLoader& loadShader() noexcept(false);
	void deleteShader(GLuint programId = 0);

	GLuint getShaderId() const;
};