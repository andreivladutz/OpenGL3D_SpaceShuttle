#pragma once

#include "./ProgramLoader.h"
#include "./utils.h"
#include "./GLContext.h"

#include <vector>

#include "../glm/glm.hpp"
#include "../glm/gtc/random.hpp"
#include "../glm/gtx/transform.hpp"
#include "../glm/gtx/string_cast.hpp"
#include "../glm/gtc/matrix_transform.hpp"

class Camera {
public:
	const static struct MovementLimit {
		float UP, DOWN, RIGHT, LEFT;

		struct {
			float UP, DOWN, RIGHT, LEFT;
		} PAN;
	} MAX;

	enum class MOVEMENT {UP, DOWN, RIGHT, LEFT};

	float obsX = 0, obsY = 0, obsZ = 10, refX = 0, refY = 0, refZ = -200;
	float lastMouseX = 0, lastMouseY = 0;

	bool firstMove = true;

	Camera& initCameraView();
	Camera& moveCamera(MOVEMENT direction, float step);
	Camera& panCamera(double xpos, double ypos);


	void resetViewMatrix(std::vector<ProgramLoader*>& shaderPrograms);
	void resetProjectionMatrix(std::vector<ProgramLoader*>& shaderPrograms);

	glm::vec3 getObserver() {
		return glm::vec3(obsX, obsY, obsZ);
	}
	glm::vec3 getRefPoint() {
		return glm::vec3(refX, refY, refZ);
	}
private:
};