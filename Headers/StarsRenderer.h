#pragma once

#include "./Mesh.h"
#include "./ProgramLoader.h"

#include "../glm/gtc/random.hpp"
#include "../glm/gtx/transform.hpp"

class StarsRenderer {
public:
	const unsigned int STAR_COUNT = 3000;
	const float zPosition = -300,
		lowerRadiusBound = 50,
		higherRadiusBound = 15000;

	Mesh *stars;

	StarsRenderer();
	~StarsRenderer();

	void Draw(ProgramLoader* program);
protected:
};