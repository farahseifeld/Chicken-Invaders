#ifndef Rocks_h
#define Rocks_h
#include <string>
#include "SFML\Graphics.hpp"
using namespace sf;

class Rocks
{

private:
	Sprite Shape; 
	Texture rock;
	int Sizefactor;
	float Speedfactor;
	int phi;
	int HealthBar;

public:
	bool Dead = false;
	Rocks();
	~Rocks();
	Sprite getShape() const;
	float getSpeedFactor();
	void setpos(double x, double y);
	void setTheme();
	void minusHealth();
};

Rocks::Rocks()
{
	rock.loadFromFile("Rock4.png");
	Shape.setTexture(rock);
	if (rand() % 2 == 0) {
		Shape.setScale(1.4, 1.4);
		HealthBar = 5;
	}
	else HealthBar = 2;
	Speedfactor = ((rand() % 100) / 100.0)+0.2;
}

Rocks::~Rocks()
{
}

Sprite Rocks::getShape() const {
	return Shape;
}

float Rocks::getSpeedFactor() {
	return Speedfactor;
}

void Rocks::setpos(double x, double y) {
	Shape.setPosition(x, y);
}

void Rocks::setTheme() {
	rock.loadFromFile("coralrocks.png");
	Shape.setTexture(rock);
	Shape.setScale(0.3, 0.3);
	if (rand() % 2 == 0) {
		Shape.setScale(0.4, 0.4);
		HealthBar = 5;
	}
	else HealthBar = 2;
	Shape.setTextureRect(sf::IntRect(0, 0, rock.getSize().x, rock.getSize().y));
}

void Rocks::minusHealth() {
	if (HealthBar > 0)
		HealthBar--;
	else if(HealthBar==0)
		Dead = true;
}

#endif 