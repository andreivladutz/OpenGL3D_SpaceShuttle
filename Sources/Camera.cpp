#include "../Headers/Camera.h"

const Camera::MovementLimit Camera::MAX = {
	10, -10, 30, -30, {
		50, -50, 100, -100
	}};

Camera& Camera::initCameraView() {
	return *this;
}

Camera& Camera::moveCamera(MOVEMENT direction, float step) {
	if (direction == MOVEMENT::UP) {
		obsY = glm::min(obsY + step, MAX.UP);
		
		if (refY > MAX.UP) {
			return *this;
		}
		refY = glm::min(refY + step, MAX.UP);
	}
	else if (direction == MOVEMENT::DOWN) {
		obsY = glm::max(obsY - step, MAX.DOWN);

		if (refY < MAX.DOWN) {
			return *this;
		}
		refY = glm::max(refY - step, MAX.DOWN);
	}
	else if (direction == MOVEMENT::RIGHT) {
		obsX = glm::min(obsX + step, MAX.RIGHT);

		if (refX > MAX.RIGHT) {
			return *this;
		}
		refX = glm::min(refX + step, MAX.RIGHT);
	}
	else if (direction == MOVEMENT::LEFT) {
		obsX = glm::max(obsX - step, MAX.LEFT);

		if (refX < MAX.LEFT) {
			return *this;
		}
		refX = glm::max(refX - step, MAX.LEFT);
	}

	return *this;
}

// move the camera with the mouse cursor
Camera& Camera::panCamera(double xpos, double ypos) {
	if (firstMove) {
		lastMouseX = (float)xpos; lastMouseY = (float)ypos;
		firstMove = false;

		return *this;
	}

	float deltaX = float(xpos) - lastMouseX, deltaY = lastMouseY - float(ypos);

	refX = glm::clamp(refX + deltaX / 1, MAX.PAN.LEFT, MAX.PAN.RIGHT);
	refY = glm::clamp(refY + deltaY / 1, MAX.PAN.DOWN, MAX.PAN.UP);

	lastMouseX = (float)xpos; lastMouseY = (float)ypos;

	return *this;
}

void Camera::resetViewMatrix(std::vector<ProgramLoader*>& shaderPrograms) {
	glm::vec3 observer = glm::vec3(obsX, obsY, obsZ),
		referencePt = glm::vec3(refX, refY, refZ);

	for (auto* programShader : shaderPrograms) {
		programShader->use().setUniformMatrix("viewMatrix",
			glm::lookAt(observer, referencePt, glm::vec3(0, 1, 0))
		);
	}
}

void Camera::resetProjectionMatrix(std::vector<ProgramLoader*>& shaderPrograms) {
	GLContext& glCtx = GLContext::getContext();

	for (auto* programShader : shaderPrograms) {
		programShader->use().setUniformMatrix("projectionMatrix",
			glm::perspective(PI / 2, GLfloat(glCtx.framebufWidth) / GLfloat(glCtx.framebufHeight), 1.0f, -5.0f)
		);
	}
}