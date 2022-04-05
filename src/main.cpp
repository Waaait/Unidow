#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/rendering/shader.h"
#include "graphics/rendering/texture.h"
#include "graphics/models/cube.hpp"
#include "graphics/models/lamp.hpp"
#include "graphics/objects/model.h"
#include "graphics/rendering/planet.h"

#include "io/keyboard.h"
#include "io/camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, double dt);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

glm::mat4 transform = glm::mat4(1.0f);

// LIGHT
float x, y, z;
float lightPosX, lightPosY, lightPosZ;

// CAMERA
Camera camera(glm::vec3(8.0f, 0.0f, 30.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
	int success;
	char infoLog[512];

	std::cout << "Hello, world!" << std::endl;

	glfwInit();

	// openGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

# ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Unidow", NULL, NULL);
	if (window == NULL) { // window not created
		std::cout << "Could not create window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	// set callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, Keyboard::keyCallback);

	glEnable(GL_DEPTH_TEST);

	// SHADERS===============================
	Shader shader("assets/object.vs", "assets/object.fs");
	Shader lampShader("assets/object.vs", "assets/lamp.fs");

	// PLANET
	Planet system;
	system.genList(system);

	const int numPlanet = 9;
	Cube planet[numPlanet];
	for (unsigned int i = 0; i < numPlanet; i++)
	{
		glm::vec3 distance;
		if (i == 0) {
			distance = glm::vec3((2.0f * 1), 0.0, -1.0f);
		}
		else {
			/*distance = glm::vec3((2.0f * (i+1) * system.getListitem(i).distance), 0.0, -1.0f);*/
			distance = glm::vec3((2.0f * (i + 1)), 0.0, -1.0f);
		}
		planet[i] = Cube(system.getListitem(i), Material::red_rubber, distance, glm::vec3(0.75f));
		planet[i].init();
	}

	// LIGHTS

	lightPosX = -1.0f;
	lightPosY = 0.0f;
	lightPosZ = 0.0f;

	Lamp lamp(glm::vec3(0.9f,0.6f,0.3f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(lightPosX, lightPosY, lightPosZ), glm::vec3(1.75f));
	lamp.init();

	// TEXTURES_____________________________________
	x = 0.0f;
	y = 0.0f;
	z = 5.0f;

	while (!glfwWindowShouldClose(window)) {
		// calculate dt
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// process input
		processInput(window, deltaTime);

		// render
		glClearColor(.0f, .0f, .0f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		shader.activate();

		shader.activate();
		shader.set3Float("light.position", lamp.pos);

		shader.set3Float("light.ambient", lamp.ambient);
		shader.set3Float("light.diffuse", lamp.diffuse);
		shader.set3Float("light.specular", lamp.specular);

		// create transformation
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		//model.render(shader, 2.4f);

		for (unsigned int i = 0; i < numPlanet; i++) {
			planet[i].render(shader, 24.7f);
		}

		lampShader.activate();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		lamp.render(lampShader, glm::vec3(lightPosX, lightPosY, lightPosZ), Planet::Sun.selfRotationSpeed);


		// send new frame to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	for (unsigned int i = 0; i < numPlanet; i++) {
		planet[i].cleanup();
	}
	//model.cleanup();
	lamp.cleanup();
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

void processInput(GLFWwindow* window, double dt) {
	if (Keyboard::key(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}

	// move light
	if (Keyboard::keyWentDown(GLFW_KEY_A)) {
		lightPosX -= 0.5;
		printf("light pos X %f\n", lightPosX);
	}
	if (Keyboard::keyWentDown(GLFW_KEY_D)) {
		lightPosX += 0.5;
		printf("light pos X %f\n", lightPosX);
	}
	if (Keyboard::keyWentDown(GLFW_KEY_W)) {
		lightPosY += 0.5;
		printf("light pos Y %f\n", lightPosY);
	}
	if (Keyboard::keyWentDown(GLFW_KEY_S)) {
		lightPosY -= 0.5;
		printf("light pos Y %f\n", lightPosY);
	}

	//move camera
	if (Keyboard::key(GLFW_KEY_UP)) {
		camera.updateCameraPos(CameraDirection::UP, dt);
	}
	if (Keyboard::key(GLFW_KEY_DOWN)) {
		camera.updateCameraPos(CameraDirection::DOWN, dt);
	}
	if (Keyboard::key(GLFW_KEY_LEFT)) {
		camera.updateCameraPos(CameraDirection::LEFT, dt);
	}
	if (Keyboard::key(GLFW_KEY_RIGHT)) {
		camera.updateCameraPos(CameraDirection::RIGHT, dt);
	}
	if (Keyboard::key(GLFW_KEY_COMMA)) {
		camera.updateCameraPos(CameraDirection::BACKWARD, dt);
	}
	if (Keyboard::key(GLFW_KEY_PERIOD)) {
		camera.updateCameraPos(CameraDirection::FORWARD, dt);
	}
}