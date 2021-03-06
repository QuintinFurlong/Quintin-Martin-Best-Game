#include "MainMenu.h"
#include "Colour.h"

MainMenu::MainMenu()
{
	//load font
	if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "error";
	}
	//set font
	m_title.setFont(m_font);
	m_backStory.setFont(m_font);
	m_controls.setFont(m_font);
	m_controlScheme.setFont(m_font);
	m_backStoryDisplay.setFont(m_font);
	m_levelSelect.setFont(m_font);

	//set string
	m_title.setString("Hoppin & Droppin");
	m_backStory.setString("Boss Guide");
	m_controls.setString("Controls");
	m_controlScheme.setString("Press 'a' to jump \n Use left analogstick to move \n Use right analogstick to aim \n Press 'RT' to shoot\n Press 'back' to return to menu from game");
	m_backStoryDisplay.setString("This game has 5 unique bosses each in its own level \nSPIDER(LEVEL 1) "
		"\nAfter making it up the tower, past some enemies you'll get to a platform to high to jump on to"
		".You'll have \nto use enemy bodies to reach the spider.The spider has low health and runs back and forward on the floor.\n"
		"WALL(LEVEL 2) \nThe wall boss is at the top and moves back and forward.The wall boss has a lot of health and is too tall \nto jump over.\n"
		"BIRD(LEVEL 3) \nThe bird flies around the arena ignoring platform blocks \nKangaroo(LEVEL 4)\n"
		"The kangaroo jumps the height of the arena from the ground and moves back and forward \nMutant(LEVEL 5)\nThe mutant teleports between the top 5 platforms");
	m_levelSelect.setString("Level Select");

	//setPosition
	m_title.setPosition(sf::Vector2f(430, 70));
	m_backStory.setPosition(sf::Vector2f(620, 310));
	m_controls.setPosition(sf::Vector2f(620, 410));
	m_controlScheme.setPosition(sf::Vector2f(20, 20));
	m_backStoryDisplay.setPosition(sf::Vector2f(20, 20));
	m_levelSelect.setPosition(sf::Vector2f(620, 210));

	//setSize
	m_title.setCharacterSize(56);
	m_backStory.setCharacterSize(24);
	m_controls.setCharacterSize(24);
	m_controlScheme.setCharacterSize(24);
	m_backStoryDisplay.setCharacterSize(24);
	m_levelSelect.setCharacterSize(24);

	//set colour
	m_title.setFillColor(MENU_TITLE);
	m_backStory.setFillColor(MENU_TEXT);
	m_controls.setFillColor(MENU_TEXT);
	m_controlScheme.setFillColor(MENU_TEXT);
	m_backStoryDisplay.setFillColor(MENU_TEXT);
	m_levelSelect.setFillColor(MENU_TEXT);

	for (int i = 0; i < 3; i++)
	{
		m_buttons[i].setPosition(sf::Vector2f(400, i * 100 + 200));
		m_buttons[i].setSize(sf::Vector2f(600, 60));
		m_buttons[i].setFillColor(MENU_BUTTON_FILL);
		m_buttons[i].setOutlineColor(MENU_OUTLINE_OFF);
		m_buttons[i].setOutlineThickness(15);
	}

}

void MainMenu::update(sf::Time &t_time, GameState &gamestate)
{
	if (sf::Joystick::isButtonPressed(0, 0))//if a is pressed
	{
		if (count == 0)
		{
			gamestate = GameState::Levels; //goes to level select
		}
		else if (count == 1)
		{
			displayLore = true; //displays controls
		}
		else if (count == 2)
		{
			displayControls = true; //displays lore
		}
	}
	if (displayControls)
	{
		if (sf::Joystick::isButtonPressed(0, 1))
		{
			displayControls = false; //returns to main menu
		}
	}
	if (displayLore)
	{
		if (sf::Joystick::isButtonPressed(0, 1))
		{
			displayLore = false; //returns to main menu
		}
	}

	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) > 20) //if stick is moved down
	{
		if (stickMoved)
		{
			m_buttons[count].setOutlineColor(MENU_OUTLINE);
			count++;
			stickMoved = false;
		}
	}
	else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) < -20) //if stick is moved up
	{
		if (stickMoved)
		{
			m_buttons[count].setOutlineColor(MENU_OUTLINE);
			count--;
			stickMoved = false;
		}
	}
	else
	{
		stickMoved = true;
	}

	if (count > 2)
	{
		count = 0;
	}
	else if (count < 0)
	{
		count = 2;
	}
	for (int i = 0; i < 3; i++)
	{
		m_buttons[i].setOutlineColor(MENU_OUTLINE_OFF);
		m_buttons[count].setOutlineColor(MENU_OUTLINE_ON);
	}
}

void MainMenu::render(sf::RenderWindow & t_window)
{
	if (!displayControls && !displayLore)
	{
		for (int i = 0; i < 3; i++)
		{
			t_window.draw(m_buttons[i]);
		}

		t_window.draw(m_title);
		sf::Text temp = m_title;
		temp.move(3,-3);
		temp.setFillColor(sf::Color(temp.getFillColor().r, temp.getFillColor().g, temp.getFillColor().b, 150));
		t_window.draw(temp);
		t_window.draw(m_controls);
		t_window.draw(m_backStory);
		t_window.draw(m_levelSelect);

	}
	else if (displayControls)//if y has been pressed
	{
		t_window.draw(m_controlScheme);//display controls
	}
	else//if x has been pressed
	{
		t_window.draw(m_backStoryDisplay);//display lore
	}
}
