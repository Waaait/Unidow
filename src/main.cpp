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
//#include "graphics/models/item.hpp"
#include "graphics/objects/model.h"

#include "io/keyboard.h"
#include "io/camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

glm::mat4 transform = glm::mat4(1.0f);

float x, y, z;
float lightPosX, lightPosY, lightPosZ;

Camera Camera::defaultCamera(glm::vec3(0.0f, 0.0f, 0.0f));

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
	//Shader shader("assets/vertex_core.glsl", "assets/fragment_core.glsl");
	Shader shader("assets/object.vs", "assets/object.fs");
	Shader lampShader("assets/object.vs", "assets/lamp.fs");

	// MODELS
	Cube model(Material::red_plastic, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
	model.init();

	
	lightPosX = -1.0f;
	lightPosY = -.5f;
	lightPosZ = .5f;

	Lamp lamp(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(lightPosX, lightPosY, lightPosZ), glm::vec3(0.25f));
	lamp.init();

	// LIGHTS
	// 
	//glm::vec3 pointLightPositions[] = {
	//	glm::vec3(0.7f,  0.2f,  2.0f),
	//	glm::vec3(2.3f, -3.3f, -4.0f)/*,
	//	glm::vec3(-4.0f,  2.0f, -12.0f),
	//	glm::vec3(0.0f,  0.0f, -3.0f)*/
	//};
	//Lamp lamps[4];
	/*const int lampNum = 2;
	Lamp lamps[lampNum];
	for (unsigned int i = 0; i < lampNum; i++) {
		lamps[i] = Lamp(glm::vec3(1.0f),
			glm::vec4(0.05f, 0.05f, 0.05f, 1.0f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), glm::vec4(1.0f),
			1.0f, 0.07f, 0.032f,
			pointLightPositions[i], glm::vec3(0.25f));
		lamps[i].init();
	}*/

	/*SpotLight s = {
		Camera::defaultCamera.cameraPos, Camera::defaultCamera.cameraFront,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)),
		1.0f, 0.07f, 0.032f,
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f)
	};*/

	// TEXTURES_____________________________________
	

	x = 0.0f;
	y = 0.0f;
	z = 5.0f;

	while (!glfwWindowShouldClose(window)) {
		// process input
		processInput(window);

		// render
		glClearColor(.0f, .0f, .0f, 0.1f);
		//glClearColor(0.8f, 0.8f, 0.7f, 0.1f);
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
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		view = glm::translate(view, glm::vec3(-x, -y, -z));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		model.render(shader, 2.4f);

		lampShader.activate();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		lamp.render(lampShader,glm::vec3(lightPosX, lightPosY, lightPosZ));
		/*for (unsigned int i = 0; i < lampNum; i++) {
			lamps[i].render(lampShader);
		}*/


		// send new frame to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	model.cleanup();
	lamp.cleanup();
	//cube.cleanup();
	/*for (unsigned int i = 0; i < lampNum; i++) {
		lamps[i].cleanup();
	}*/
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

void processInput(GLFWwindow* window) {
	if (Keyboard::key(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}

	// move obj
	if (Keyboard::keyWentDown(GLFW_KEY_A)) {
		//transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		lightPosX -= 0.5;
		printf("light pos X %f\n", lightPosX);
	}
	if (Keyboard::keyWentDown(GLFW_KEY_D)) {
		//transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		lightPosX += 0.5;
		printf("light pos X %f\n", lightPosX);
	}
	if (Keyboard::keyWentDown(GLFW_KEY_W)) {
		//transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightPosY += 0.5;
		printf("light pos Y %f\n", lightPosY);
	}
	if (Keyboard::keyWentDown(GLFW_KEY_S)) {
		//transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightPosY -= 0.5;
		printf("light pos Y %f\n", lightPosY);
	}
}