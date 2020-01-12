#include "../Headers/utils.h"

std::vector<float> linearSpace(float start, float end, int numPoints) {
	if (numPoints <= 2) {
		return { start, end };
	}

	std::vector<float> linspace(numPoints);
	linspace[0] = start;

	float step = (end - start) / (numPoints - 1);

	for (int i = 1; i < numPoints; i++) {
		linspace[i] = linspace[i - 1] + step;
	}

	return linspace;
}