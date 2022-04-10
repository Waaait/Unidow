#ifndef PLANET_H
#define PLANET_H

#include <glm/glm.hpp>

struct Planet {
	const char* path;
	float selfRotationSpeed;
	float distance;
	float size;

	static Planet Sun;
	static Planet Mercury;
	static Planet Venus;
	static Planet Earth;
	static Planet Mars;

	static Planet Jupiter;
	static Planet Saturn;
	static Planet Uranus;
	static Planet Neptune;
	static Planet Pluto;


	static void setListitem(int i, Planet p);
	static Planet getListitem(int i);
	void genList(Planet sys);

	char* getPath();
};


#endif