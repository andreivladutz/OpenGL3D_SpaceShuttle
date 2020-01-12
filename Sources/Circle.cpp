#include "../Headers/Circle.h"

Circle::Circle(float radius, int numPoints, glm::vec4 color)
: DrawableObject(generatePoints(radius, numPoints), std::vector<glm::vec4>(numPoints + 1, color), GL_TRIANGLE_FAN), color(color) {}

std::vector<glm::vec4>& Circle::generatePoints(float radius, int numPoints) {
	std::vector<float> angles = linearSpace(0, 2 * PI, numPoints);

	// push the origin
	generatedPoints.push_back(glm::vec4(0, 0, 0, 1));

	// push the points on the circle
	for (float& angle : angles) {
		generatedPoints.push_back(
			glm::vec4(radius * glm::cos(angle), radius * glm::sin(angle), 0, 1)
		);
	}

	return generatedPoints;
}