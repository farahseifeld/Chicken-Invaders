#include <SFML\Graphics.hpp>
#include <cmath>
using namespace sf;
#ifndef Chicken_h
#define Chicken_h


class Chicken
{
public:
	sf::Texture CH;
	sf::Sprite CHShape;
	int Level;
	int HealthBar;

public:
	bool hasKey = false;
	bool descending;
	bool justdead;
	bool Dead;
	Chicken();
	Chicken(int level);
	~Chicken();
	void minusHealth();
	void setHealthBar(int healthbar);
	void setCHShape(sf::Sprite chshape);
	void setLevel(int level,bool underwater);
	void setpos(float x, float y);
	sf::Sprite getCHShape()const;
	int getLevel()const;
	int getHealthBar()const;
	void setTheme();
	void levelelwa7sh(int level);
	void setDescending(int level);
	double getSpeedFactor();
	Vector2f normalize(const Vector2f & source);
	double getSpeedFactorBeast();
	void beastminushealth();
};

Chicken::Chicken()
{
	descending = false;
	justdead = false;
	CH.loadFromFile("chicken1.png");
	CHShape.setTexture(CH);
	CHShape.setScale(0.75, 0.75);
	Level = 1;
	HealthBar = Level;
	Dead = false;
}
Chicken::Chicken(int level) {
	Level = level;
	if (Level == 1)	CH.loadFromFile("chicken1.png");
	//originally chicken2
	if (Level == 2)	CH.loadFromFile("chicken2.png");
	if (Level == 3)	CH.loadFromFile("chicken3.png");
	CHShape.setTexture(CH);
	CHShape.setScale(0.75,0.75);
	HealthBar = Level;
	Dead = false;
}
void Chicken::minusHealth() {
	if (HealthBar >= 1) HealthBar--;
	if (HealthBar == 0) { Dead = true; justdead = true; }
	else if (HealthBar == 1) {
		CH.loadFromFile("chicken1.png");
		CHShape.setTextureRect(sf::IntRect(0, 0, CH.getSize().x, CH.getSize().y));
	}
	else if (HealthBar == 2)	CH.loadFromFile("chicken2.png");
	else if (HealthBar == 3)	CH.loadFromFile("chicken3.png");

}
void Chicken::setHealthBar(int healthbar) {
	HealthBar = healthbar;
	if (HealthBar == 0) { Dead = true; justdead = true; }
	else if (HealthBar == 1)	
		CH.loadFromFile("chicken1.png");
	else if (HealthBar == 2)	CH.loadFromFile("chicken2.png");
	else if (HealthBar == 3)	CH.loadFromFile("chicken3.png");
}
void Chicken::setCHShape(sf::Sprite chshape) {
	CHShape = chshape;
}
void Chicken::setLevel(int level, bool underwater) {
	Level = level;
	if (Level == 1)	CH.loadFromFile("chicken1.png");
	else if (Level == 2)
		CH.loadFromFile("chicken2.png");
	else if (Level == 3)
		if(!underwater)
		CH.loadFromFile("chicken3.png");
	else
		{
			CH.loadFromFile("chickenwater.png");
			CHShape.setTexture(CH);
			CHShape.setScale(0.6, 0.6);
			CHShape.setTextureRect(sf::IntRect(0, 0, CH.getSize().x, CH.getSize().y));
		}
	
	else if (Level == 4)
		CH.loadFromFile("chicken4.png");

	else if (Level == 5) {
		if (!underwater)
			CH.loadFromFile("chicken5.png");
		else
		{
			CH.loadFromFile("chickenwater.png");
			CHShape.setTexture(CH);
			CHShape.setScale(0.6, 0.6);
			CHShape.setTextureRect(sf::IntRect(0, 0, CH.getSize().x, CH.getSize().y));
		}
	}
	HealthBar = Level;
}
sf::Sprite Chicken::getCHShape()const {
	return CHShape;
}
int Chicken::getLevel()const {
	return Level;
}
int Chicken::getHealthBar()const {
	return HealthBar;
}
void Chicken::setpos(float x, float y) {
	CHShape.setPosition(x, y);
}

void Chicken::setTheme() {
	if (Level == 1) {
		CH.loadFromFile("chickenwater.png");
		CHShape.setTexture(CH);
		CHShape.setScale(0.6, 0.6);
		
	}
	else if (Level == 2) {
		CH.loadFromFile("chickenwater.png");
		CHShape.setTexture(CH);
		CHShape.setScale(0.6, 0.6);
	}
	if (Level == 3) {
		CH.loadFromFile("chickenwater.png");
		CHShape.setTexture(CH);
		CHShape.setScale(0.6, 0.6);

	}
	if (HealthBar == 1)
		CH.loadFromFile("chickenwater.png");

	CHShape.setTextureRect(sf::IntRect(0, 0, CH.getSize().x, CH.getSize().y));
}
Chicken::~Chicken()
{
}

void Chicken::levelelwa7sh(int level) {
	CH.loadFromFile("please.png");
	CHShape.setTextureRect(sf::IntRect(0, 0, CH.getSize().x, CH.getSize().y));
 CHShape.setScale(1.3, 1.3);
 HealthBar = 20 * level;
}

double Chicken::getSpeedFactor() {
	return (rand() % 4 / 10.0)+0.17;
}

void Chicken::setDescending(int level) {
	CH.loadFromFile("chickendescending_burned.png");
	CHShape.setTextureRect(sf::IntRect(0, 0, CH.getSize().x, CH.getSize().y));
	HealthBar = level+1;
	descending = true;
}

Vector2f Chicken:: normalize(const Vector2f& source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return Vector2f(source.x / length, source.y / length);
	else
		return source;
}

double Chicken::getSpeedFactorBeast() {
	return (rand() % 100) + 50;
}

void Chicken::beastminushealth() {
	HealthBar--;
	if (HealthBar <= 0)
		Dead = true;
}

#endif 