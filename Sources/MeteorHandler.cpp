#include "../Headers/MeteorHandler.h"

#include "../glm/gtc/random.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/transform.hpp"

MeteorHandler::MeteorHandler() noexcept(false) {
	for (int i = 0; i < METEOR_COUNT; i++) {
		meteors.push_back(new Model(METEOR_PATHS[i].c_str()));
	}

	meteorProgram = new ProgramLoader();
	meteorProgram->compileProgram({
		{ "./Shaders/meteorVS.vert", GL_VERTEX_SHADER },
		{ "./Shaders/meteorFS.frag", GL_FRAGMENT_SHADER },
	});

	for (int i = 0; i < INSTANCE_COUNT; i++) {
		generateMeteor();
	}
}

MeteorHandler::~MeteorHandler() {
	for (int i = 0; i < METEOR_COUNT; i++) {
		delete meteors[i];
	}
}

MeteorHandler& MeteorHandler::updatePositions(double deltaTime) {
	for (list<pair<int, glm::mat4>>::iterator it = translations.begin(); it != translations.end(); it++) {
		pair<int, glm::mat4>& meteorInstance = *it;
		float zPos = 
			meteorInstance.second[3][2] = meteorInstance.second[3][2] + deltaTime * glm::linearRand<int>(5, 10);

		if (zPos > Z_NEAR) {
			translations.erase(it);
		}
	}

	if (glm::linearRand<int>(0, INSTANCE_COUNT) == 0) {
		generateMeteor();
	}

	return *this;
}

MeteorHandler& MeteorHandler::draw(glm::vec3 viewPos) {
	for (pair<int, glm::mat4> &meteorInstance: translations) {
		meteorProgram->use();

		meteorProgram->setUniformMatrix("modelMatrix",
			meteorInstance.second
		);

		meteorProgram->setVec3("viewPos", viewPos);

		meteors[meteorInstance.first]->Draw(meteorProgram);
	}

	return *this;
}

void MeteorHandler::generateMeteor() {
	// get random index of meteor
	int rndIndex = glm::linearRand<int>(0, METEOR_COUNT - 1);

	// generate random coords
	float rndX = glm::linearRand<float>(MIN_X, MAX_X),
		rndY = glm::linearRand<float>(MIN_Y, MAX_Y);

	translations.push_back({ rndIndex, glm::translate(glm::vec3(rndX, rndY, Z_FAR)) });
}