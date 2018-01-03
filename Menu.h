#ifndef Menu_h
#define Menu_h
#define MAX_NUMBER_OF_ITEMS 3
#include "SFML/Graphics.hpp"
#include <string>
class Menu
{
private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[MAX_NUMBER_OF_ITEMS];
	sf::Texture bg2;
	sf::Sprite background2;

public:
	Menu(float width, float height)
	{

		bg2.loadFromFile("logo.jpg");
		background2.setTexture(bg2);
		background2.setPosition(-10, 0);

		if (!font.loadFromFile("arial.ttf"))
		{
			std::cout << "font not loaded";
		}

		menu[0].setFont(font);
		menu[0].setFillColor(sf::Color::Red);
		menu[0].setString("Play");
		menu[0].setPosition(sf::Vector2f(width / 2 - 50, 200 + (height - 200) / (MAX_NUMBER_OF_ITEMS + 1) * 1));

		menu[1].setFont(font);
		menu[1].setFillColor(sf::Color::White);
		menu[1].setString("Options");
		menu[1].setPosition(sf::Vector2f(width / 2 - 70, 200 + (height - 200) / (MAX_NUMBER_OF_ITEMS + 1) * 2));

		menu[2].setFont(font);
		menu[2].setFillColor(sf::Color::White);
		menu[2].setString("Return to Main Menu");
		menu[2].setPosition(sf::Vector2f(width / 2 - 150, 200 + (height - 200) / (MAX_NUMBER_OF_ITEMS + 1) * 3));

		selectedItemIndex = 0;
	}; 
	Menu(float width, float height,std::string title1,std::string title2,std::string title3)
	{

		bg2.loadFromFile("logo.jpg");
		background2.setTexture(bg2);
		background2.setPosition(-10, 0);
		if (!font.loadFromFile("arial.ttf"))
		{
			std::cout << "font not loaded";
		}

		menu[0].setFont(font);
		menu[0].setFillColor(sf::Color::Red);
		menu[0].setString(title1);
		menu[0].setPosition(sf::Vector2f(width / 2 -  30-menu[0].getString().getSize() * 5, 200 + (height - 200) / (MAX_NUMBER_OF_ITEMS + 1) * 1));
		
		menu[1].setFont(font);
		menu[1].setFillColor(sf::Color::White);
		menu[1].setString(title2);
		menu[1].setPosition(sf::Vector2f(width / 2 - 30 - menu[1].getString().getSize() * 5, 200 + (height - 200) / (MAX_NUMBER_OF_ITEMS + 1) * 2));

		menu[2].setFont(font);
		menu[2].setFillColor(sf::Color::White);
		menu[2].setString(title3);
		menu[2].setPosition(sf::Vector2f(width / 2 - 30 - menu[2].getString().getSize() * 5, 200 + (height - 200) / (MAX_NUMBER_OF_ITEMS + 1) * 3));

		selectedItemIndex = 0;
	};
	
	~Menu()
	{}
	;

	void draw(sf::RenderWindow &window)
	{
		window.draw(background2);
		for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
		{
			window.draw(menu[i]);
		}
	};
	void MoveUp()
	{
		if (selectedItemIndex - 1 >= 0)
		{
			menu[selectedItemIndex].setFillColor(sf::Color::White);
			selectedItemIndex--;
			menu[selectedItemIndex].setFillColor(sf::Color::Red);
		}
	};
	void MoveDown()
	{
		if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
		{
			menu[selectedItemIndex].setFillColor(sf::Color::White);
			selectedItemIndex++;
			menu[selectedItemIndex].setFillColor(sf::Color::Red);
		}
	};
	int GetPressedItem()const { return selectedItemIndex; }
	void setPressedItem(int presseditem){ 
		selectedItemIndex = presseditem;
	};
	void disable(sf::RenderWindow& App){
		for (int i = 0; i<3; i++)menu[i].setPosition(App.getSize().x + menu[i].getPosition().x, App.getSize().y + menu[i].getPosition().y);
	};
	void enable(sf::RenderWindow& App){

		for (int i = 0; i<3; i++)menu[i].setPosition(sf::Vector2f(App.getSize().x/ 2 - 30 - menu[i].getString().getSize() * 5, 200 + (App.getSize().y - 200) / (MAX_NUMBER_OF_ITEMS + 1) * (i+1)));
	};
};
#endif