#pragma once

#include "DrawableObject.h"
#include "utils.h"

class Circle : public DrawableObject {
public:
	Circle(float radius, int numPoints, glm::vec4 color);
protected:
	std::vector<glm::vec4>& generatePoints(float radius, int numPoints);

	std::vector<glm::vec4> generatedPoints;
	glm::vec4 color;
};