#include "./Headers/GLContext.h"
#include "./Headers/Model.h"
#include "./Headers/MeteorHandler.h"
#include "./Headers/Camera.h"
#include "./Headers/Ship.h"
#include "./Headers/StarsRenderer.h"

#include <iostream>

using std::string;

GLContext *glCtx;
Model * planetCoreModel, * planetCrustModel;

ProgramLoader* planetProgram;
std::vector<ProgramLoader*> shaderPrograms;

// game objects
MeteorHandler* meteorHandler;
Camera camera;
Ship* ship; 
StarsRenderer* starsRenderer;

glm::vec3 lightSource(-100, 50, -50), lightColor(1, 1, 1);

// move the camera with the mouse cursor
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	camera.panCamera(xpos, ypos);
}

// move the observer via key pressing
void keyCallback(GLFWwindow* window, int key, int, int action, int) {
	const float MOVEMENT_STEP = 1;

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_UP) {
			camera.moveCamera(Camera::MOVEMENT::UP, MOVEMENT_STEP * 2);
			ship->moveShip(Camera::MOVEMENT::UP, MOVEMENT_STEP * 2, false);
		}
		else if (key == GLFW_KEY_DOWN) {
			camera.moveCamera(Camera::MOVEMENT::DOWN, MOVEMENT_STEP * 2);
			ship->moveShip(Camera::MOVEMENT::DOWN, MOVEMENT_STEP * 2, false);
		}
		else if (key == GLFW_KEY_RIGHT) {
			ship->moveShip(Camera::MOVEMENT::RIGHT, MOVEMENT_STEP, action == GLFW_REPEAT);
		}
		else if (key == GLFW_KEY_LEFT) {
			ship->moveShip(Camera::MOVEMENT::LEFT, MOVEMENT_STEP, action == GLFW_REPEAT);
		}
	}

	if (action == GLFW_RELEASE) {
		ship->stopRepeatingTilt();
	}
}

void clearScreen() {
	glm::vec4 backgroundColor{ 0.02, 0.03, 0.05, 1 };

	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderFunction(double deltaTime) {
	clearScreen();

	// the ship update also moves the camera
	ship->update(deltaTime, camera);
	camera.resetViewMatrix(shaderPrograms);

// ---------------------------------------------------> planet program (shaders)
	// draw the planet core with it's special shaders
	planetProgram->use();
	planetProgram->setUniformMatrix("modelMatrix",
		glm::translate(lightSource) * glm::scale(glm::vec3(5, 5, 5)) * glm::rotate(PI / 2, glm::vec3(0, 1, 0))
	);
	planetCoreModel->Draw(planetProgram);

	// draw the background stars
	starsRenderer->Draw(planetProgram);
// ---------------------------------------------------> ship program -> planet's crust
	// draw the planet's crust with the main shaders
	glCtx->progrLoader->use();
	glCtx->progrLoader->setUniformMatrix("modelMatrix",
		glm::translate(lightSource) * glm::scale(glm::vec3(5, 5, 5)) * glm::rotate(PI / 2, glm::vec3(0, 1, 0))
	);
	planetCrustModel->Draw(glCtx->progrLoader);

// ---------------------------------------------------> ship program
	ship->Draw();

// ---------------------------------------------------> meteor program
	meteorHandler->updatePositions(deltaTime).draw(camera.getObserver());

	glFlush();
}

void setLight() {
	for (auto* programShader : shaderPrograms) {
		// set the light location uniform for the fragment shader
		programShader->use().setVec3("lightPosition", lightSource);

		// set the light color uniform for the fragment shader
		programShader->setVec3("lightColor", lightColor);
	}
}

void init() {
	GLFWwindow* window = glCtx->winHandle.getWindow();

	// keyboard input
	glfwSetKeyCallback(window, keyCallback);

	// mouse input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	glfwSetCursorPosCallback(window, cursorPositionCallback);

	glEnable(GL_DEPTH_TEST);

	setLight();
}

int main() {
	try 
	{
		glCtx = &GLContext::createContext("OpenGL 3D", {
			{ "./Shaders/shipVS.vert", GL_VERTEX_SHADER },
			{ "./Shaders/shipFS.frag", GL_FRAGMENT_SHADER },
		});

		planetProgram = new ProgramLoader();
		planetProgram->compileProgram({
			{ "./Shaders/planetVS.vert", GL_VERTEX_SHADER },
			{ "./Shaders/planetFS.frag", GL_FRAGMENT_SHADER },
		});

		meteorHandler = new MeteorHandler();

		shaderPrograms = std::vector<ProgramLoader*>{
			glCtx->progrLoader, planetProgram, meteorHandler->meteorProgram
		};
	}
	catch(std::exception &e) {
		std::cout << e.what();
		
		exit(0);
	}

	camera.initCameraView();

	init();

	//planetModel = new Model("./Assets/planet/planet.obj");
	planetCoreModel = new Model("./Assets/planet/planet_core.obj");
	planetCrustModel = new Model("./Assets/planet/planet_crust.obj");

	ship = new Ship(glCtx->progrLoader, "./Assets/ship/ship.obj");

	starsRenderer = new StarsRenderer;

	camera.resetProjectionMatrix(shaderPrograms);

	//glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT);

	glCtx->loop(renderFunction);

	delete planetProgram;
	delete planetCoreModel;
	delete planetCrustModel;
	delete ship;
	delete starsRenderer;
}
