#ifndef Pickups_h
#define Pickups_h
enum bonus { life, fire, immunity, meat,key };
#include "SFML\Graphics.hpp"

class Pickups
{
private:
	sf::Texture BON;
	bonus Name;
	sf::Sprite Shape;


public:
	Pickups();
	Pickups(bonus name);
	void setName(bonus name);
	void setShape(sf::Sprite shape);
	void setpos(float x, float y);
	bonus getName()const;
	sf::Sprite getShape()const;
	double getSpeedfactor();
	~Pickups();


};
Pickups::Pickups() {
	Name = bonus(rand() % 4);
	if (Name == life) BON.loadFromFile("heart.png");
	else if (Name == fire) BON.loadFromFile("gift.png");
	else if (Name == immunity) BON.loadFromFile("shield.png");
	else if (Name == meat) BON.loadFromFile("meat.png");
	Shape.setTexture(BON);
}
Pickups::Pickups(bonus name) {
	Name = name;
	if (Name == life) BON.loadFromFile("heart.png");
	else if (Name == fire) BON.loadFromFile("gift.png");
	else if (Name == immunity) BON.loadFromFile("shield.png");
	else if (Name == meat) BON.loadFromFile("meat.png");
	Shape.setTexture(BON);

}
void Pickups::setName(bonus name) {
	Name = name;
	if (Name == life) BON.loadFromFile("heart.png");
	else if (Name == fire) BON.loadFromFile("gift.png");
	else if (Name == immunity) BON.loadFromFile("shield.png");
	else if (Name == meat) BON.loadFromFile("meat.png");
	Shape.setTexture(BON);
	 if (Name == key) {
		BON.loadFromFile("key.png");
		Shape.setTexture(BON);
		Shape.setScale(0.3, 0.3);
	}
	
}
void Pickups::setShape(sf::Sprite shape) {
	Shape = shape;
}
bonus Pickups::getName()const {
	return Name;
}
sf::Sprite Pickups::getShape()const {
	return Shape;
}
void Pickups::setpos(float x, float y) {
	Shape.setPosition(x, y);
}

double Pickups::getSpeedfactor() {
	return (rand() % 20)/10.0 + 0.25;
}
Pickups::~Pickups()
{
}
#endif