#include "../Headers/ProgramLoader.h"
#include <iostream>

ProgramLoader::ProgramLoader() {
	programId = glCreateProgram();
}

ProgramLoader::~ProgramLoader() {
	deleteProgram();
}

ProgramLoader& ProgramLoader::attachShader(const char* filename, GLenum shaderType) noexcept(false) {
	// create a new shaderLoader and push it in the vector of 
	// ShaderLoaders after the attaching to the program succeeded
	ShaderLoader shLoader = ShaderLoader(filename, shaderType);
	
	GLuint shaderId;

	// read the shader from the source file and compile it, then get the it's id
	try {
		shaderId = shLoader
			.loadShader()
			.getShaderId();
	}
	catch (std::exception e) {
		// throw the error outside
		throw e;
	}

	glAttachShader(programId, shaderId);
	shaderLoaders.push_back(std::move(shLoader));

	// return a reference to this to enable method chaining
	return *this;
}

ProgramLoader& ProgramLoader::linkProgram() noexcept(false) {
	glLinkProgram(programId);

	GLint isLinked = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);

	// Linkage failed

	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

		std::string outError("Program linkage failed\n");

		if (maxLength != 0)
		{
			std::vector<GLchar> errMsg(maxLength);
			glGetProgramInfoLog(programId, maxLength, &maxLength, &errMsg[0]);

			outError += &errMsg[0];
		}

		// if there is no linkage, there is no point to keeping the program and the shaders
		deleteProgram();

		throw std::exception(outError.c_str());
	}

	programLinkageSucceeded = true;
	return *this;
}

GLuint ProgramLoader::getProgramId() {
	return programId;
}

ProgramLoader& ProgramLoader::deleteShaders() {
	for (std::vector<ShaderLoader>::iterator it = shaderLoaders.begin(); it != shaderLoaders.end(); it++) {
		// if the linkage succeeded, we know the shaders have also been successfully attached to the program
		if (programLinkageSucceeded) {
			// before deleting the shader, make sure we detach it from the progam
			it->deleteShader(programId);
		}
		else {
			it->deleteShader();
		}
	}

	// remove all shaderLoader objects from the vector
	while (!shaderLoaders.empty()) {
		shaderLoaders.pop_back();
	}

	return *this;
}

ProgramLoader&  ProgramLoader::compileProgram(std::map<std::string, GLenum> shadersDictionary) noexcept(false) {
	// compile all shaders, link program and delete shaders
	try {
		for (auto& shader : shadersDictionary) {
			// first is the path to the shader, second is the type of the shader
			attachShader(shader.first.c_str(), shader.second);
		}

		linkProgram().deleteShaders();
	}
	catch (std::exception & e) {
		throw e;
	}

	return *this;
}

void ProgramLoader::deleteProgram()
{
	deleteShaders();
	glDeleteProgram(programId);
}

// ------------------------------------------------------------------------
void ProgramLoader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

// ------------------------------------------------------------------------

void ProgramLoader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

// ------------------------------------------------------------------------
void ProgramLoader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

// ------------------------------------------------------------------------
void ProgramLoader::setUniformMatrix(std::string uniformName, glm::mat4 matrix) const {
	GLint location = glGetUniformLocation(programId, uniformName.c_str());

	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}