#ifndef Eggs_h
#define Eggs_h 
#include "SFML\Graphics.hpp"
#include <SFML/Audio.hpp>

class Eggs
{
private:
	sf::Texture* EG;
	sf::Sprite Shape;
	float Speedfactor;
	bool Broken;
	sf::Clock Timer;
	sf::SoundBuffer b;
	sf::Sound broke;
	sf::Texture* broken_egg;

public:
	float getTimer() const;
	Eggs();
	void setBroken(bool broken);
	void setSpeedfactor(float speedfactor);
	void setpos(float x, float y);
	bool getBroken()const;
	sf::Sprite getShape()const;
	float getSpeedfactor()const;
	~Eggs();


};

Eggs::Eggs()
{
	Speedfactor = ((rand() % 51) / 100.0) + 1;
	Broken = false;
	b.loadFromFile("Ci1eggdestroy.oga");
	broke.setBuffer(b);

	EG = new sf::Texture();
	broken_egg = new sf::Texture();
	EG->loadFromFile("egg.png");
	broken_egg->loadFromFile("egg2.png");
	Shape.setTexture(*EG);
}

void Eggs::setSpeedfactor(float speedfactor){
	Speedfactor = speedfactor;
}
float Eggs::getTimer()const{
	return Timer.getElapsedTime().asSeconds();
}
void Eggs::setBroken(bool broken){
	Broken = broken;
	if (Broken)
	{	
		broke.play();
		Timer.restart();
		Shape.setTexture(*broken_egg);
		Shape.setTextureRect(sf::IntRect(0, 0, broken_egg->getSize().x, broken_egg->getSize().y));
	}
}
void Eggs::setpos(float x, float y){
	Shape.setPosition(x, y);
}
bool Eggs::getBroken()const{
	return Broken;
}
sf::Sprite Eggs::getShape()const{
	return Shape;
}
float Eggs::getSpeedfactor()const{
	return Speedfactor;
}

Eggs::~Eggs()
{
}
#endif 
