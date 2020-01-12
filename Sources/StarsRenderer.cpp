#include "../Headers/StarsRenderer.h"


StarsRenderer::StarsRenderer() {
	Material starMaterial{
		glm::vec3(1, 1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(1, 1, 1),
		32
	};

	vector<Vertex> starVertices(STAR_COUNT + 1);
	vector<unsigned int> starIndices(STAR_COUNT + 1);

	for (unsigned int i = 0; i <= STAR_COUNT; i++) {
		float randRadius = glm::linearRand<float>(lowerRadiusBound, higherRadiusBound);

		starVertices[i] = {
			glm::vec3(glm::diskRand(randRadius), zPosition),
			glm::vec3(0, 0, 1),
			glm::vec2(0, 0)
		};

		starIndices[i] = i;
	}

	stars = new Mesh(starVertices, starIndices, std::vector<Texture>(), starMaterial);
}

StarsRenderer::~StarsRenderer() {
	delete stars;
}

void StarsRenderer::Draw(ProgramLoader* program) {
	glPointSize(2);
	program->setUniformMatrix("modelMatrix",
		glm::mat4(1)
	);

	stars->DrawPoints(program);
}