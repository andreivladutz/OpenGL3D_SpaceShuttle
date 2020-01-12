#include "../Headers/ShaderLoader.h"
#include <cstring>

ShaderLoader::ShaderLoader(std::string location, GLenum shaderType) noexcept(false) 
	: shaderType(shaderType), fileLocation(location), shaderId(0) {
	shaderSource.open(location.c_str());

	if (!shaderSource.is_open()) {
		std::string errMsg(" shader file could not be located.");
		errMsg = location + errMsg;

		throw std::exception(errMsg.c_str());
	}
}

ShaderLoader::ShaderLoader(ShaderLoader &&other) noexcept 
	: fileLocation(std::move(other.fileLocation)), sourceCode(std::move(other.sourceCode)), shaderSource((std::move(other.shaderSource))){
	
	shaderType = other.shaderType;
	shaderId = other.shaderId;

	other.shaderId = 0;
}

ShaderLoader::~ShaderLoader() {
	deleteShader();

	if (shaderSource.is_open()) {
		shaderSource.close();
	}
}

const ShaderLoader& ShaderLoader::loadShader() noexcept(false) {
	if (!shaderSource.is_open()) {
		throw std::exception("Attempting to read from an inexistent or closed shader file!");
	}

	const int maxLineSize = 1000;
	char line[maxLineSize];

	sourceCode = "";
	// read the shader source file 
	while (shaderSource.getline(line, maxLineSize)) {
		sourceCode += line;
		sourceCode += "\n";
	}

	shaderSource.close();

	// create the shader, pass the source code and compile
	shaderId = glCreateShader(shaderType);

	const GLchar* sourceCodeCString = sourceCode.c_str();
	// 1 = length. consider only the first cstring of the string array
	glShaderSource(shaderId, 1, &sourceCodeCString, NULL);
	glCompileShader(shaderId);

	// checking the success of the compilation
	GLint success = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	// compilation failed
	if (success == GL_FALSE) {
		// get the length of the error logs
		GLint logMsgLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logMsgLength);

		std::vector<GLchar> logMsg(logMsgLength);
		glGetShaderInfoLog(shaderId, logMsgLength, NULL, &logMsg[0]);

		// don't leak the shader
		deleteShader();

		// build the error message
		std::string errorDetail("Failed to compile ");
		errorDetail += fileLocation + ":\n";
		// attach the log message retrieved from the shader
		logMsg.insert(logMsg.begin(), errorDetail.begin(), errorDetail.end());
		// throw an exception with the error message
		throw std::exception(&logMsg[0]);
	}

	return *this;
}

void ShaderLoader::deleteShader(GLuint programId) {
	if (programId != 0) {
		// detach the shader from the program
		glDetachShader(programId, shaderId);
	}

	glDeleteShader(shaderId);
	shaderId = 0;
}

GLuint ShaderLoader::getShaderId() const {
	return shaderId;
}