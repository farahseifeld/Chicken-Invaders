#include <SFML/Audio.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include "Chicken.h"
#include "Player.h"
#include "Bullets.h"
#include "Eggs.h"
#include "Pickups.h"
#include "Menu.h"
#include "Rocks.h"
#include <vector>
#include <fstream>
#include <string>
using namespace std;
#define number 5
bool gameover = false;
int cnt = 0;
bool firstmenu = true;
bool shielded = true;
bool movesleft = true;
bool playing = true;
bool muted = false;
bool inOptions = false;
bool insound = false;
bool incontrols = false;
bool keyboardf = true;
bool mousef = false;
bool inlevel[12] = { false };
bool inmain = true;
bool pos1 = false, pos2 = false, pos3 = true;
bool gamejustopened = true;
bool scoretaken = false;
sf::Clock habaltimer;
sf::Clock transittimer;
Pickups BO[40];
int n = 0;
vector <Eggs> E;
Eggs egg;
vector<Bullets> B;
Bullets bullet;
Rocks R[1000];

void rocksOnScreen(Rocks R[]) {
	for (int i = 0;i < 77;i++)
		if (R[i].getShape().getPosition().x > 700 || R[i].getShape().getPosition().y > 780)
			R[i].Dead = true;
}

bool rocksOver(Rocks R[]) {
	for (int i = 0;i < 77;i++)
		if (!R[i].Dead)
			return false;
	return true;
}

int countAliveChickens(Chicken round1[][5]) {
	int cnt = 0;
	for (int i = 0;i < number;i++)
		for (int j = 0;j < number;j++)
			if (!round1[i][j].Dead)
				cnt++;
	return cnt;
}

void chickenDescendingOnScreen(Chicken a[], Chicken b[], Chicken c[]) {

	for (int i = 0;i < 20;i++) {
		if (!a[i].Dead)
			if (a[i].getCHShape().getPosition().y > 770)
				a[i].Dead = true;
		if (i < 9) {
			if (!b[i].Dead)
				if (b[i].getCHShape().getPosition().y > 770)
					b[i].Dead = true;
		}
		if (i < 5) {
			if (!c[i].Dead)
				if (c[i].getCHShape().getPosition().y > 770)
					c[i].Dead = true;
		}
	}
}

bool chickensDescendingOver(Chicken a[], Chicken b[], Chicken c[]) {
	for (int i = 0;i < 20;i++) {
		if (!a[i].Dead)
			return false;
		if (i < 9)
			if (!b[i].Dead)
				return false;
		if (i < 5)
			if (!c[i].Dead)
				return false;
	}
	return true;
}

void handleChickenDescendingWave(Chicken a[], Chicken b[], Chicken c[], Player &player, Clock &descendingtimer,int level);
void pickupsChickenDescending(Chicken a[], Chicken b[], Chicken c[], Player & player);
void afterhittingdescending(Chicken a[], Chicken b[], Chicken c[], Player & player,int level);
void moveAround(Chicken & a, Player & player);
void hittingChickenDescending(Chicken a[], Chicken b[], Chicken c[], Player &player);
void dealwithChickens(Chicken a[], Chicken b[], Chicken c[], int i, int ib, int ic,  int level);
bool descendingChickensAlive(Chicken a[], int s, int e);
void descendingChickensMoving(Chicken &a, int level);
void descendingChickensInitial(Chicken a[], Chicken b[], Chicken c[], int size, int level);
void movingRocks(Rocks R[], Clock & rocktimer, int &cnt);
void startWave2(int& cnt, Clock& rocktimer, Clock& wave2);
void rockWave(Rocks R[], int s, int e);
void beastMovement(Chicken & beast, sf::Clock & timer3);
void handlechicken(Player& player, Chicken round1[][number], sf::Sound& laid, int level);
void handleplayer(Player& player, sf::RenderWindow& App, sf::Clock& shieldtimer, sf::Sound& eat, sf::Sprite& PB);
void shooting(Player& player, sf::Clock& timer, sf::Sound& shoot);
void movingship(Player& player, sf::RenderWindow& App);
void handlemain(sf::RenderWindow& App, Player& player, sf::Event& event);
void handleround0(int level, sf::RenderWindow& App, Player& player);
void handleround1(int level, sf::RenderWindow& App, Player& player);
void handleround2(int level, sf::RenderWindow& App, Player& player);
void handleround3(int level, sf::RenderWindow& App, Player& player);
bool round0done(Chicken round1[][number]);
void handlecontrols(Menu& menu, sf::RenderWindow& App, sf::Event& event);
void handlesound(Menu& menu, sf::RenderWindow& App, sf::Event& event);
void handleoptions(Menu& options, sf::RenderWindow& App, sf::Event& event);
void handlemenu(Menu& menu, sf::RenderWindow& App, sf::Event& event, Sound &menumusic);
void loadplayer(sf::RenderWindow& App, vector<sf::Text>& players);
string fromKtoS(const sf::Keyboard::Key& k);
void addplayer(string name);
void deleteplayer(int z, int size);
void chooseplayer(Player& player, int pos);
void updatesheet(Player& player);
void addScore2(string name, int score);
void readscores(sf::RenderWindow& App, vector<sf::Text>& players);
void handleStore(Player&player);

int main()
{
	sf::Font font;
	font.loadFromFile("futureforcescondital.ttf");
	sf::Text loading;
	loading.setFont(font);
	loading.setFillColor(sf::Color::White);
	loading.setPosition(230, 300);
	loading.setCharacterSize(80);
	SoundBuffer mm;
	Sound menumusic;
	mm.loadFromFile("Ci1maintheme.oga");
	menumusic.setBuffer(mm);

	Clock load;
	load.restart();
	SoundBuffer logo;
	Sound start;
	logo.loadFromFile("Ci1logo.oga");
	start.setBuffer(logo);



	sf::RenderWindow App(sf::VideoMode(200 + number * 120, 300 + 100 * number, 32), "CHICKEN INVADERS");
	Player player("koko");
	inmain = true;
	while (App.isOpen())
	{	start.play();
		if (load.getElapsedTime().asSeconds() < 4) {
			
			loading.setString("Loading.");
			App.clear();
			App.draw(loading);
		
			App.display();
			App.clear();
			loading.setString("Loading...");
			App.draw(loading);
				App.display();
		}
		sf::Event event;
		while (App.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				App.close();
		}
		while (App.isOpen() && inmain && load.getElapsedTime().asSeconds() > 3)
		{
			menumusic.play();
		handlemain(App, player, event); 
		for (int i = 0; i < 12; i++)
			inlevel[i] = false; 
		inlevel[player.getLevel()] = true; 
		}
		menumusic.stop();
		for (int i = 0; i < 12; i++)
		{	
		
			if (inlevel[i] && i % 4 == 0 && !player.Dead &&!inmain) 
				handleround0(i / 4 + 1, App, player);
		
			if (inlevel[i] && i % 4 == 1 && !player.Dead && !inmain)
				handleround1(i / 4 + 1, App, player);
			
		
			if (inlevel[i] && i % 4 == 2 && !player.Dead && !inmain)
				handleround2(i / 4 + 1, App, player);
			
		
			if (inlevel[i] && i % 4 == 3 && !player.Dead && !inmain)
				handleround3(i / 4 + 1, App, player);
			
		}
	}
	//mabrook you win the game heeeh closure function
	App.close();


	return 0;
}

void handleround0(int level, sf::RenderWindow& App, Player& player)
{
	Menu menu(App.getSize().x, App.getSize().y);
	srand(unsigned(time(NULL)));
	bool started = false;
	//intialize some sounds
	sf::SoundBuffer sh;
	sh.loadFromFile("Ionblaster.oga");
	sf::Sound shoot;
	shoot.setBuffer(sh);
	shoot.setVolume(50);
	sf::SoundBuffer l;
	sf::Sound laid;
	l.loadFromFile("Ci1egglay.oga");
	laid.setBuffer(l);
	sf::SoundBuffer e;
	sf::Sound eat;
	e.loadFromFile("Ci1foodeat.oga");
	eat.setBuffer(e);
	SoundBuffer mm;
	Sound menumusic;
	mm.loadFromFile("Ci1maintheme.oga");
	menumusic.setBuffer(mm);

	//initialize player pos
	player.setpos(350, 680);
	//some timers
	sf::Clock timer;
	timer.restart();
	shielded = true;
	sf::Clock shieldtimer;
	shieldtimer.restart();
	sf::Clock starttimer;
	starttimer.restart();
	//initialize background
	sf::Texture bg;
	sf::Sprite background;
	if (level == 3) {
		bg.loadFromFile("coral.jpg");
		background.setTexture(bg);
	}
	else if(level==1){
		bg.loadFromFile("space2.png");
		background.setTexture(bg);
		background.setScale(1.4, 1.4);
	}
	else if (level == 2) {
		bg.loadFromFile("sky.jpg");
		background.setTexture(bg);
		background.setScale(1.35, 1.35);
	}
	//shield
	sf::CircleShape shield(80);
	shield.setFillColor(sf::Color::Transparent);
	shield.setOutlineThickness(10);
	//hearts on top texture/sprite
	sf::Texture LI;
	LI.loadFromFile("heart.png");
	sf::Sprite Life;
	Life.setTexture(LI);
	Life.setPosition(5, 15);
	//pause button
	sf::Texture P;
	P.loadFromFile("pausebutton.png");
	sf::Sprite PB;
	PB.setTexture(P);
	PB.setPosition(App.getSize().x - 60, 0);

	//arial font
	sf::Font font;
	font.loadFromFile("futureforcescondital.ttf");
	sf::Text lives;
	lives.setFont(font);
	lives.setFillColor(sf::Color::White);
	lives.setPosition(55, -15);
	lives.setCharacterSize(70);
	//intialize score
	sf::Text score;
	score.setFont(font);
	score.setFillColor(sf::Color::White);
	score.setPosition(App.getSize().x / 2 - 100, 0);
	score.setCharacterSize(50);
	//conclusion
	sf::Text conclusion;
	conclusion.setFont(font);
	conclusion.setFillColor(sf::Color::White);
	conclusion.setPosition(150, 350);
	conclusion.setCharacterSize(80);
	//intialize intro text
	sf::Text intro;
	intro.setFont(font);
	intro.setFillColor(sf::Color::White);
	intro.setPosition(130, 250);
	intro.setCharacterSize(80);
	intro.setString("Level " + std::to_string(level) + " wave 1");
	//initialize chicken
	Chicken round1[number][number];
	for (int i = 0; i < number; i++){
		for (int j = 0; j < number; j++)
		{
			if (level == 3) {
				round1[i][0].setLevel(level + 1, true);
				round1[i][j].setpos(90 + 120 * i, 45 + 100 * j);
				if (j > 0)
					round1[i][j].setLevel(level, true);
				if (i == 1 && j == 3)
					round1[i][j].hasKey = true;
			}
			else {
				round1[i][0].setLevel(level + 1, false);
				round1[i][j].setpos(90 + 120 * i, 45 + 100 * j);
				if (j > 0)
					round1[i][j].setLevel(level, false);
			}

		}
	}

	n = 0;
	// Start game loop-----------------------------------------------------------------------------
	while (App.isOpen() && !inmain&&inlevel[(level - 1) * 4])
	{
		shield.setOrigin(-player.getShip().getPosition() + sf::Vector2f(30, 30));
		shield.setOutlineColor(sf::Color(100 * std::sin(shieldtimer.getElapsedTime().asSeconds()), 100 * std::cos(shieldtimer.getElapsedTime().asSeconds()), 100 + 100 * std::sin(shieldtimer.getElapsedTime().asSeconds())));
		shield.setRadius(80 + 10 * std::sin(shieldtimer.getElapsedTime().asSeconds()));
		lives.setString(std::to_string(player.getLifes()));
		score.setString("Score: " + std::to_string(player.getScore()));
		if (shieldtimer.getElapsedTime().asSeconds() > 10)shielded = false;
		if (!player.Dead && player.hitbyegg && player.deathtimer.getElapsedTime().asSeconds() >= 2){ player.hitbyegg = false; player.setpos(350, 680); shielded = true; shieldtimer.restart(); }
		if (player.Dead&&!scoretaken){  conclusion.setPosition(150, 350); conclusion.setString("YOU LOSE :P"); }
		if (starttimer.getElapsedTime().asSeconds() >= 3){ started = true; }

		if (!inlevel[(level - 1) * 4 + 1] && round0done(round1))
		{
			conclusion.setPosition(150, 250);
			conclusion.setString("YOU WIN :D\nNOW ROCKS");
			player.setLevel(player.getLevel() + 1);
			transittimer.restart(); inlevel[(level - 1) * 4 + 1] = true;
			if (!player.Dead) {
				updatesheet(player);
			}
		}
		if (player.Dead) {

			addScore2(player.getName(), player.getScore());
			player.setLevel(0);
			player.setLifes(3);
			player.setShots(1);
			player.setScore(0);
			inmain = true;
		}
		
		
		if (inlevel[(level - 1) * 4 + 1] && transittimer.getElapsedTime().asSeconds() >= 3){ inlevel[(level - 1) * 4] = false; }

		if (player.Dead) {
			//inmain = true;
		}
		n = n % 40;
		sf::Event event;
		if (!playing){
			handlemenu(menu, App, event,menumusic); 
		if (firstmenu)
			starttimer.restart(); 
		}
		else
		{
			while (App.pollEvent(event))
			if (event.type == sf::Event::Closed)App.close();
			movingship(player, App);
			shooting(player, timer, shoot);
			if (started)
			{
				handleplayer(player, App, shieldtimer, eat, PB);
				handlechicken(player, round1, laid,level);
				for (int i = 0;i<number;i++)
					for (int j = 0;j<number;j++)
						if (!shielded && !player.Dead && !player.hitbyegg && !round1[i][j].Dead && player.getShip().getGlobalBounds().intersects(round1[i][j].getCHShape().getGlobalBounds()))
							player.minusHealth();
			}
		}
		// Clear the screen (fill it with black color)
		App.clear();

		if (!playing)
		{ menu.draw(App); }

		else{
			App.draw(background);
			if (!started)
			{ App.draw(intro); }

			else{
				App.draw(Life);
				App.draw(lives);
				App.draw(score);
				App.draw(PB);
				for (int i = 0; i < number; i++)
				{
					for (int j = 0; j < number; j++)
					{
						if (round1[i][j].Dead == false &&!player.Dead)
							App.draw(round1[i][j].getCHShape());
					}
				}
				if (E.size()>0 && !player.Dead){
					for (int i = 0; i < E.size(); i++)
					{
						if (E[i].getBroken() == false){
							E[i].setpos(E[i].getShape().getPosition().x, E[i].getShape().getPosition().y + (0.3*E[i].getSpeedfactor()));
							App.draw(E[i].getShape());
						}

						if (E[i].getShape().getPosition().y >= App.getSize().y - 60 && E[i].getBroken() == false){
							E[i].setBroken(true);
						}
						if (E[i].getTimer()<0.5)App.draw(E[i].getShape());
						if (E[i].getTimer()>0.6 && E[i].getBroken()){ E.erase(E.begin() + i); }
					}
				}
				if (n > 0&&!player.Dead)
					for (int i = 0; i < n; i++)
				{
					BO[i].setpos(BO[i].getShape().getPosition().x, BO[i].getShape().getPosition().y + 0.25);
					App.draw(BO[i].getShape());
				}
			}
			if (!player.Dead && !player.hitbyegg)
				App.draw(player.getShip());
			if (player.Dead )
				//|| round0done(round1))
				App.draw(conclusion);

			if (shielded&&!player.Dead)
				App.draw(shield);
			if (B.size()>0&&!player.Dead) 
				for (int i = 0; i < B.size(); i++)App.draw(B[i].getArrow());
		}

		App.display();
	}

	return;

}
void handlecontrols(Menu& menu, sf::RenderWindow& App, sf::Event& event){
	while (!playing&&App.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			App.close();
			break;
		case sf::Event::KeyReleased:
			switch (event.key.code)
			{
			case sf::Keyboard::Up:
				menu.MoveUp();
				break;

			case sf::Keyboard::Down:
				menu.MoveDown();
				break;

			case sf::Keyboard::Return:
				switch (menu.GetPressedItem())
				{
				case 0:
					mousef = true;
					keyboardf = false;
					break;
				case 1:
					keyboardf = true;
					mousef = false;
					break;
				case 2:
					incontrols = false;
					break;
				}

				break;
			}

			break;

		}
	}
}
void handlesound(Menu& menu, sf::RenderWindow& App, sf::Event& event){
	while (!playing&&App.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			App.close();
			break;
		case sf::Event::KeyReleased:
			switch (event.key.code)
			{
			case sf::Keyboard::Up:
				menu.MoveUp();
				break;

			case sf::Keyboard::Down:
				menu.MoveDown();
				break;

			case sf::Keyboard::Return:
				switch (menu.GetPressedItem())
				{
				case 0:
					muted = true;
					break;
				case 1:
					//soundfactor = 0.5; //between 0&1
					break;
				case 2:
					insound = false;
					break;
				}

				break;
			}

			break;

		}
	}
}
void handleoptions(Menu& options, sf::RenderWindow& App, sf::Event& event){
	Menu sound(App.getSize().x, App.getSize().y, "mute", "50%", "Back");
	Menu controls(App.getSize().x, App.getSize().y, "mouse", "keyboard", "Back");

	while (!playing&&App.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			App.close();
			break;
		case sf::Event::KeyReleased:
			switch (event.key.code)
			{
			case sf::Keyboard::Up:
				options.MoveUp();
				break;

			case sf::Keyboard::Down:
				options.MoveDown();
				break;


			case sf::Keyboard::Return:
				switch (options.GetPressedItem())
				{
				case 0:
					insound = true;
					while (App.isOpen() && insound)
					{
						handlesound(sound, App, event);
						App.clear();
						sound.draw(App);
						App.display();
					}
					break;
				case 1:
					incontrols = true;
					while (App.isOpen() && incontrols)
					{
						handlecontrols(controls, App, event);
						App.clear();
						controls.draw(App);
						App.display();
					}
					break;
				case 2:
					inOptions = false;
					break;
				}

				break;
			}

			break;

		}
	}
}
void handlemenu(Menu& menu, sf::RenderWindow& App, sf::Event& event, Sound &menumusic){
	Menu options(App.getSize().x, App.getSize().y, "Sound", "Controls", "Back");
	menumusic.play();
	while (!playing&&App.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			App.close();
			break;
		case sf::Event::KeyReleased:
			switch (event.key.code)
			{
			case sf::Keyboard::Up:
				menu.MoveUp();
				break;

			case sf::Keyboard::Down:
				menu.MoveDown();
				break;

			case sf::Keyboard::Return:
				switch (menu.GetPressedItem())
				{
				case 0:
					playing = true;
					firstmenu = false;
					break;
				case 1:
					inOptions = true;
					while (App.isOpen() && inOptions)
					{
						handleoptions(options, App, event);
						App.clear();
						options.draw(App);
						App.display();
					}
					break;
				case 2:
					sf::Font font;
					font.loadFromFile("arial.ttf");
					bool agreed = false;
					sf::Text yes;
					yes.setFont(font);
					yes.setCharacterSize(25);
					yes.setString("Yes");
					sf::Text no;
					no.setFont(font);
					no.setCharacterSize(25);
					no.setString("No");
					sf::Text confirm;
					confirm.setFont(font);
					confirm.setCharacterSize(25);
					confirm.setString("Are you sure you want to continue?");
					confirm.setPosition(0, 0);
					sf::RenderWindow conmsg(sf::VideoMode(confirm.getCharacterSize()*confirm.getString().getSize() / 2, confirm.getCharacterSize() * 4, 32), "Delete confirmation");
					yes.setPosition(conmsg.getSize().x / 2 + 30, conmsg.getSize().y - 50);
					no.setPosition(conmsg.getSize().x / 2 - 30, conmsg.getSize().y - 50);
					while (conmsg.isOpen())
					{
						sf::Event event2;
						while (conmsg.pollEvent(event2))
						{
							if ((event2.type == sf::Event::Closed) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))))
								conmsg.close();
						}
						yes.setFillColor(sf::Color::White);
						no.setFillColor(sf::Color::White);
						if (yes.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(conmsg))))
						{
							yes.setFillColor(sf::Color::Red);
						}
						if (no.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(conmsg))))
						{
							no.setFillColor(sf::Color::Red);
						}
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
							if (yes.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(conmsg))))
							{
								agreed = true;
								conmsg.close();
							}
							if (no.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(conmsg))))
							{
								agreed = false;
								conmsg.close();
							}
						}
						conmsg.clear(sf::Color(169, 169, 169));
						conmsg.draw(confirm);
						conmsg.draw(yes);
						conmsg.draw(no);
						conmsg.display();
					}
					if (agreed)
					{
						inmain = true;
					}break;
				}

				break;
			}

			break;

		}
	}
}
bool round0done(Chicken round1[][number]){
	for (int i = 0; i < number; i++)
	for (int j = 0; j < number; j++){
		if (round1[i][j].Dead == false)return false;
	}
	return true;
}
void handleround1(int level, sf::RenderWindow& App, Player& player){
	Menu menu(App.getSize().x, App.getSize().y);
	srand(unsigned(time(NULL)));
	bool started = false;
	//intialize some sounds
	sf::SoundBuffer sh;
	sh.loadFromFile("Ionblaster.oga");
	sf::Sound shoot;
	shoot.setBuffer(sh);
	shoot.setVolume(50);
	//add your sounds here
	/*sf::SoundBuffer l;
	sf::Sound laid;
	l.loadFromFile("Ci1egglay.oga");
	laid.setBuffer(l);
	*/sf::SoundBuffer e;
	sf::Sound eat;
	e.loadFromFile("Ci1foodeat.oga");
	eat.setBuffer(e);
	SoundBuffer mm;
	Sound menumusic;
	mm.loadFromFile("Ci1maintheme.oga");
	menumusic.setBuffer(mm);
	//initialize player pos
	player.setpos(350, 680);
	//some timers
	sf::Clock timer;
	timer.restart();
	shielded = true;
	sf::Clock shieldtimer;
	shieldtimer.restart();
	sf::Clock starttimer;
	starttimer.restart();
	//initialize background
	sf::Texture bg;
	sf::Sprite background;
	if (level == 3) {
		bg.loadFromFile("coral.jpg");
		background.setTexture(bg);
	}
	else if(level==1){
		bg.loadFromFile("space2.png");
		background.setTexture(bg);
		background.setScale(1.4, 1.4);
	}
	else if (level == 2) {
		bg.loadFromFile("sky.jpg");
		background.setTexture(bg);
		background.setScale(1.35, 1.35);
	}
	//shield
	sf::CircleShape shield(80);
	shield.setFillColor(sf::Color::Transparent);
	shield.setOutlineThickness(10);
	//hearts on top texture/sprite
	sf::Texture LI;
	LI.loadFromFile("heart.png");
	sf::Sprite Life;
	Life.setTexture(LI);
	Life.setPosition(5, 15);
	//pause button
	sf::Texture P;
	P.loadFromFile("pausebutton.png");
	sf::Sprite PB;
	PB.setTexture(P);
	PB.setPosition(App.getSize().x - 60, 0);

	//arial font
	sf::Font font;
	font.loadFromFile("futureforcescondital.ttf");
	sf::Text lives;
	lives.setFont(font);
	lives.setFillColor(sf::Color::White);
	lives.setPosition(55, -15);
	lives.setCharacterSize(70);
	//intialize score
	sf::Text score;
	score.setFont(font);
	score.setFillColor(sf::Color::White);
	score.setPosition(App.getSize().x / 2 - 100, 0);
	score.setCharacterSize(50);
	//conclusion
	sf::Text conclusion;
	conclusion.setFont(font);
	conclusion.setFillColor(sf::Color::White);
	conclusion.setPosition(150, 350);
	conclusion.setCharacterSize(80);
	//intialize intro text
	sf::Text intro;
	intro.setFont(font);
	intro.setFillColor(sf::Color::White);
	intro.setPosition(130, 250);
	intro.setCharacterSize(80);
	intro.setString("Level " + std::to_string(level) + " wave 2");
	//initialize rocks
	cnt = 0;
	R[0].setpos(-100, -100);
	Clock rocktimer;
	Clock wave2;
	startWave2(cnt, rocktimer, wave2);
	if (level == 3)
		for (int i = 0;i < 500;i++)
			R[i].setTheme();

	wave2.restart();

	n = 0;
	// Start game loop-----------------------------------------------------------------------------
	while (App.isOpen() && !inmain&&inlevel[(level - 1) * 4 + 1])
	{
		shield.setOrigin(-player.getShip().getPosition() + sf::Vector2f(30, 30));
		shield.setOutlineColor(sf::Color(100 * std::sin(shieldtimer.getElapsedTime().asSeconds()), 100 * std::cos(shieldtimer.getElapsedTime().asSeconds()), 100 + 100 * std::sin(shieldtimer.getElapsedTime().asSeconds())));
		shield.setRadius(80 + 10 * std::sin(shieldtimer.getElapsedTime().asSeconds()));
		lives.setString(std::to_string(player.getLifes()));
		score.setString("Score: " + std::to_string(player.getScore()));
		if (shieldtimer.getElapsedTime().asSeconds() > 5)shielded = false;
		if (!player.Dead && player.hitbyegg && player.deathtimer.getElapsedTime().asSeconds() >= 2)
		{ player.hitbyegg = false; 
		player.setpos(350, 680); 
		shielded = true; 
		shieldtimer.restart(); }
		if (player.Dead) { conclusion.setPosition(150, 350); conclusion.setString("YOU LOSE :P"); }
		if (starttimer.getElapsedTime().asSeconds() >= 3)
		{ started = true; }

		if (!inlevel[(level - 1) * 4 + 2] && wave2.getElapsedTime().asSeconds() > 15) {
			conclusion.setPosition(100, 250);
			//	conclusion.setString("YOU WIN :D\nNOW Decsending chicken"); 
			player.setLevel(player.getLevel() + 1);
			transittimer.restart();
				inlevel[(level - 1) * 4 + 2] = true;

			if (!player.Dead) {
			
				updatesheet(player);
			}
		
		}
			if (player.Dead) {

				addScore2(player.getName(), player.getScore());
				player.setLevel(0);
				player.setLifes(3);
				player.setShots(1);
				player.setScore(0);
				inmain=true;
			}
		
		if (inlevel[(level - 1) * 4 + 2] && transittimer.getElapsedTime().asSeconds() >= 2)
		{ inlevel[(level - 1) * 4 + 1] = false; }
		if (player.Dead) {
			inmain = true;
		}

		n = n % 40;
		sf::Event event;
		if (!playing)
		{ handlemenu(menu, App, event,menumusic); 
		if (firstmenu)
			starttimer.restart(); }
		else
		{
			while (App.pollEvent(event))
			if (event.type == sf::Event::Closed)App.close();
			movingship(player, App);
			shooting(player, timer, shoot);
			if (started)
			{
				handleplayer(player, App, shieldtimer, eat, PB);
				
				if (wave2.getElapsedTime().asSeconds() < 15)
					movingRocks(R, rocktimer, cnt);

				for (int i = 0;i<cnt * 5 + 5;i++)
					if (!shielded && !player.Dead && !player.hitbyegg && !R[i].Dead && player.getShip().getGlobalBounds().intersects(R[i].getShape().getGlobalBounds()))
						player.minusHealth();

				for (int i = 0;i < cnt * 5 + 5;i++)
					for (int m = 0;m < B.size();m++)
						if (B[m].getArrow().getGlobalBounds().intersects(R[i].getShape().getGlobalBounds())) {
							R[i].minusHealth();
							B.erase(B.begin() + m);
						}
						else if (B.at(m).getArrow().getPosition().y < -2)
							B.erase(B.begin() + m);
			//	rocksOnScreen(R);
			}

		}
		// Clear the screen (fill it with black color)
		App.clear();

		if (!playing){ menu.draw(App); }

		else{
			App.draw(background);
			if (!started){ App.draw(intro); }
			else{

				App.draw(Life);
				App.draw(lives);
				App.draw(score);
				App.draw(PB);
				//draw your enemies here
				if (wave2.getElapsedTime().asSeconds()<15 && !player.Dead)
				for (int i = 0; i < 5 * cnt + 5; i++)
					if(!R[i].Dead&&!player.Dead)
					App.draw(R[i].getShape());

			}
			if (!player.Dead && !player.hitbyegg)
				App.draw(player.getShip());
			if (player.Dead)
				//|| wave2.getElapsedTime().asSeconds()>20)
				App.draw(conclusion);

			if (shielded && !player.Dead)
				App.draw(shield);
			if (B.size()>0 && !player.Dead) 
				for (int i = 0; i < B.size(); i++)
					App.draw(B[i].getArrow());
		}

		App.display();
	}

	return;


}
void handleround2(int level, sf::RenderWindow& App, Player& player)
{
	Menu menu(App.getSize().x, App.getSize().y);
	srand(unsigned(time(NULL)));
	bool started = false;
	//intialize some sounds
	sf::SoundBuffer sh;
	sh.loadFromFile("Ionblaster.oga");
	sf::Sound shoot;
	shoot.setBuffer(sh);
	shoot.setVolume(50);
	//add your sounds here
	sf::SoundBuffer l;
	sf::Sound laid;
	l.loadFromFile("Ci1egglay.oga");
	laid.setBuffer(l);
	sf::SoundBuffer e;
	sf::Sound eat;
	e.loadFromFile("Ci1foodeat.oga");
	eat.setBuffer(e);
	SoundBuffer mm;
	Sound menumusic;
	mm.loadFromFile("Ci1maintheme.oga");
	menumusic.setBuffer(mm);
	//initialize player pos
	player.setpos(350, 680);
	//some timers
	sf::Clock timer;
	timer.restart();
	shielded = true;
	sf::Clock shieldtimer;
	shieldtimer.restart();
	sf::Clock starttimer;
	starttimer.restart();
	//initialize background
	sf::Texture bg;
	sf::Sprite background;
	if (level == 3) {
		bg.loadFromFile("coral.jpg");
		background.setTexture(bg);
	}
	else if(level==1){
		bg.loadFromFile("space2.png");
		background.setTexture(bg);
		background.setScale(1.4, 1.4);
	}
	else if (level == 2) {
		bg.loadFromFile("sky.jpg");
		background.setTexture(bg);
		background.setScale(1.35, 1.35);
	}
	
	//shield
	sf::CircleShape shield(80);
	shield.setFillColor(sf::Color::Transparent);
	shield.setOutlineThickness(10);
	//hearts on top texture/sprite
	sf::Texture LI;
	LI.loadFromFile("heart.png");
	sf::Sprite Life;
	Life.setTexture(LI);
	Life.setPosition(5, 15);
	//pause button
	sf::Texture P;
	P.loadFromFile("pausebutton.png");
	sf::Sprite PB;
	PB.setTexture(P);
	PB.setPosition(App.getSize().x - 60, 0);

	//arial font
	sf::Font font;
	font.loadFromFile("futureforcescondital.ttf");
	sf::Text lives;
	lives.setFont(font);
	lives.setFillColor(sf::Color::White);
	lives.setPosition(55, -15);
	lives.setCharacterSize(70);
	//intialize score
	sf::Text score;
	score.setFont(font);
	score.setFillColor(sf::Color::White);
	score.setPosition(App.getSize().x / 2 - 100, 0);
	score.setCharacterSize(50);

	//conclusion
	sf::Text conclusion;
	conclusion.setFont(font);
	conclusion.setFillColor(sf::Color::White);
	conclusion.setPosition(150, 350);
	conclusion.setCharacterSize(80);

	//intialize intro text
	sf::Text intro;
	intro.setFont(font);
	intro.setFillColor(sf::Color::White);
	intro.setPosition(130, 250);
	intro.setCharacterSize(80);
	intro.setString("Level " + std::to_string(level) + " wave 3");

	//initialize your enemies here
	Chicken a[20];
	Chicken b[9];
	Chicken c[5];
	descendingChickensInitial(a, b, c, 20,level);
	Clock wave3;
	bool flag = true;
	n = 0;

	// Start game loop-----------------------------------------------------------------------------
	while (App.isOpen() && !inmain&&inlevel[(level - 1) * 4 + 2])
	{
		shield.setOrigin(-player.getShip().getPosition() + sf::Vector2f(30, 30));
		shield.setOutlineColor(sf::Color(100 * std::sin(shieldtimer.getElapsedTime().asSeconds()), 100 * std::cos(shieldtimer.getElapsedTime().asSeconds()), 100 + 100 * std::sin(shieldtimer.getElapsedTime().asSeconds())));
		shield.setRadius(80 + 10 * std::sin(shieldtimer.getElapsedTime().asSeconds()));
		lives.setString(std::to_string(player.getLifes()));
		score.setString("Score: " + std::to_string(player.getScore()));
		if (shieldtimer.getElapsedTime().asSeconds() > 10)shielded = false;
		if (!player.Dead && player.hitbyegg && player.deathtimer.getElapsedTime().asSeconds() >= 2){ player.hitbyegg = false; player.setpos(350, 680); shielded = true; shieldtimer.restart(); }
		if (player.Dead){ conclusion.setPosition(150, 350); conclusion.setString("YOU LOSE :P"); }
		if (starttimer.getElapsedTime().asSeconds() >= 3){ started = true; }

		if (!inlevel[(level - 1) * 4 + 3] && chickensDescendingOver(a, b, c)) {
			conclusion.setPosition(250, 250);
			conclusion.setString("YOU WIN :D\nNOW The Beast");
			player.setLevel(player.getLevel() + 1);
			transittimer.restart();
			inlevel[(level - 1) * 4 + 3] = true;
			if (!player.Dead) {
				
				updatesheet(player);
			}
		}
		if (player.Dead) {

			addScore2(player.getName(), player.getScore());
			player.setLevel(0);
			player.setLifes(3);
			player.setShots(1);
			player.setScore(0);
			inmain = true;
		}		
		
		if (inlevel[(level - 1) * 4 + 3] && transittimer.getElapsedTime().asSeconds() >= 3)
		{ inlevel[(level - 1) * 4 + 2] = false; }
		if (player.Dead) {
			inmain = true;
		}
		n = n % 40;
		sf::Event event;
		if (!playing)
		{ handlemenu(menu, App, event,menumusic); 
		if (firstmenu)
			starttimer.restart(); }
		else
		{
			while (App.pollEvent(event))
			if (event.type == sf::Event::Closed)App.close();
			movingship(player, App);
			shooting(player, timer, shoot);
			if (started)
			{
				if (flag) {
					wave3.restart();
					flag = false;
				}
				handleplayer(player, App, shieldtimer, eat, PB);
				handleChickenDescendingWave(a, b, c, player, wave3, level);
				for (int i = 0;i < 20;i++) {

					if (!shielded && !player.Dead && !player.hitbyegg && !a[i].Dead && player.getShip().getGlobalBounds().intersects(a[i].getCHShape().getGlobalBounds()))
						player.minusHealth();

					if(i<9)
						if (!shielded && !player.Dead && !player.hitbyegg && !b[i].Dead && player.getShip().getGlobalBounds().intersects(b[i].getCHShape().getGlobalBounds()))
							player.minusHealth();
					if(i<5)
						if (!shielded && !player.Dead && !player.hitbyegg && !c[i].Dead && player.getShip().getGlobalBounds().intersects(c[i].getCHShape().getGlobalBounds()))
							player.minusHealth();
				}
				chickenDescendingOnScreen(a, b, c);
			}
		}
		// Clear the screen (fill it with black color)
		App.clear();

		if (!playing){ menu.draw(App); }

		else{
			App.draw(background);
			if (!started){ App.draw(intro); }
			else{

				App.draw(Life);
				App.draw(lives);
				App.draw(score);
				App.draw(PB);
				for (int i = 0; i < 20; i++) {
					if (!a[i].Dead && !player.Dead)
						App.draw(a[i].getCHShape());
					if (i < 9) {
						if (!b[i].Dead && !player.Dead)
							App.draw(b[i].getCHShape());
					}
					if (i < 5) {
						if (!c[i].Dead && !player.Dead)
							App.draw(c[i].getCHShape());
					}
				}


				if (E.size()>0 && !player.Dead){
					for (int i = 0; i < E.size(); i++)
					{
						if (E[i].getBroken() == false){
							E[i].setpos(E[i].getShape().getPosition().x, E[i].getShape().getPosition().y + 1);
							App.draw(E[i].getShape());
						}

						if (E[i].getShape().getPosition().y >= App.getSize().y - 60 && E[i].getBroken() == false){
							E[i].setBroken(true);
						}
						if (E[i].getTimer()<0.5)App.draw(E[i].getShape());
						if (E[i].getTimer()>0.6 && E[i].getBroken()){ E.erase(E.begin() + i); }
					}
				}
				if (n > 0 && !player.Dead)
					for (int i = 0; i < n; i++)
				{
					BO[i].setpos(BO[i].getShape().getPosition().x, BO[i].getShape().getPosition().y + 0.9);
					App.draw(BO[i].getShape());
				}
			}
			if (!player.Dead && !player.hitbyegg)
				App.draw(player.getShip());
			
			if (player.Dead || chickensDescendingOver(a, b, c))
				App.draw(conclusion);

			if (shielded && !player.Dead)
				App.draw(shield);
			if (B.size() > 0 && !player.Dead)
				for (int i = 0; i < B.size(); i++)
					App.draw(B[i].getArrow());
		}

		App.display();
	}

	return;


}
void handleround3(int level, sf::RenderWindow& App, Player& player){
	Menu menu(App.getSize().x, App.getSize().y);
	srand(unsigned(time(NULL)));
	bool started = false;
	//intialize some sounds
	sf::SoundBuffer sh;
	sh.loadFromFile("Ionblaster.oga");
	sf::Sound shoot;
	shoot.setBuffer(sh);
	shoot.setVolume(50);
	//add you sounds here
	sf::SoundBuffer l;
	sf::Sound laid;
	l.loadFromFile("Ci1egglay.oga");
	laid.setBuffer(l);
	sf::SoundBuffer e;
	sf::Sound eat;
	e.loadFromFile("Ci1foodeat.oga");
	eat.setBuffer(e);
	SoundBuffer mm;
	Sound menumusic;
	mm.loadFromFile("Ci1maintheme.oga");
	menumusic.setBuffer(mm);
	//initialize player pos
	player.setpos(350, 680);
	//some timers
	sf::Clock timer;
	timer.restart();
	shielded = true;
	sf::Clock shieldtimer;
	shieldtimer.restart();
	sf::Clock starttimer;
	starttimer.restart();
	//initialize background
	sf::Texture bg;
	sf::Sprite background;
	if (level == 3) {
		bg.loadFromFile("coral.jpg");
		background.setTexture(bg);
	}
	else if(level==1) {
		bg.loadFromFile("space2.png");
		background.setTexture(bg);
		background.setScale(1.4, 1.4);
	}
	else if (level == 2) {
		bg.loadFromFile("sky.jpg");
		background.setTexture(bg);
		background.setScale(1.35, 1.35);
	}
	////sliding
	//sf::Sprite background2;
	//bg.loadFromFile("space2.png");
	//background2.setTexture(bg);
	//background2.setScale(1.4, 1.4);
	//background2.setPosition(600, 0);

	//shield
	sf::CircleShape shield(80);
	shield.setFillColor(sf::Color::Transparent);
	shield.setOutlineThickness(10);
	//hearts on top texture/sprite
	sf::Texture LI;
	LI.loadFromFile("heart.png");
	sf::Sprite Life;
	Life.setTexture(LI);
	Life.setPosition(5, 15);
	//pause button
	sf::Texture P;
	P.loadFromFile("pausebutton.png");
	sf::Sprite PB;
	PB.setTexture(P);
	PB.setPosition(App.getSize().x - 60, 0);

	bool flag = true;

	//arial font
	sf::Font font;
	font.loadFromFile("futureforcescondital.ttf");
	sf::Text lives;
	lives.setFont(font);
	lives.setFillColor(sf::Color::White);
	lives.setPosition(55, -15);
	lives.setCharacterSize(70);
	//intialize score
	sf::Text score;
	score.setFont(font);
	score.setFillColor(sf::Color::White);
	score.setPosition(App.getSize().x / 2 - 100, 0);
	score.setCharacterSize(50);
	//conclusion
	sf::Text conclusion;
	conclusion.setFont(font);
	conclusion.setFillColor(sf::Color::White);
	conclusion.setPosition(150, 350);
	conclusion.setCharacterSize(80);
	//intialize intro text
	sf::Text intro;
	intro.setFont(font);
	intro.setFillColor(sf::Color::White);
	intro.setPosition(130, 250);
	intro.setCharacterSize(80);
	intro.setString("Level " + std::to_string(level) + " wave 4");
	//initialize enemies here
	Chicken beast;
	beast.levelelwa7sh(level);
	SoundBuffer yw;
	yw.loadFromFile("youwin.ogg");
	Sound youwin;
	youwin.setBuffer(yw);
	Clock wave4, win;
	wave4.restart();

	n = 0;
	// Start game loop-----------------------------------------------------------------------------
	while (App.isOpen() && !inmain && inlevel[(level - 1) * 4 + 3])
	{
		shield.setOrigin(-player.getShip().getPosition() + sf::Vector2f(30, 30));
		shield.setOutlineColor(sf::Color(100 * std::sin(shieldtimer.getElapsedTime().asSeconds()), 100 * std::cos(shieldtimer.getElapsedTime().asSeconds()), 100 + 100 * std::sin(shieldtimer.getElapsedTime().asSeconds())));
		shield.setRadius(80 + 10 * std::sin(shieldtimer.getElapsedTime().asSeconds()));
		lives.setString(std::to_string(player.getLifes()));
		score.setString("Score: " + std::to_string(player.getScore()));
		if (shieldtimer.getElapsedTime().asSeconds() > 10)
			shielded = false;
		if (!player.Dead && player.hitbyegg && player.deathtimer.getElapsedTime().asSeconds() >= 2)
		{ player.hitbyegg = false; 
		player.setpos(350, 680); 
		shielded = true; 
		shieldtimer.restart(); 
		}
		if (player.Dead)
		{ conclusion.setPosition(150, 350); 
		conclusion.setString("YOU LOSE :P"); 
		}
		if (starttimer.getElapsedTime().asSeconds() >= 3)
		{ started = true; }
		if (level <= 3){
			if (!inlevel[(level - 1) * 4 + 4] && beast.Dead)
			{
				conclusion.setPosition(100, 250);
				if (level < 3) {
					conclusion.setString("  Get Ready For \n     Planet " + to_string(level + 1));
					player.setLevel(player.getLevel() + 1);
				}
				transittimer.restart();
				inlevel[(level - 1) * 4 + 4] = true;
				if (!player.Dead) {
					
					updatesheet(player);
				}
			}
				
			if (player.Dead || (beast.Dead&&level==3)){

				addScore2(player.getName(), player.getScore());
				player.setLevel(0);
				player.setLifes(3);
				player.setShots(1);
				player.setScore(0);
				inmain = true;
				//inlevel[(level - 1) * 4 + 3] = false;
			}
			
			
			
			if (inlevel[(level - 1) * 4 + 4] && transittimer.getElapsedTime().asSeconds() >= 3)
			{ inlevel[(level - 1) * 4 + 3] = false; }
		}
		else{
			if (!scoretaken)
				addScore2(player.getName(), player.getScore());
			else if(beast.Dead&&level==3)
			{ inlevel[(level - 1) * 4 + 3] = false;
			conclusion.setString("YOU WIN"); 
			//inmain = true;
			}
		}
		/*if (win.getElapsedTime().asSeconds() > 3&&beast.Dead&&level==3) {
				youwin.stop();
				inmain = true;
			}*/
		if (player.Dead) {
			inmain = true;
		}
		n = n % 40;
		sf::Event event;
		if (!playing){ handlemenu(menu, App, event,menumusic); if (firstmenu)starttimer.restart(); }
		else
		{
			while (App.pollEvent(event))
			if (event.type == sf::Event::Closed)App.close();
			movingship(player, App);
			shooting(player, timer, shoot);
			if (started)
			{
				handleplayer(player, App, shieldtimer, eat, PB);
				beastMovement(beast, wave4);

				if (!shielded && !player.Dead && !beast.Dead&& !player.hitbyegg && player.getShip().getGlobalBounds().intersects(beast.getCHShape().getGlobalBounds()))
					player.minusHealth();

				for (int m = 0; m < B.size(); m++)
				if (!beast.Dead && beast.getCHShape().getGlobalBounds().intersects(B.at(m).getArrow().getGlobalBounds()))
				{
					beast.beastminushealth();
					B.erase(B.begin() + m);
				}
				else if (B.at(m).getArrow().getPosition().y < -5)
					B.erase(B.begin() + m);
				//dropping eggs
				int random = rand() % 500;
				if (random == 0)
				{if (!beast.Dead&&!player.Dead)
					{
						E.push_back(egg); E.at(E.size() - 1).setSpeedfactor(((rand() % 51) / 100.0) + 1);
						E.at(E.size() - 1).setpos(beast.getCHShape().getPosition().x + 120, beast.getCHShape().getPosition().y + 260);
						laid.play();
					}
				}
				if (beast.Dead && flag) {
					player.setScore(player.getScore() + 2000 * player.getLevel());
					flag = false;
				}
			}
		}
		// Clear the screen (fill it with black color)
		App.clear();

		if (!playing){ menu.draw(App); }

		else{
			App.draw(background);
			if (!started)
			{ App.draw(intro); }
			else{

				App.draw(Life);
				App.draw(lives);
				App.draw(score);
				App.draw(PB);
				if (!beast.Dead && !player.Dead)
					App.draw(beast.getCHShape());

				if (E.size()>0 && !player.Dead && !beast.Dead){
					for (int i = 0; i < E.size(); i++)
					{
						if (E[i].getBroken() == false){
							E[i].setpos(E[i].getShape().getPosition().x, E[i].getShape().getPosition().y + (0.3*E[i].getSpeedfactor()));
							App.draw(E[i].getShape());
						}

						if (E[i].getShape().getPosition().y >= App.getSize().y - 60 && E[i].getBroken() == false){
							E[i].setBroken(true);
						}
						if (E[i].getTimer()<0.5)
							App.draw(E[i].getShape());
						if (E[i].getTimer()>0.6 && E[i].getBroken())
						{ E.erase(E.begin() + i); }
					}
				}

				if (n > 0 &&!beast.Dead)
					for (int i = 0; i < n; i++)
				{
					BO[i].setpos(BO[i].getShape().getPosition().x, BO[i].getShape().getPosition().y + 0.25);
					
					App.draw(BO[i].getShape());
				}
			}

			if (!player.Dead && !player.hitbyegg)
				App.draw(player.getShip());

			if (player.Dead || beast.Dead)
				App.draw(conclusion);

			if (beast.Dead && level == 3) {
				
			//	youwin.play();
				win.restart();
				gameover = true;
			}

			

			if (shielded && !player.Dead && !beast.Dead)
				App.draw(shield);
			for (int i = 0; i < B.size(); i++)
				if(!player.Dead&&!beast.Dead)
				App.draw(B[i].getArrow());
		}

		App.display();
	}

	return;


}
void handlemain(sf::RenderWindow& App, Player& player, sf::Event& event){
	Menu main(App.getSize().x, App.getSize().y, "Start Journey", "Hall of Fame", "Exit");
	bool playerloaded = false;
	int pressedacc;
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		cout << "font not loaded";
	}
	//list of users
	vector<sf::Text> players;

	//choose and delete
	sf::Text options[2];
	for (int i = 0; i < 2; i++)
	{
		options[i].setFont(font);
		options[i].setCharacterSize(25);
		options[0].setString("Delete");
		options[i].setPosition(App.getSize().x - 25 * options[1].getString().getSize() - 25 * options[0].getString().getSize(), App.getSize().y);
		options[1].setString("Choose");

	}
	//title of account
	sf::Text account, store;
	store.setFont(font);
	account.setFont(font);
	account.setCharacterSize(40);
	account.setPosition(App.getSize().x / 4 + 10, App.getSize().y / 2 - 100);
	account.setString("Choose an account");
	//border of title
	sf::RectangleShape border, border1;
	border.setFillColor(sf::Color::Transparent);
	border1.setFillColor(sf::Color::Transparent);
	border.setOutlineThickness(5);
	border1.setOutlineThickness(5);
	border.setSize(sf::Vector2f(account.getCharacterSize()*account.getString().getSize() / 1.9, 1.5* account.getCharacterSize()));

	while (App.isOpen() && inmain){
		if (gamejustopened)
		{
			account.setPosition(App.getSize().x / 4 + 10, App.getSize().y / 2 - 100);
			account.setString("Choose an account");
			border.setPosition(account.getPosition());
			border.setSize(sf::Vector2f(account.getCharacterSize()*account.getString().getSize() / 1.9, 1.5* account.getCharacterSize()));

			main.disable(App);//hide start game and exit
			//back to white
			account.setFillColor(sf::Color::White);
			border.setOutlineColor(sf::Color::White);
			options[0].setFillColor(sf::Color::White);
			options[1].setFillColor(sf::Color::White);
			for (int i = 0; i < players.size(); i++)
				players.at(i).setFillColor(sf::Color::White);
			if (players.size() <= 2){
				options[0].setPosition(options[0].getPosition().x, App.getSize().y);
			}

			//if x is pressed
			while (App.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					App.close();
				}

			}
			//if hovering over...
			//choose and delete
			for (int i = 0; i < 2; i++)
			{
				if (options[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
				{
					options[i].setFillColor(sf::Color::Red);
				}
			}
			//borders and account
			if (border.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
			{
				account.setFillColor(sf::Color::Red);
				border.setOutlineColor(sf::Color::Red);
			}
			//the list of players
			for (int i = 0; i < players.size(); i++)
			if (players.at(i).getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
			{
				players.at(i).setFillColor(sf::Color::Red);
			}
			//if mouse is pressed on...
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				//title
				if (!playerloaded&&border.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
				{
					loadplayer(App, players);
					playerloaded = true;
				}
				//list of names==> gets you choose and delete 
				if (playerloaded){
					for (int i = 0; i < players.size() - 1; i++)
					if (players.at(i).getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
					{
						for (int j = 0; j < 2; j++)
							options[j].setPosition(options[j].getPosition().x, players[i].getPosition().y);
						pressedacc = i;
					}
					//pressing on create new
					if (players.at(players.size() - 1).getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
					{
						//full list of users
						if (players.size() == 5){
							sf::Text full;
							full.setFont(font);
							full.setCharacterSize(25);
							full.setString("You have reached the maximum number of users.\nPlease delete one of the accounts in order to proceed!");
							full.setPosition(0, 0);
							sf::RenderWindow fullmsg(sf::VideoMode(full.getCharacterSize()*full.getString().getSize() / 4, full.getCharacterSize()*2.5, 32), "Full Capicity Reached");
							while (fullmsg.isOpen())
							{
								sf::Event event2;
								while (fullmsg.pollEvent(event2))
								{
									if ((event2.type == sf::Event::Closed) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))))
										fullmsg.close();
								}
								fullmsg.clear(sf::Color(169, 169, 169));
								fullmsg.draw(full);
								fullmsg.display();
							}
						}
						//not full gets you new page to enter the new user name when you press done
						else{
							for (int j = 0; j < 2; j++)
								options[j].setPosition(options[j].getPosition().x, App.getSize().y);
							pressedacc = players.size() - 2;
							sf::RenderWindow create(sf::VideoMode(400, 200, 32), "Enter a new username");
							sf::Text mine;
							mine.setFont(font);
							mine.setCharacterSize(25);
							mine.setString("Username:");
							mine.setPosition(create.getSize().x / 2 - 7 * mine.getString().getSize(), 50);
							sf::Text yours;
							yours.setFont(font);
							yours.setCharacterSize(25);
							yours.setString("");
							sf::Text done;
							done.setFont(font);
							done.setCharacterSize(25);
							done.setString("Done");

							sf::Clock backspacetimer;
							while (create.isOpen())
							{
								done.setFillColor(sf::Color::White);
								yours.setPosition(create.getSize().x / 2 - 7.5 * yours.getString().getSize(), 100);
								if (yours.getString().getSize() > 0){ done.setPosition(create.getSize().x - 60, create.getSize().y - 30); }
								else { done.setPosition(create.getSize().x, App.getSize().y); }
								sf::Event event2;
								while (create.pollEvent(event2))
								{
									if ((event2.type == sf::Event::Closed) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))))
										create.close();
								}
								if (done.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(create))))
								{
									done.setFillColor(sf::Color::Red);
								}
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && backspacetimer.getElapsedTime().asSeconds() > 0.15)
								{
									string s = std::string(yours.getString()); if (s.size() > 0)s.resize(s.size() - 1);
									yours.setString(s);
									backspacetimer.restart();
								}
								else
								for (int i = 0; i < 102; i++)if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(i)) && backspacetimer.getElapsedTime().asSeconds() > 0.15)
								{
									if (i != 57 && i != 59){ yours.setString(std::string(yours.getString()) + fromKtoS(sf::Keyboard::Key(i))); }
									backspacetimer.restart();
								}
								if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && done.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(create))))
								{
									addplayer(string(yours.getString()));
									create.close();
									loadplayer(App, players);
								}
								create.clear(sf::Color(169, 169, 169));
								create.draw(mine);
								create.draw(yours);
								create.draw(done);
								create.display();
							}
						}


					}
				}
				//delete is pressed
				if (options[0].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
				{
					bool agreed = false;
					sf::Text yes;
					yes.setFont(font);
					yes.setCharacterSize(25);
					yes.setString("Yes");
					sf::Text no;
					no.setFont(font);
					no.setCharacterSize(25);
					no.setString("No");
					sf::Text confirm;
					confirm.setFont(font);
					confirm.setCharacterSize(25);
					confirm.setString("Are you sure you want to permenetly delete this user " + string(players.at(pressedacc).getString()) + "?");
					confirm.setPosition(0, 0);
					sf::RenderWindow deletemsg(sf::VideoMode(confirm.getCharacterSize()*confirm.getString().getSize() / 2, confirm.getCharacterSize() * 4, 32), "Data Will Be Lost");
					yes.setPosition(deletemsg.getSize().x / 2 + 30, deletemsg.getSize().y - 50);
					no.setPosition(deletemsg.getSize().x / 2 - 30, deletemsg.getSize().y - 50);
					while (deletemsg.isOpen())
					{
						sf::Event event2;
						while (deletemsg.pollEvent(event2))
						{
							if ((event2.type == sf::Event::Closed) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))))
								deletemsg.close();
						}
						yes.setFillColor(sf::Color::White);
						no.setFillColor(sf::Color::White);
						if (yes.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(deletemsg))))
						{
							yes.setFillColor(sf::Color::Red);
						}
						if (no.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(deletemsg))))
						{
							no.setFillColor(sf::Color::Red);
						}
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
							if (yes.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(deletemsg))))
							{
								agreed = true;
								deletemsg.close();
							}
							if (no.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(deletemsg))))
							{
								agreed = false;
								deletemsg.close();
							}
						}
						deletemsg.clear(sf::Color(169, 169, 169));
						deletemsg.draw(confirm);
						deletemsg.draw(yes);
						deletemsg.draw(no);
						deletemsg.display();
					}
					if (agreed){
						deleteplayer(pressedacc, players.size() - 2);
						for (int j = 0; j < 2; j++)
							options[j].setPosition(options[j].getPosition().x, App.getSize().y);
						loadplayer(App, players);
					}
				}
				//choose is pressed
				if (options[1].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
				{
					{
						bool agreed = false;
						sf::Text yes;
						yes.setFont(font);
						yes.setCharacterSize(25);
						yes.setString("Yes");
						sf::Text no;
						no.setFont(font);
						no.setCharacterSize(25);
						no.setString("No");
						sf::Text confirm;
						confirm.setFont(font);
						confirm.setCharacterSize(25);
						confirm.setString("Are you sure you want Choose this user " + string(players.at(pressedacc).getString()) + "?");
						confirm.setPosition(0, 0);
						sf::RenderWindow conmsg(sf::VideoMode(confirm.getCharacterSize()*confirm.getString().getSize() / 2, confirm.getCharacterSize() * 4, 32), "Choosing Confirmation");
						yes.setPosition(conmsg.getSize().x / 2 + 30, conmsg.getSize().y - 50);
						no.setPosition(conmsg.getSize().x / 2 - 30, conmsg.getSize().y - 50);
						while (conmsg.isOpen())
						{
							sf::Event event2;
							while (conmsg.pollEvent(event2))
							{
								if ((event2.type == sf::Event::Closed) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))))
									conmsg.close();
							}
							yes.setFillColor(sf::Color::White);
							no.setFillColor(sf::Color::White);
							if (yes.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(conmsg))))
							{
								yes.setFillColor(sf::Color::Red);
							}
							if (no.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(conmsg))))
							{
								no.setFillColor(sf::Color::Red);
							}
							if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
								if (yes.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(conmsg))))
								{
									agreed = true;
									conmsg.close();
								}
								if (no.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(conmsg))))
								{
									agreed = false;
									conmsg.close();
								}
							}
							conmsg.clear(sf::Color(169, 169, 169));
							conmsg.draw(confirm);
							conmsg.draw(yes);
							conmsg.draw(no);
							conmsg.display();
						}
						if (agreed)
						{
							chooseplayer(player, pressedacc);
							gamejustopened = false;
							main.enable(App);
						}

					}
				}
			}
		}
		else{
			players.clear();
			for (int j = 0; j < 2; j++)
				options[j].setPosition(options[j].getPosition().x, App.getSize().y);
			account.setString("Change \nAccount");
			account.setPosition(0.75*App.getSize().x, 0.4*App.getSize().y);
			account.setFillColor(sf::Color::White);
			border.setSize(sf::Vector2f(account.getCharacterSize()*account.getString().getSize() / 3.8, 3 * account.getCharacterSize()));
			border.setPosition(account.getPosition());
			border.setOutlineColor(sf::Color::White);
			if (border.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
			{
				account.setFillColor(sf::Color::Red);
				border.setOutlineColor(sf::Color::Red);
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			if (border.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
			{
				gamejustopened = true;
				playerloaded = false;
			}

			store.setString("Store");
			store.setPosition(0.75*App.getSize().x, 0.6*App.getSize().y);
			store.setFillColor(sf::Color::White);
			border1.setSize(sf::Vector2f(store.getCharacterSize()*store.getString().getSize() / 1.8, 1.5 * store.getCharacterSize()));
			border1.setPosition(store.getPosition());
			border1.setOutlineColor(sf::Color::White);
			if (border1.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
			{
				store.setFillColor(sf::Color::Red);
				border1.setOutlineColor(sf::Color::Red);
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				if (border1.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
				{
					handleStore(player);
				}
			while (App.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
					App.close();
					break;
				case sf::Event::KeyReleased:
					switch (event.key.code)
					{
					case sf::Keyboard::Up:
						main.MoveUp();
						break;

					case sf::Keyboard::Down:
						main.MoveDown();
						break;

					case sf::Keyboard::Return:
						switch (main.GetPressedItem())
						{
						case 0:
							inmain = false;
							playing = false;
							scoretaken = false;
							inlevel[player.getLevel()] = true;
							break;
						case 1:
							readscores(App, players);
							main.disable(App);
							playerloaded = true;
							while (playerloaded&&App.isOpen())
							{
								if (players[players.size() - 1].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
								{
									players.at(players.size() - 1).setFillColor(sf::Color::Red);
									if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
									{
										main.enable(App); players.clear(); playerloaded = false;
									}
								}
								App.clear();
								main.draw(App);
								for (int i = 0; i < players.size(); i++){
									players.at(i).setFont(font);
									App.draw(players.at(i));
								}
								App.display();
							}

							break;
						case 2:
							App.close();
							break;
						}

						break;
					}

					break;
				}
			}

		}
		App.clear();
		main.draw(App);
		App.draw(account);
		App.draw(store);
		App.draw(border1);
		App.draw(border);
		for (int i = 0; i < 2; i++)
			App.draw(options[i]);

		if (playerloaded)for (int i = 0; i < players.size(); i++){
			players.at(i).setFont(font);
			App.draw(players.at(i));
		}


		App.display();

	}
}
void loadplayer(sf::RenderWindow& App, vector<sf::Text>& players){
	ifstream file;
	players.clear();
	sf::Text playername;
	playername.setFillColor(sf::Color::White);
	file.open("accounts.txt");
	if (!file.fail()){
		string name;
		while (!file.eof() && players.size() < 4){
			getline(file, name);
			name.resize(name.find(" "));
			playername.setString(name);
			players.push_back(playername);
		}file.close();
	}
	else cout << "error loading account.txt";
	playername.setString("CREATE A NEW ACCOUNT");
	players.push_back(playername);
	for (int i = 0; i < players.size(); i++)
	{
		players[i].setPosition(sf::Vector2f(App.getSize().x / 2 - 30 - players[i].getString().getSize() * 8, 400 + (App.getSize().y - 400) / (players.size() + 1) * i));
	}

}
string fromKtoS(const sf::Keyboard::Key& k){
	string ret;
	switch (k){

	case sf::Keyboard::A:

		ret = "A";
		break;
	case sf::Keyboard::B:

		ret = "B";
		break;
	case sf::Keyboard::C:

		ret = "C";
		break;
	case sf::Keyboard::D:

		ret = "D";
		break;
	case sf::Keyboard::E:

		ret = "E";
		break;
	case sf::Keyboard::F:

		ret = "F";
		break;
	case sf::Keyboard::G:

		ret = "G";
		break;
	case sf::Keyboard::H:

		ret = "H";
		break;
	case sf::Keyboard::I:

		ret = "I";
		break;
	case sf::Keyboard::J:

		ret = "J";
		break;
	case sf::Keyboard::K:

		ret = "K";
		break;
	case sf::Keyboard::L:

		ret = "L";
		break;
	case sf::Keyboard::M:

		ret = "M";
		break;
	case sf::Keyboard::N:

		ret = "N";
		break;
	case sf::Keyboard::O:

		ret = "O";
		break;
	case sf::Keyboard::P:

		ret = "P";
		break;
	case sf::Keyboard::Q:

		ret = "Q";
		break;
	case sf::Keyboard::R:

		ret = "R";
		break;
	case sf::Keyboard::S:

		ret = "S";
		break;
	case sf::Keyboard::T:

		ret = "T";
		break;
	case sf::Keyboard::U:

		ret = "U";
		break;
	case sf::Keyboard::V:

		ret = "V";
		break;
	case sf::Keyboard::W:

		ret = "W";
		break;
	case sf::Keyboard::X:

		ret = "X";
		break;
	case sf::Keyboard::Y:

		ret = "Y";
		break;
	case sf::Keyboard::Z:

		ret = "Z";
		break;
	case sf::Keyboard::Num0:

		ret = "0";
		break;
	case sf::Keyboard::Num1:

		ret = "1";
		break;
	case sf::Keyboard::Num2:

		ret = "2";
		break;
	case sf::Keyboard::Num3:

		ret = "3";
		break;
	case sf::Keyboard::Num4:

		ret = "4";
		break;
	case sf::Keyboard::Num5:

		ret = "5";
		break;
	case sf::Keyboard::Num6:

		ret = "6";
		break;
	case sf::Keyboard::Num7:

		ret = "7";
		break;
	case sf::Keyboard::Num8:

		ret = "8";
		break;
	case sf::Keyboard::Num9:

		ret = "9";
		break;

	case sf::Keyboard::Comma:

		ret = ",";
		break;
	case sf::Keyboard::Period:

		ret = ".";
		break;
	case sf::Keyboard::Equal:

		ret = "=";
		break;
	case sf::Keyboard::Dash:

		ret = "-";
		break;
	case sf::Keyboard::Space:

	case sf::Keyboard::Numpad0:

		ret = "0";
		break;
	case sf::Keyboard::Numpad1:

		ret = "1";
		break;
	case sf::Keyboard::Numpad2:

		ret = "2";
		break;
	case sf::Keyboard::Numpad3:

		ret = "3";
		break;
	case sf::Keyboard::Numpad4:

		ret = "4";
		break;
	case sf::Keyboard::Numpad5:

		ret = "5";
		break;
	case sf::Keyboard::Numpad6:

		ret = "6";
		break;
	case sf::Keyboard::Numpad7:

		ret = "7";
		break;
	case sf::Keyboard::Numpad8:

		ret = "8";
		break;
	case sf::Keyboard::Numpad9:

		ret = "9";
		break;


	default:
		ret = "";
		break;
	}
	return ret;






}
void addplayer(string name){
	ifstream inp;
	ofstream file;
	char c;
	inp.open("accounts.txt");
	if (!inp.fail()){
		file.open("temp.txt");
		if (!file.fail()){
			inp.get(c);
			while (!inp.eof())
			{
				file.put(c);
				inp.get(c);
			}
			inp.close();
			file << endl << name << " " << "0" << " " << "0" << " " << "3" << " " << "1";
			file.flush();
			file.close();
		}
		else cout << "error loading temp.txt";
	}
	else cout << "error loading account.txt";

	inp.open("temp.txt");
	if (!inp.fail()){
		file.open("accounts.txt");
		if (!file.fail()){
			inp.get(c);
			while (!inp.eof())
			{
				file.put(c);
				inp.get(c);
			}
			inp.clear();
			inp.close();
			file.close();
		}
		else cout << "error loading account.txt";
	}
	else cout << "error loading temp.txt";

}
void deleteplayer(int z, int size){
	ifstream inp;
	ofstream file;
	char c;
	int i = 0;
	inp.open("accounts.txt");
	if (!inp.fail()){
		file.open("temp.txt");
		if (!file.fail()){
			inp.get(c);
			while (!inp.eof())
			{

				if (z == size){
					if (i == size - 1 && c == '\n');
					else if (i == z);
					else file.put(c);
				}
				else
				if (i != z)file.put(c);
				if (c == '\n')
					i++;
				inp.get(c);
			};
			inp.close();
			file.flush();
			file.close();
		}
		else cout << "error loading temp.txt";
	}
	else cout << "error loading account.txt";

	inp.open("temp.txt");
	if (!inp.fail()){
		file.open("accounts.txt");
		if (!file.fail()){
			inp.get(c);
			while (!inp.eof())
			{
				file.put(c);
				inp.get(c);
			}
			inp.clear();
			inp.close();
			file.close();
		}
		else cout << "error loading account.txt";
	}
	else cout << "error loading temp.txt";

}
void chooseplayer(Player& player, int pos){
	ifstream file;
	int i = 0;
	char c;
	string name;
	file.open("accounts.txt");
	if (!file.fail()){
		while (!file.eof() && i != pos){
			file.get(c);
			if (c == '\n')i++;
		}
		file >> name;
		player.setName(name);
		file >> name;
		player.setLevel(stoi(name));
		file >> name;
		player.setScore(stoi(name));
		file >> name;
		player.setLifes(stoi(name));
		file >> name;
		player.setShots(stoi(name));
		file.close();
	}

}
void updatesheet(Player& player){
	ifstream inp;
	ofstream file;
	char c;
	string name;
	inp.open("accounts.txt");
	if (!inp.fail()){
		file.open("temp.txt");
		if (!file.fail()){
			getline(inp, name);
			if (!name.find(player.getName() + " ")) 
				file << player.getName() << " " << player.getLevel() << " " << player.getScore() << " " << player.getLifes() << " " << player.getShots();
			
			else
				file << name;
			while (!inp.eof())
			{
				getline(inp, name);
				if (!name.find(player.getName() + " ")) 
					 file << endl << player.getName() << " " << player.getLevel() << " " << player.getScore() << " " << player.getLifes() << " " << player.getShots();
				
				else
					file << endl << name;

			}
			inp.close();
			file.flush();
			file.close();
		}
		else cout << "error loading temp.txt";
	}
	else cout << "error loading account.txt";

	inp.open("temp.txt");
	if (!inp.fail()){
		file.open("accounts.txt");
		if (!file.fail()){
			inp.get(c);
			while (!inp.eof())
			{
				file.put(c);
				inp.get(c);
			}
			inp.clear();
			inp.close();
			file.close();
		}
		else cout << "error loading account.txt";
	}
	else cout << "error loading temp.txt";

}
void movingship(Player& player, sf::RenderWindow& App){
	if (keyboardf){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player.getShip().getPosition().x < 690)
			player.setpos(player.getShip().getPosition().x + 0.75, player.getShip().getPosition().y);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player.getShip().getPosition().x>0)
			player.setpos(player.getShip().getPosition().x - 0.75, player.getShip().getPosition().y);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player.getShip().getPosition().y < 680)

			player.setpos(player.getShip().getPosition().x, player.getShip().getPosition().y + 0.75);

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player.getShip().getPosition().y > 0)
			player.setpos(player.getShip().getPosition().x, player.getShip().getPosition().y - 0.75);
	}
	if (mousef)
		player.setpos(App.mapPixelToCoords(sf::Mouse::getPosition(App)).x, App.mapPixelToCoords(sf::Mouse::getPosition(App)).y);
}
void shooting(Player& player, sf::Clock& timer, sf::Sound& shoot){

	if (!player.Dead &&!player.hitbyegg && (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) && timer.getElapsedTime().asSeconds() > 0.4)
	{
		for (int i = 0; i < player.getShots(); i++)
		{
			B.push_back(bullet);
			B.at(B.size() - 1).setpos(player.getShip().getPosition().x + 43, player.getShip().getPosition().y - 50);
			if (player.getShots() == 2){
				if (i == 0)B.at(B.size() - 1).setpos(B.at(B.size() - 1).getArrow().getPosition().x + 23, B.at(B.size() - 1).getArrow().getPosition().y);
				else if (i == 1)B.at(B.size() - 1).setpos(B.at(B.size() - 1).getArrow().getPosition().x - 23, B.at(B.size() - 1).getArrow().getPosition().y);
			}
			else if (player.getShots() == 3){
				if (i == 0)B.at(B.size() - 1).setrotation(20);
				else if (i == 1)B.at(B.size() - 1).setrotation(-20);
			}
		}
		shoot.play();
		timer.restart();
	}
	if (B.size() > 0){
		if (player.getShots() < 3)for (int i = 0; i < B.size(); i++){ B[i].setpos(B[i].getArrow().getPosition().x, B[i].getArrow().getPosition().y - 1); }
		else for (int i = 0; i < B.size(); i++)
		{
			if (B.at(i).getrotation() == 20)
				B[i].setpos(B[i].getArrow().getPosition().x + 0.447, B[i].getArrow().getPosition().y - 1);
			else if (B.at(i).getrotation() == 340)
				B[i].setpos(B[i].getArrow().getPosition().x - 0.447, B[i].getArrow().getPosition().y - 1);
			else if (B.at(i).getrotation() == 0)
				B[i].setpos(B[i].getArrow().getPosition().x, B[i].getArrow().getPosition().y - 1);
		}
	}
}
void handleplayer(Player& player, sf::RenderWindow& App, sf::Clock& shieldtimer, sf::Sound& eat, sf::Sprite& PB){
	//clicking pause
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//if pause is pressed
		if (PB.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))))
		{
			playing = false;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){ playing = !playing; }
	//collecting pickups
	for (int m = 0; m < n; m++)
	if (!player.Dead&& !player.hitbyegg && player.getShip().getGlobalBounds().intersects(BO[m].getShape().getGlobalBounds()))
	{
		switch (BO[m].getName())
		{
		case(life) : player.setLifes(player.getLifes() + 1); break;
		case(immunity) : shielded = true; shieldtimer.restart(); break;
		case(meat) : player.setScore(player.getScore() + 50); eat.play(); break;
		case(fire) : player.setShots(player.getShots() + 1); break;
		}
		BO[m].setpos(1000, 1000);
	}
	//hitting eggs
	for (int m = 0; m < E.size(); m++)
	if (!shielded&&!player.Dead&&!player.hitbyegg && E.at(m).getBroken() == false && player.getShip().getGlobalBounds().intersects(E.at(m).getShape().getGlobalBounds()))
	{
		player.minusHealth(); E.erase(E.begin() + m);
	}
}
void handlechicken(Player& player, Chicken round1[][number], sf::Sound& laid,int level){
	//hitting chicken
	for (int i = 0; i < number; i++)
	for (int j = 0; j < number; j++)
	for (int m = 0; m < B.size(); m++)
	if (round1[i][j].Dead == false && round1[i][j].getCHShape().getGlobalBounds().intersects(B.at(m).getArrow().getGlobalBounds()))
	{
		round1[i][j].minusHealth(); B.erase(B.begin() + m);
	}
	else if (B.at(m).getArrow().getPosition().y < -50) B.erase(B.begin() + m);
	//Pickups
	for (int i = 0; i < number; i++)
	for (int j = 0; j < number; j++){
	if (round1[i][j].Dead && round1[i][j].justdead){
		if (level == 3 && round1[i][j].Dead && round1[i][j].justdead&& i == 1 && j == 3) {
			round1[i][j].justdead = false;
			player.setScore(player.getScore() + 100 * round1[i][j].getLevel());
			BO[n].setName(key);
			BO[n++].setpos(round1[i][j].getCHShape().getPosition().x + 60, round1[i][j].getCHShape().getPosition().y + 80);
		}
		else {
			round1[i][j].justdead = false;
			player.setScore(player.getScore() + 100 * round1[i][j].getLevel());

			if (rand() % 10 == 0)
			{
				BO[n].setName(bonus(rand() % 4));
				BO[n++].setpos(round1[i][j].getCHShape().getPosition().x + 60, round1[i][j].getCHShape().getPosition().y + 80);
			}
		}
	}
	 if  (level==3 && round1[i][j].Dead && round1[i][j].justdead&& i == 1 && j == 3 ) {
		round1[i][j].justdead = false;
		BO[n].setName(key);
		BO[n++].setpos(round1[i][j].getCHShape().getPosition().x + 60, round1[i][j].getCHShape().getPosition().y + 80);
	}
	}
	//dropping eggs
	int random = rand() % 1000;
	if (countAliveChickens(round1) <= 5)
		random = rand() % 80;
	if (random == 0)
	{
		int i = rand() % number; int j = rand() % number; if (round1[i][j].Dead == false)
		{
			E.push_back(egg); E.at(E.size() - 1).setSpeedfactor(((rand() % 51) / 100.0) + 1);
			E.at(E.size() - 1).setpos(round1[i][j].getCHShape().getPosition().x + 60, round1[i][j].getCHShape().getPosition().y + 80);
			laid.play();
		}
	}
	//movingchicken
	if (round1[0][0].getCHShape().getPosition().x <= 0){ movesleft = false; }
	else if (round1[0][0].getCHShape().getPosition().x >= 180)movesleft = true;
	for (int i = 0; i < number; i++)
	for (int j = 0; j < number; j++)
	{
		if (movesleft)round1[i][j].setpos(round1[i][j].getCHShape().getPosition().x - 0.05, round1[i][j].getCHShape().getPosition().y);
		else round1[i][j].setpos(round1[i][j].getCHShape().getPosition().x + 0.05, round1[i][j].getCHShape().getPosition().y);
	}
}
void addScore2(string name, int score) {
	scoretaken = true;
	ifstream inp;
	ofstream file;
	char c;
	string k;
	int counter2 = 0;
	int counter = 0;
	inp.open("score.txt");
	if (!inp.fail()) 
	{
		while (!inp.eof())
		{
			inp >> k >> k;
 			if (score >= stoi(k)) { inp.close(); }
			else { counter++; }
		}
	}
	inp.open("score.txt");
	if (!inp.fail()) 
	{
		file.open("temp.txt");
		if (!file.fail()) 
		{
			if (counter2 == counter) { file << name << " " << score << endl; counter = -1; }
			inp.get(c);
			while (!inp.eof())
			{
				
				if (c == '\n')
					counter2++;
				file.put(c);
				if (counter2 == counter) 
				{ file << name << " " << score << endl; counter = -1; }
				inp.get(c);
			}

			inp.close();
			file.flush();
			file.close();
		}
		else cout << "error loading temp.txt";
	}
	else cout << "error loading score.txt";

	inp.open("temp.txt");
	if (!inp.fail()) 
	{
		file.open("score.txt");
		if (!file.fail()) 
		{
			inp.get(c);
			while (!inp.eof())
			{
				file.put(c);
				inp.get(c);
			}
			inp.clear();
			inp.close();
			file.close();
		}
		else cout << "error loading score.txt";
	}
	else cout << "error loading temp.txt";


}
void readscores(sf::RenderWindow& App, vector<sf::Text>& players){
	ifstream file;
	players.clear();
	sf::Text playername;
	playername.setFillColor(sf::Color::White);
	file.open("score.txt");
	if (!file.fail()){
		string name;
		while (!file.eof() && players.size() < 5){
			getline(file, name);
			playername.setString(name);
			players.push_back(playername);
		}file.close();
	}
	else cout << "error loading account.txt";
	playername.setString("--Back--");
	players.push_back(playername);
	for (int i = 0; i < players.size(); i++)
	{
		players[i].setPosition(sf::Vector2f(App.getSize().x / 2 - 30 - players[i].getString().getSize() * 8, 400 + (App.getSize().y - 400) / (players.size() + 1) * i));
	}

}

void beastMovement(Chicken & beast, sf::Clock & timer3){
	if (pos3 && timer3.getElapsedTime().asSeconds() >= 0.3)
	{
		sf::Vector2f np1(0, 100);
		sf::Vector2f direction = beast.normalize(np1 - beast.getCHShape().getPosition());
		beast.setpos(beast.getCHShape().getPosition().x + beast.getSpeedFactorBeast()* direction.x, beast.getCHShape().getPosition().y + beast.getSpeedFactorBeast()* direction.y);
		pos1 = true;
		pos2 = false;
		pos3 = false;
		timer3.restart();
	}
	else if (pos1 && timer3.getElapsedTime().asSeconds() >= 0.3)
	{

		sf::Vector2f np2(600, 50);
		sf::Vector2f direction = beast.normalize(np2 - beast.getCHShape().getPosition());
		beast.setpos(beast.getCHShape().getPosition().x + beast.getSpeedFactorBeast()* direction.x, beast.getCHShape().getPosition().y + beast.getSpeedFactorBeast()*direction.y);
		pos2 = true;
		pos1 = false;
		pos3 = false;
		timer3.restart();
	}
	else if (pos2 && timer3.getElapsedTime().asSeconds() >= 0.3)
	{
		sf::Vector2f np3(300, 100);
		sf::Vector2f direction = beast.normalize(np3 - beast.getCHShape().getPosition());
		beast.setpos(beast.getCHShape().getPosition().x + beast.getSpeedFactorBeast()* direction.x, beast.getCHShape().getPosition().y + beast.getSpeedFactorBeast()* direction.y);
		pos3 = true;
		pos1 = false;
		pos2 = false;
		timer3.restart();

	}

}
void rockWave(Rocks R[], int s, int e){
	if (e<77) {
		int xy = rand() % 2;
		int randx = rand() % 801 + 100;

		for (int i = s; i < e; i++) {
			if (xy == 0)
				R[i].setpos(randx - 910, 0);
			else R[i].setpos(0, randx - 910);
			R[i].getShape().setOrigin(10, 10);
			randx = rand() % 801 + 100;
			xy = rand() % 2;
		}
	}
}
void startWave2(int& cnt, Clock& rocktimer, Clock& wave2){
	cnt = 0;
	rockWave(R, 0, 7);
	rocktimer.restart();
	wave2.restart();
}
void movingRocks(Rocks R[], Clock & rocktimer, int &cnt) {

	R[0].setpos(R[0].getShape().getPosition().x + R[0].getSpeedFactor(), R[0].getShape().getPosition().y + R[0].getSpeedFactor());
	for (int i = 1; i < 5 * cnt + 5; i++) {
		R[i].setpos(R[i].getShape().getPosition().x + R[i].getSpeedFactor(), R[i].getShape().getPosition().y + R[i].getSpeedFactor());
		R[i].getShape().setRotation(R[i].getShape().getRotation() + 30);
	}		if (rocktimer.getElapsedTime().asSeconds() > 1.4) {
		cnt++;
		rockWave(R, 5 * cnt, 5 * cnt + 5);

		rocktimer.restart();

	}
}
void descendingChickensInitial(Chicken a[], Chicken b[], Chicken c[], int size,int level) {
	int randx = rand() % 501;
	int before;
	for (int i = 0; i < size; i++) {

		a[i].setDescending(level);
		a[i].setpos(randx, -110);
		if (i < 9) {
			b[i].setDescending(level);

			before = rand() % 200 + 60;
			b[i].setpos(randx + before + 90, -110);
		}

		if (i < 5) {
			c[i].setDescending(level);
			c[i].setpos(randx + before + 160, -110);
		}
		randx = rand() % 501;
	}
}
void descendingChickensMoving(Chicken &a,int level){
	if (a.getHealthBar() == level+1)
		a.setpos(a.getCHShape().getPosition().x, a.getCHShape().getPosition().y + a.getSpeedFactor());

}
bool descendingChickensAlive(Chicken a[], int s, int e) {
	for (int i = s; i < e; i++) {
		if (a[i].Dead == false)
			return true;
	}
	return false;
}
void dealwithChickens(Chicken a[], Chicken b[], Chicken c[], int i, int ib, int ic,int level) {

	descendingChickensMoving(a[i], level);

	if (i < ib)
		descendingChickensMoving(b[i], level);
	if (i<ic)
		descendingChickensMoving(c[i],level);

	int random = rand() % 3000;
	if (random == 0 && a[i].getCHShape().getPosition().y<600 && a[i].getCHShape().getPosition().y>5) {
		if (!a[i].Dead) {
			E.push_back(egg); E.at(E.size() - 1).setSpeedfactor(((rand() % 51) / 100.0) + 1);
			E.at(E.size() - 1).setpos(a[i].getCHShape().getPosition().x + 30, a[i].getCHShape().getPosition().y + 80);
			random = rand() % 3000;
		}
	}
	random = rand() % 3000;
	if (i < ib) {
		if (random == 0 && b[i].getCHShape().getPosition().y<600 && b[i].getCHShape().getPosition().y>5) {
			if (!b[i].Dead) {
				E.push_back(egg); E.at(E.size() - 1).setSpeedfactor(((rand() % 51) / 100.0) + 1);
				E.at(E.size() - 1).setpos(b[i].getCHShape().getPosition().x + 30, b[i].getCHShape().getPosition().y + 80);
				random = rand() % 3000;
			}
		}
	}

	random = rand() % 3000;
	if (random == 0 && c[i].getCHShape().getPosition().y<600 && c[i].getCHShape().getPosition().y>5) {
		if (i < ic) {
			if (!c[i].Dead) {
				E.push_back(egg); E.at(E.size() - 1).setSpeedfactor(((rand() % 51) / 100.0) + 1);
				E.at(E.size() - 1).setpos(c[i].getCHShape().getPosition().x + 30, c[i].getCHShape().getPosition().y + 80);
				random = rand() % 3000;
			}
		}
	}
}
void hittingChickenDescending(Chicken a[], Chicken b[], Chicken c[], Player &player) {
	for (int i = 0; i < 5; i++) {
		for (int m = 0; m < B.size(); m++) {
			if (a[i].Dead == false && a[i].getCHShape().getGlobalBounds().intersects(B.at(m).getArrow().getGlobalBounds()))
			{
				a[i].minusHealth();
				B.erase(B.begin() + m);
			}
			else	if (b[i].Dead == false && b[i].getCHShape().getGlobalBounds().intersects(B.at(m).getArrow().getGlobalBounds()))
			{
				b[i].minusHealth();
				B.erase(B.begin() + m);
			}

			else	if (c[i].Dead == false && c[i].getCHShape().getGlobalBounds().intersects(B.at(m).getArrow().getGlobalBounds()))
			{
				c[i].minusHealth();
				B.erase(B.begin() + m);
			}
			else if (B.at(m).getArrow().getPosition().y < -2)
				B.erase(B.begin() + m);
		}
	}

	for (int i = 5; i < 9; i++) {
		for (int m = 0; m < B.size(); m++) {
			if (a[i].Dead == false && a[i].getCHShape().getGlobalBounds().intersects(B.at(m).getArrow().getGlobalBounds()))
			{
				a[i].minusHealth();
				B.erase(B.begin() + m);
			}
			else	if (b[i].Dead == false && b[i].getCHShape().getGlobalBounds().intersects(B.at(m).getArrow().getGlobalBounds()))
			{
				b[i].minusHealth();
				B.erase(B.begin() + m);
			}
			else if (B.at(m).getArrow().getPosition().y < -50)
				B.erase(B.begin() + m);
		}
	}

	for (int i = 9; i < 20; i++) {
		for (int m = 0; m < B.size(); m++) {
			if (a[i].Dead == false && a[i].getCHShape().getGlobalBounds().intersects(B.at(m).getArrow().getGlobalBounds()))
			{
				a[i].minusHealth();
				B.erase(B.begin() + m);
			}
			else if (B.at(m).getArrow().getPosition().y < -50)
				B.erase(B.begin() + m);
		}
	}
}
void moveAround(Chicken & a, Player & player) {
	Vector2f direction = a.normalize(player.getShip().getPosition() - a.getCHShape().getPosition());
	if (a.getCHShape().getPosition().y<800)
		a.setpos(a.getCHShape().getPosition().x + a.getSpeedFactor()*direction.x, a.getCHShape().getPosition().y + a.getSpeedFactor()*direction.y);
	else 	a.setpos(a.getCHShape().getPosition().x - a.getSpeedFactor()*direction.x, a.getCHShape().getPosition().y - a.getSpeedFactor()*direction.y);
}
void afterhittingdescending(Chicken a[], Chicken b[], Chicken c[], Player & player,int level) {
	for (int i = 0; i < 20; i++) {
		if (a[i].getHealthBar() <=level &&!player.Dead) {
			//if (level == 3)
				//a[i].setTheme();
			if (!player.hitbyegg)
			moveAround(a[i], player);
		}
		if (i<9)
		if (b[i].getHealthBar() <= level && !player.Dead) {
			//if (level == 3)
				//b[i].setTheme();
			if (!player.hitbyegg)
			moveAround(b[i], player);
		}
		if (i<5)
		if (c[i].getHealthBar() <= level && !player.Dead) {
			//if (level == 3)
			//	c[i].setTheme();
			if (!player.hitbyegg)
			moveAround(c[i], player);
		}
	}
}
void pickupsChickenDescending(Chicken a[], Chicken b[], Chicken c[], Player & player) {
	for (int i = 0; i < 20; i++) {
		if (a[i].Dead && a[i].justdead) {
			a[i].justdead = false;
			player.setScore(player.getScore() + 100 * a[i].getLevel());
			if (rand() % 10 == 0)
			{
				BO[n].setName(bonus(rand() % 4));
				BO[n++].setpos(a[i].getCHShape().getPosition().x + 60, a[i].getCHShape().getPosition().y + 80);
			}
		}

		if (i < 9) {
			if (b[i].Dead && b[i].justdead) {
				b[i].justdead = false;
				player.setScore(player.getScore() + 100 * b[i].getLevel());
				if (rand() % 10 == 0)
				{
					BO[n].setName(bonus(rand() % 4));
					BO[n++].setpos(b[i].getCHShape().getPosition().x + 60, b[i].getCHShape().getPosition().y + 80);
				}
			}
		}

		if (i < 5) {
			if (c[i].Dead && c[i].justdead) {
				c[i].justdead = false;
				player.setScore(player.getScore() + 100 * c[i].getLevel());
				if (rand() % 10 == 0)
				{
					BO[n].setName(bonus(rand() % 4));
					BO[n++].setpos(c[i].getCHShape().getPosition().x + 60, c[i].getCHShape().getPosition().y + 80);
				}
			}
		}
	}
}
void handleChickenDescendingWave(Chicken a[], Chicken b[], Chicken c[], Player &player, Clock &descendingtimer,int level) {
	switch (int(descendingtimer.getElapsedTime().asSeconds())) {
	case 0:
		descendingChickensMoving(a[0], level);
		descendingChickensMoving(b[0], level);
		descendingChickensMoving(c[0], level);
		break;
	case 1:
		for (int i = 0; i < 2; i++)
			dealwithChickens(a, b, c, i, 1, 1,level);
		break;
	case 2:
		for (int i = 0; i < 3; i++)
			dealwithChickens(a, b, c, i, 2, 1, level);
		break;
	case 3:
		for (int i = 0; i < 4; i++)
			dealwithChickens(a, b, c, i, 2, 2, level);
		break;
	case 4:
		for (int i = 0; i < 5; i++)
			dealwithChickens(a, b, c, i, 3, 2, level);
		break;
	case 5:
		for (int i = 0; i < 6; i++)
			dealwithChickens(a, b, c, i, 3, 2, level);
		break;
	case 6:
		for (int i = 0; i < 7; i++)
			dealwithChickens(a, b, c, i, 4, 2, level);
		break;
	case 7:
		for (int i = 0; i < 8; i++)
			dealwithChickens(a, b, c, i, 4, 3, level);
		break;
	case 8:
		for (int i = 0; i < 9; i++)
			dealwithChickens(a, b, c, i, 4, 3, level);
		break;
	case 9:
		for (int i = 0; i < 10; i++)
			dealwithChickens(a, b, c, i, 4, 3, level);
		break;

	case 10:
		for (int i = 0; i < 11; i++)
			dealwithChickens(a, b, c, i, 5, 3, level);
		break;
	case 11:
		for (int i = 0; i < 12; i++)
			dealwithChickens(a, b, c, i, 5, 4, level);
		break;
	case 12:
		for (int i = 0; i < 13; i++)
			dealwithChickens(a, b, c, i, 5, 4,level);
		break;
	case 13:
		for (int i = 0; i < 14; i++)
			dealwithChickens(a, b, c, i, 5, 4, level);
		break;
	case 14:
		for (int i = 0; i < 15; i++)
			dealwithChickens(a, b, c, i, 6, 4, level);
		break;
	case 15:
		for (int i = 0; i < 16; i++)
			dealwithChickens(a, b, c, i, 6, 5, level);
		break;
	case 16:
		for (int i = 0; i < 17; i++)
			dealwithChickens(a, b, c, i, 7, 5, level);
		break;
	case 17:
		for (int i = 0; i < 18; i++)
			dealwithChickens(a, b, c, i, 7, 5, level);
		break;
	case 18:
		for (int i = 0; i < 19; i++)
			dealwithChickens(a, b, c, i, 8, 5, level);
		break;
	default:
		for (int i = 0; i < 20; i++)
			dealwithChickens(a, b, c, i, 9, 5, level);
		break;

	}
	hittingChickenDescending(a, b, c, player);
	afterhittingdescending(a, b, c, player,level);
	pickupsChickenDescending(a, b, c, player);
}

void handleStore(Player &player) {
	RenderWindow Rocket(sf::VideoMode(800, 650, 32), "Choose Rocket");
	Sprite rocket1;
	Sprite rocket2;
	Sprite rocket4;
	Sprite rocket5;
	Sprite rocket6;
	Text text;
	Text text2, text4, text5, text6;
	Font font;
	font.loadFromFile("futureforcescondital.ttf");
	text.setString("Rocket Store");
	text.setScale(1.5, 1.5);
	text.setFillColor(sf::Color::Cyan);
	text.setCharacterSize(80);
	text.setPosition(30, 0);
	text.setFont(font);
	text2.setFont(font); text2.setFillColor(Color::White); text2.setPosition(150, 250); text2.setString("Lives = 4"); text2.setCharacterSize(40);
	text4.setFont(font); text4.setFillColor(Color::White); text4.setPosition(150, 500); text4.setString("Lives = 5"); text4.setCharacterSize(40);
	text5.setFont(font); text5.setFillColor(Color::White); text5.setPosition(520, 250); text5.setString("Lives = 6"); text5.setCharacterSize(40);
	text6.setFont(font); text6.setFillColor(Color::White); text6.setPosition(520, 500); text6.setString("Lives = 7"); text6.setCharacterSize(40);

	Texture R1;
	R1.loadFromFile("earth.jpg");
	rocket1.setTexture(R1);
	rocket1.setScale(1.3, 1.3);

	Texture* R2;
	R2 = new sf::Texture();
	R2->loadFromFile("rocket2.png");
	rocket2.setTexture(*R2);
	rocket2.setScale(0.3, 0.3);
	rocket2.setPosition(70, 250);

	Texture* R4;
	R4 = new sf::Texture();
	R4->loadFromFile("rocket4.png");
	rocket4.setTexture(*R4);
	rocket4.setScale(0.25, 0.25);
	rocket4.setPosition(70, 500);

	Texture* R5;
	R5 = new sf::Texture();
	R5->loadFromFile("rocket5 (1).png");
	rocket5.setTexture(*R5);
	rocket5.setScale(0.4, 0.4);
	rocket5.setPosition(400, 250);

	Texture* R6;
	R6 = new sf::Texture();
	R6->loadFromFile("rocket6.png");
	rocket6.setTexture(*R6);
	rocket6.setScale(1.1, 1.1);
	rocket6.setPosition(400, 500);

	Event event;

	while (Rocket.isOpen()) {
		while (Rocket.pollEvent(event))
		{
			if (event.type == Event::Closed)
				Rocket.close();
		}

		if (Mouse::isButtonPressed(Mouse::Left)) {

			if (rocket2.getGlobalBounds().contains(Rocket.mapPixelToCoords(Mouse::getPosition(Rocket))))
			{
				player.setShip(R2,2);
				player.setLifes(4);
				//std::cout << "rocket2";
			}

			else if (rocket4.getGlobalBounds().contains(Rocket.mapPixelToCoords(Mouse::getPosition(Rocket))))
			{
				player.setShip(R4,4);
				player.setLifes(5);
				//std::cout << "rocket4";
			}
			else if (rocket5.getGlobalBounds().contains(Rocket.mapPixelToCoords(Mouse::getPosition(Rocket))))
			{
				player.setShip(R5,5);
				player.setLifes(6);
				//std::cout << "rocket5";
			}
			else if (rocket6.getGlobalBounds().contains(Rocket.mapPixelToCoords(Mouse::getPosition(Rocket))))
			{
				player.setShip(R6,6);
				player.setLifes(7);
				//std::cout << "rocket6";
			}
		}
		Rocket.clear();

		Rocket.draw(rocket1);
		Rocket.draw(rocket2);
		Rocket.draw(rocket4);
		Rocket.draw(rocket5);
		Rocket.draw(rocket6);

		Rocket.draw(text);Rocket.draw(text2);Rocket.draw(text4);Rocket.draw(text5);Rocket.draw(text6);

		Rocket.display();
	}
}