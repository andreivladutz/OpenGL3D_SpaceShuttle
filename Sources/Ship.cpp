#include "../Headers/Ship.h"

#include "../glm/gtx/transform.hpp"
#include "../glm/glm.hpp"

Ship::Ship(ProgramLoader* shipProgram, const char* modelPath) : shipProgram(shipProgram) {
	shipModel = new Model(modelPath);
}

Ship::~Ship() {
	delete shipModel;
}

void Ship::Draw() {
	glm::mat4 rotationMatrix(1);

	if (tiltingLeft) {
		rotationMatrix = glm::rotate(shipAngle, glm::vec3(0, 0, 1));
	}
	else if (tiltingRight) {
		rotationMatrix = glm::rotate(-shipAngle, glm::vec3(0, 0, 1));
	}

	// draw the ship with the main program loader object
	shipProgram->setUniformMatrix("modelMatrix",
		glm::translate(translationVector) * rotationMatrix
	);
	shipModel->Draw(shipProgram);
}

void Ship::moveShip(Camera::MOVEMENT direction, float step, bool repeatingKeyPress) {
	if (direction == Camera::MOVEMENT::UP) {
		translationVector.y = glm::min(translationVector.y + step, Camera::MAX.UP - Y_DISTANCE_FROM_CAMERA);
	}
	else if (direction == Camera::MOVEMENT::DOWN) {
		translationVector.y = glm::max(translationVector.y - step, Camera::MAX.DOWN - Y_DISTANCE_FROM_CAMERA);
	}
	else if (direction == Camera::MOVEMENT::RIGHT) {
		this->repeatingKeyPress = repeatingKeyPress;
		if (tiltingRight) {
			return;
		}

		timeSinceTiltStart = 0;
		tiltingRight = true;
		tiltingLeft = false;

		moveStep = step;
		
	}
	else if (direction == Camera::MOVEMENT::LEFT) {
		this->repeatingKeyPress = repeatingKeyPress;
		if (tiltingLeft) {
			return;
		}

		timeSinceTiltStart = 0;
		tiltingLeft = true;
		tiltingRight = false;

		moveStep = -step;
	}
}

Ship& Ship::update(double deltaTime, Camera& camera) {
	timeSinceTiltStart += deltaTime;
	float fraction = glm::clamp(timeSinceTiltStart / TILT_TIME, 0., 1.),
		step = moveStep * fraction,
		initialFraction = fraction;

	// go forward until the middle then go back to zero for the other half of the interval
	if (fraction >= 0 && fraction <= 0.5) {
		fraction = fraction * 2;
	}
	else if (fraction > 0.5 && fraction <= 1) {
		fraction = (1 - fraction) * 2;
	}

	// if repeated key press clamp the fraction
	if (repeatingKeyPress) {
		fraction = lastFraction;
		step = moveStep;
	}

	lastFraction = fraction;

	if (tiltingLeft) {
		camera.moveCamera(Camera::MOVEMENT::LEFT, -step);
	}
	else if (tiltingRight) {
		camera.moveCamera(Camera::MOVEMENT::RIGHT, step);
	}
	else {
		return *this;
	}

	translationVector.x = glm::clamp(translationVector.x + step, Camera::MAX.LEFT, Camera::MAX.RIGHT);

	shipAngle = TILT_ANGLE * fraction;

	if (translationVector.x == Camera::MAX.LEFT || translationVector.x == Camera::MAX.RIGHT) {
		shipAngle = 0;
	}

	if (initialFraction >= 1 && !repeatingKeyPress) {
		tiltingLeft = tiltingRight = false;
	}

	return *this;
}

void Ship::stopRepeatingTilt() {
	repeatingKeyPress = false;
}