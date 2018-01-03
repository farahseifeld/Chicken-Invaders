#ifndef Beast_h
#define Beast_h
#include <string>
#include "SFML\Graphics.hpp"
#include "Chicken.h"
using namespace sf;

class Beast : Chicken
{

	Beast() {
		CH.loadFromFile("Chicken1.png");
		CHshape.setTexture(CH);
		CHshape.setScale(2, 2);
	}
};
#endif