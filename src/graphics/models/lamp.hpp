#ifndef LAMP_HPP
#define LAMP_HPP

#include "cube.hpp"
#include "../rendering/material.h"
#include "../rendering/shader.h"
#include "../rendering/light.h"

#include <glm/glm.hpp>

class Lamp : public Cube {
public:
	glm::vec3 lightColor;

	// light strength values
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Lamp(glm::vec3 lightColor = glm::vec3(1.0f),
		glm::vec3 ambient = glm::vec3(1.0f),
		glm::vec3 diffuse = glm::vec3(1.0f),
		glm::vec3 specular = glm::vec3(1.0f),
		glm::vec3 pos = glm::vec3(0.0f),
		glm::vec3 size = glm::vec3(1.0f))
		: lightColor(lightColor),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		Cube((char*)"assets/texture/cubeblue.jpg",glm::vec3(1.0f), Material::jade, pos, size) {}

	void render(Shader shader, glm::vec3 pos) {
		// set light color
		shader.set3Float("lightColor", lightColor);
		this->pos = pos;

		Cube::render(shader, 1.0f);
	}
};

#endif