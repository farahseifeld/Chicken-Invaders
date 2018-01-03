#ifndef Bullets_h
#define Bullets_h
#include "SFML\Graphics.hpp"

class Bullets
{
private:
	sf::Texture AR;
	sf::Sprite Arrow;

public:
	Bullets();
	void setpos(float x, float y);
	void setrotation(float theta);
	int getrotation()const;
	sf::Sprite getArrow()const;
	~Bullets();


};

Bullets::Bullets()
{
	AR.loadFromFile("arrow.png");
	Arrow.setTexture(AR);
}

void Bullets::setrotation(float theta){
	Arrow.setRotation(theta);
	
}
void Bullets::setpos(float x, float y){
	Arrow.setPosition(x, y);
}
int Bullets::getrotation()const{
	return Arrow.getRotation();
}
sf::Sprite Bullets::getArrow()const{
	return Arrow;
}
Bullets::~Bullets()
{
}

#endif