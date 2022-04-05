#include "planet.h"

// pic path -- rotation days -- distance to sun -- size (compare to Earth)
Planet Planet::Sun = { "assets/texture/sun.JPG",		  25, NULL, 1 };
Planet Planet::Mercury = { "assets/texture/mercury.JPG",  57, 0.39f, 1 };
Planet Planet::Venus = { "assets/texture/venus.JPG",	-243, 0.70f, 1 };		// clockwise
Planet Planet::Earth = { "assets/texture/earth.JPG",	   1, 1.00f, 1 };
Planet Planet::Mars = { "assets/texture/mars.JPG",		1.65, 1.50f, 1 };

Planet Planet::Jupiter = { "assets/texture/jupiter.JPG", 0.41, 5.20f, 1 };
Planet Planet::Saturn = { "assets/texture/saturn.JPG",	 0.43, 9.50f, 1 };
Planet Planet::Uranus = { "assets/texture/uranus.JPG",   -0.43, 19.8f, 1 };		// clockwise
Planet Planet::Neptune = { "assets/texture/neptune.JPG", 0.75, 30.10f, 1 };
Planet Planet::Pluto = { "assets/texture/pluto.JPG",     6.4, 39.50f, 1 };

Planet list[9];
void Planet::setListitem(int i, Planet p) {
	list[i] = p;
}

Planet Planet::getListitem(int i){
	return list[i];
}

char* Planet::getPath() {
	return (char*)path;
}