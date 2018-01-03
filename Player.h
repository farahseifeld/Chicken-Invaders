#include <string>
#include <SFML\Graphics.hpp>
using namespace sf;
#ifndef Player_h
#define Player_h

class Player
{
private:
	sf::Texture SH;
	sf::Sprite Ship;
	std::string Name;
	int Lifes;
	int Level;
	int HealthBar;
	int ShootingSpeed;
	int Shots;
	int Immunity;
	int Score;
	sf::SoundBuffer d;
	sf::Sound die;
	sf::SoundBuffer o;
	sf::Sound gameover;


public:
	bool hasKey = false;
	bool hitbyegg;
	bool hitbychicken;
	sf::Clock deathtimer;
	bool Dead;
	Player();
	Player(std::string name);
	void setScore(int score);
	void setShip(Texture* ship, int r);
	void setName(std::string name);
	void setLifes(int lifes);
	void setLevel(int level);
	void setHelathBar(int healthbar);
	void setShootingSpeed(int shootingspeed);
	void setShots(int shots);
	void setpos(float x, float y);
	sf::Sprite getShip()const;
	std::string getName()const;
	int getScore()const;
	int getHealthBar()const;
	int getLifes()const;
	int getLevel()const;
	int getShootingSpeed()const;
	int getShots()const;
	void minusHealth();
	~Player();


};
Player::Player(){
	hitbyegg = false;
	SH.loadFromFile("spaceship.png");
	Ship.setTexture(SH);
	Name = "Player";
	HealthBar = 1;
	ShootingSpeed = 1;
	Shots = 1;
	Lifes = 3;
	Level = 0;
	Immunity = 0;
	Dead = false;
	Score = 0;
	d.loadFromFile("Ci1playerhit.oga");
	die.setBuffer(d);
	o.loadFromFile("Ci1gameover.oga");
	gameover.setBuffer(o);
}
Player::Player(std::string name){
	hitbyegg = false;
	hitbychicken = false;
	SH.loadFromFile("spaceship.png");
	Ship.setTexture(SH);
	Name = name;
	HealthBar = 1;
	ShootingSpeed = 1;
	Shots = 1;
	Lifes = 3;
	Level = 0;
	Immunity = 0;
	Dead = false;
	Score = 0;
	d.loadFromFile("Ci1playerhit.oga");
	die.setBuffer(d);
	o.loadFromFile("Ci1gameover.oga");
	gameover.setBuffer(o);
}
void Player::setScore(int score){
	Score = score;
}
void Player::setShip(Texture* ship,int r){
	SH = *ship;
	Ship.setTexture(SH);
	Ship.setTextureRect(sf::IntRect(0, 0, SH.getSize().x, SH.getSize().y));
	switch (r) {
	case 2:
		Ship.setScale(0.45, 0.45);
		break;
	case 4:
		Ship.setScale(0.25, 0.25);
		break;
	case 5:
		Ship.setScale(0.43, 0.43);
		break;
	case 6:
		Ship.setScale(1.1, 1.1);
		break;
	}
	
	
}
void Player::setName(std::string name){
	Name = name;
}
void Player::setHelathBar(int healthbar){
	HealthBar = healthbar;
}
void Player::setShootingSpeed(int shootingspeed){
	ShootingSpeed = shootingspeed;
}
void Player::setpos(float x, float y){
	Ship.setPosition(x, y);
}
void Player::setShots(int shots){
	if (shots<4)
	Shots = shots;
}
sf::Sprite Player::getShip()const{
	return Ship;
}
std::string Player::getName()const{
	return Name;
}
int Player::getScore()const{
	return Score;
}
int Player::getHealthBar()const{
	return HealthBar;
}
int Player::getShootingSpeed()const{
	return ShootingSpeed;
}
int Player::getShots()const{
	return Shots;
}
void Player::minusHealth(){
	if (HealthBar > 0) 
		HealthBar--;
	if (HealthBar == 0)
	{ Dead = true;
	setLifes(getLifes() - 1);
		hitbyegg = true;
	deathtimer.restart(); 
	die.play(); }

}
void Player::setLifes(int lifes){
	Lifes = lifes;
	if (Lifes <= 0)
	{ Dead = true; gameover.play(); }
	else Dead = false;
}
void Player::setLevel(int level){
	Level = level;
}
int Player::getLifes()const{
	return Lifes;
}
int Player::getLevel()const{
	return Level;
}
Player::~Player(){

}


#endif