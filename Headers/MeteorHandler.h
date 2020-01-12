#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>

#include "./Model.h"
#include "./ProgramLoader.h"
#include "../glm/glm.hpp"

using namespace std;

const float Z_FAR = -100,
	Z_NEAR = 100,
	MIN_X = -50,
	MAX_X = 50,
	MIN_Y = -50,
	MAX_Y = 50;
const int METEOR_COUNT = 3, INSTANCE_COUNT = 10;
const vector<string> METEOR_PATHS{
	"./Assets/meteors/meteor1.obj",
	"./Assets/meteors/meteor2.obj",
	"./Assets/meteors/meteor3.obj"
};

class MeteorHandler {
public:
	ProgramLoader* meteorProgram;
	vector<Model*> meteors;
	// keep the translation of each rendered instance
	list<pair<int, glm::mat4>> translations;

	MeteorHandler() noexcept(false);
	~MeteorHandler();
	
	MeteorHandler& updatePositions(double deltaTime);
	MeteorHandler& draw(glm::vec3 viewPos);
private:
	void generateMeteor();
};