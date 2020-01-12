#pragma once

#include "./ProgramLoader.h"
#include "./Model.h"
#include "./Camera.h"
#include "./utils.h"

class ProgramLoader;

const float Y_DISTANCE_FROM_CAMERA = 7,
	TILT_ANGLE = PI / 8;

class Ship {
public:
	ProgramLoader* shipProgram;
	Model* shipModel;

	bool tiltingLeft = false, tiltingRight = false;
	const double TILT_TIME = 0.6;
	double timeSinceTiltStart = 0;
	float moveStep = 0,
		shipAngle = 0,
		lastFraction = 0;

	bool repeatingKeyPress = false;

	glm::vec3 translationVector = glm::vec3(0, -Y_DISTANCE_FROM_CAMERA, -5);

	Ship(ProgramLoader* shipProgram, const char* modelPath);
	~Ship();

	void Draw();
	void moveShip(Camera::MOVEMENT direction, float step, bool repeatingKeyPress);

	Ship& update(double deltaTime, Camera& camera);

	void stopRepeatingTilt();
private:
};