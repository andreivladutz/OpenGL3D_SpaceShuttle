#pragma once
#include "ShaderLoader.h"
#include <map>

#include "../glm/glm.hpp"

class ProgramLoader 
{
	GLuint programId = 0;
	bool programLinkageSucceeded = false;
	std::vector<ShaderLoader> shaderLoaders;

public:
	ProgramLoader();
	~ProgramLoader();

	ProgramLoader& attachShader(const char *filename, GLenum shaderType) noexcept(false);
	ProgramLoader& linkProgram() noexcept(false);
	// fully compile program from a dictionary of shaders
	ProgramLoader& compileProgram(std::map<std::string, GLenum> shadersDictionary);
	GLuint getProgramId();

	ProgramLoader& deleteShaders();
	void deleteProgram();

	void setFloat(const std::string& name, float value) const;
	void setInt(const std::string& name, int value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setUniformMatrix(std::string uniformName, glm::mat4 matrix) const;

	ProgramLoader& use() {
		glUseProgram(programId);

		return *this;
	}
};