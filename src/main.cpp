#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <fstream>
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

#include "io/keyboard.h"
#include "io/camera.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, double dt);
glm::vec3 generateCode();
int checkPrime(int num);
void primePowerInit();
int primePowerCheck(glm::vec3 code);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

const int PRIME_UPPERBOUND = 100;
list<float> primePower;

const int NUM_CUBE = 4;

int dangerRoom = 0;
int totalRoom = NUM_CUBE * NUM_CUBE * NUM_CUBE;

glm::mat4 transform = glm::mat4(1.0f);

// LIGHT
float x, y, z;
float lightPosX, lightPosY, lightPosZ;

// CAMERA
Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
	int success;
	char infoLog[512];

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

	// MODELS
	primePowerInit();
	
	Cube room[NUM_CUBE][NUM_CUBE][NUM_CUBE];
	for (unsigned int i = 0; i < NUM_CUBE; i++)
	{
		for (unsigned int j = 0; j < NUM_CUBE; j++)
		{
			for (unsigned int k = 0; k < NUM_CUBE; k++)
			{
				glm::vec3 newCode = generateCode();
				Material mat = Material::white_plastic;
				if (primePowerCheck(newCode)) {
					mat = Material::red_plastic;
					printf("Room (%d, %d, %d) has traps with code (%f, %f, %f). \n", i, j, k, newCode.x, newCode.y, newCode.z);
					dangerRoom++;
				}
				else {
					mat = Material::jade;
				}
				room[i][j][k] = Cube(newCode, mat, glm::vec3((float)i, (float)j, -(float)k), glm::vec3(0.75f));
				room[i][j][k].init();
			}
		}
	}

	printf("================ CUBE REPORT ================ \n");
	printf("Number of rooms with traps: %d .\n",dangerRoom);
	printf("Number of rooms that are safe: %d .\n", totalRoom - dangerRoom);

	// LIGHTS

	lightPosX = -1.0f;
	lightPosY = 0.0f;
	lightPosZ = 1.0f;

	Lamp lamp(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(lightPosX, lightPosY, lightPosZ), glm::vec3(0.75f));
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
		glClearColor(.7f, .8f, .9f, 0.1f);
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

		for (unsigned int i = 0; i < NUM_CUBE; i++) {
			for (unsigned int j = 0; j < NUM_CUBE; j++)
			{
				for (unsigned int k = 0; k < NUM_CUBE; k++)
				{
					room[i][j][k].render(shader, 24.7f);
				}
			}
			
		}

		lampShader.activate();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		lamp.render(lampShader, glm::vec3(lightPosX, lightPosY, lightPosZ));

		// send new frame to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	for (unsigned int i = 0; i < NUM_CUBE; i++) {
		for (unsigned int j = 0; j < NUM_CUBE; j++)
		{
			for (unsigned int k = 0; k < NUM_CUBE; k++)
			{
				room[i][j][k].cleanup();
			}
		}

	}

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
	}
	if (Keyboard::keyWentDown(GLFW_KEY_D)) {
		lightPosX += 0.5;
	}
	if (Keyboard::keyWentDown(GLFW_KEY_W)) {
		lightPosY += 0.5;
	}
	if (Keyboard::keyWentDown(GLFW_KEY_S)) {
		lightPosY -= 0.5;
	}
	if (Keyboard::keyWentDown(GLFW_KEY_Q)) {
		lightPosZ -= 0.5;
	}
	if (Keyboard::keyWentDown(GLFW_KEY_E)) {
		lightPosZ += 0.5;
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
	if (Keyboard::key(GLFW_KEY_PAGE_DOWN)) {
		camera.updateCameraPos(CameraDirection::BACKWARD, dt);
	}
	if (Keyboard::key(GLFW_KEY_DELETE)) {
		camera.updateCameraPos(CameraDirection::FORWARD, dt);
	}
}

glm::vec3 generateCode() {
	int vecList[3];
	for (unsigned int i = 0; i < 3; i++)
	{
		vecList[i] = 0;
	}
	for (unsigned int i = 0; i <3; i++)
	{
		float num = 1 + (rand() % 100);
		vecList[i] = num;
	}
	return glm::vec3(vecList[0], vecList[1], vecList[2]);
}

int checkPrime(int num)
{
	if (num < 2 || num == 4) {
		return 0;
	}
	else {
		int x = num / 2;
		for (int i = 2; i < x; i++)
		{
			if (num % i == 0)
			{
				return 0;
			}
		}
	}

	return 1;
}

void primePowerInit() {
	// find all prime within 100
	for (unsigned int i = 0; i < PRIME_UPPERBOUND; i++)
	{
		if (checkPrime(i)) {
			primePower.push_back(i);
		}
	}

	// prime ^ 2
	for (auto i : primePower) {
		int square = (int)i * i;
		if (square < PRIME_UPPERBOUND) {
			primePower.push_back(square);
		}
		else {
			break;
		}
	}

	// prime ^ 3
	for (auto i : primePower) {
		int square = (int)i * i * i;
		if (square < PRIME_UPPERBOUND) {
			primePower.push_back(square);
		}
		else {
			break;
		}
	}

	// prime ^ 4
	for (auto i : primePower) {
		int square = (int)i * i;
		square *= square;
		if (square < PRIME_UPPERBOUND) {
			primePower.push_back(square);
		}
		else {
			break;
		}
	}

	// prime ^ 5
	for (auto i : primePower) {
		int square = (int)i * i;
		square *= square;
		square *= i;
		if (square < PRIME_UPPERBOUND) {
			primePower.push_back(square);
		}
		else {
			break;
		}
	}
}

int primePowerCheck(glm::vec3 code) {
	list<float>::iterator it;
	for ( it = primePower.begin();  it != primePower.end();  it++)
	{
		if (code.x == *it || code.y == *it || code.z == *it) {
			return 1;
		}
	}
	return 0;
}